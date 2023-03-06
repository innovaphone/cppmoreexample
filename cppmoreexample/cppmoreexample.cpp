/*-----------------------------------------------------------------------------------------------*/
/* Based on innovaphone App template                                                             */
/* copyright (c) innovaphone 2018                                                                */
/*                                                                                               */
/*-----------------------------------------------------------------------------------------------*/

#include "platform/platform.h"
#include "common/os/iomux.h"
#include "common/interface/task.h"
#include "common/interface/socket.h"
#include "common/interface/webserver_plugin.h"
#include "common/interface/database.h"
#include "common/interface/json_api.h"
#include "common/ilib/str.h"
#include "common/ilib/json.h"
#include "common/lib/appservice.h"
#include "common/lib/config.h"
#include "common/lib/tasks_postgresql.h"
#include "common/lib/appwebsocket.h"
#include "common/lib/app_updates.h"

#include "cppmoreexample_tasks.h"
#include "alphabet_api.h"
#include "cppmoreexample.h"


#define DBG(x) //debug->printf x

using namespace Appcppmoreexample;

/*-----------------------------------------------------------------------------------------------*/
/* class cppmoreexampleService                                                                        */
/*-----------------------------------------------------------------------------------------------*/

cppmoreexampleService::cppmoreexampleService(class IIoMux * const iomux, class ISocketProvider * localSocketProvider, class IWebserverPluginProvider * const webserverPluginProvider, class IDatabaseProvider * databaseProvider, AppServiceArgs * args) : AppService(iomux, localSocketProvider, args)
{
    this->iomux = iomux;
    this->localSocketProvider = localSocketProvider;
    this->webserverPluginProvider = webserverPluginProvider;
    this->databaseProvider = databaseProvider;
}

cppmoreexampleService::~cppmoreexampleService()
{

}

void cppmoreexampleService::AppServiceApps(istd::list<AppServiceApp> * appList)
{
    appList->push_back(new AppServiceApp("innovaphone-cppmoreexample"));
    appList->push_back(new AppServiceApp("innovaphone-cppmoreexampleadmin"));
}

void cppmoreexampleService::AppInstancePlugins(istd::list<AppInstancePlugin> * pluginList)
{
    pluginList->push_back(new AppInstancePlugin("innovaphone.cppmoreexamplemanager", "innovaphone-cppmoreexample.png", "innovaphone.cppmoreexamplemanagertexts"));
}

class AppInstance * cppmoreexampleService::CreateInstance(AppInstanceArgs * args)
{
    return new cppmoreexample(iomux, this, args);
}

/*-----------------------------------------------------------------------------------------------*/
/* class cppmoreexample                                                                               */
/*-----------------------------------------------------------------------------------------------*/

cppmoreexample::cppmoreexample(IIoMux * const iomux, class cppmoreexampleService * service, AppInstanceArgs * args) : AppInstance(service, args), AppUpdates(iomux), ConfigContext(nullptr, this)
{
    this->stopping = false;
    this->iomux = iomux;
    this->service = service;
    this->webserverPlugin = service->webserverPluginProvider->CreateWebserverPlugin(iomux, service->localSocketProvider, this, args->webserver, args->webserverPath, this);
    this->database = service->databaseProvider->CreateDatabase(iomux, this, this);
    this->database->Connect(args->dbHost, args->dbName, args->dbUser, args->dbPassword);
    this->logFlags |= LOG_APP;
    this->logFlags |= LOG_APP_WEBSOCKET;
    this->logFlags |= LOG_DATABASE;
	this->currentTask = nullptr;
	this->currentId = 0;
	this->count = 0;

    RegisterJsonApi(this);

    this->alphabetApiContext = new AlphabetApiContext("Alphabet", this, database);

    Log("App instance started");
}

cppmoreexample::~cppmoreexample()
{
    delete this->alphabetApiContext;
}

void cppmoreexample::DatabaseConnectComplete(IDatabase * const database)
{
    currentTask = new DatabaseInit(this, database);
}

void cppmoreexample::DatabaseInitComplete()
{
    delete currentTask;
    currentTask = nullptr;

    webserverPlugin->HttpListen(nullptr, nullptr, nullptr, nullptr, _BUILD_STRING_);
    webserverPlugin->WebsocketListen();
    Log("App instance initialized");
}


void cppmoreexample::cppmoreexampleSessionClosed(class cppmoreexampleSession * session)
{
    sessionList.remove(session);
    delete session;
    if (stopping) {
        TryStop();
    }
}

void cppmoreexample::DatabaseShutdown(IDatabase * const database, db_error_t reason)
{
    this->database = nullptr;
    TryStop();
}

void cppmoreexample::DatabaseError(IDatabase * const database, db_error_t error)
{

}

void cppmoreexample::ServerCertificateUpdate(const byte * cert, size_t certLen)
{
    Log("cppmoreexample::ServerCertificateUpdate cert:%x certLen:%u", cert, certLen);
}

void cppmoreexample::WebserverPluginWebsocketListenResult(IWebserverPlugin * plugin, const char * path, const char * registeredPathForRequest, const char * host)
{
    if (!this->stopping) {
        class cppmoreexampleSession * session = new cppmoreexampleSession(this);
        this->sessionList.push_back(session);
    }
    else {
        plugin->Cancel(wsr_cancel_type_t::WSP_CANCEL_UNAVAILABLE);
    }
}

void cppmoreexample::WebserverPluginHttpListenResult(IWebserverPlugin * plugin, ws_request_type_t requestType, char * resourceName, const char * registeredPathForRequest, ulong64 dataSize)
{
    if (requestType == WS_REQUEST_GET) {
        if (plugin->BuildRedirect(resourceName, _BUILD_STRING_, strlen(_BUILD_STRING_))) {
            return;
        }
    }
    plugin->Cancel(WSP_CANCEL_NOT_FOUND);
}

void cppmoreexample::WebserverPluginClose(IWebserverPlugin * plugin, wsp_close_reason_t reason, bool lastUser)
{
    Log("WebserverPlugin closed");
    if (lastUser) {
        delete webserverPlugin;
        webserverPlugin = nullptr;
        TryStop();
    }
}

void cppmoreexample::Stop()
{
    TryStop();
}

void cppmoreexample::TryStop()
{
    if (!stopping) {
        Log("App instance stopping");
        stopping = true;
        if (webserverPlugin) webserverPlugin->Close();
        if (database) database->Shutdown();
        for (std::list<cppmoreexampleSession *>::iterator it = sessionList.begin(); it != sessionList.end(); ++it) {
            (*it)->Close();
        }
    }
    
    if (!webserverPlugin && !database && sessionList.empty()) appService->AppStopped(this);
}


/*-----------------------------------------------------------------------------------------------*/
/* class cppmoreexampleSession                                                                        */
/*-----------------------------------------------------------------------------------------------*/

cppmoreexampleSession::cppmoreexampleSession(class cppmoreexample * instance) : AppUpdatesSession(instance, instance->webserverPlugin, instance, instance)
{
    this->instance = instance;

    this->currentTask = nullptr;
    this->currentSrc = nullptr;
    this->closed = false;
    this->closing = false;

    this->admin = false;
    this->adminApp = false;
}

cppmoreexampleSession::~cppmoreexampleSession()
{
}

void cppmoreexampleSession::AppWebsocketMessage(class json_io & msg, word base, const char * mt, const char * src)
{
    if (currentSrc) free(currentSrc);
    currentSrc = src ? _strdup(src) : nullptr;
    if (!strcmp(mt, "PbxInfo")) {
        AppWebsocketMessageComplete();
    }
    else {
        AppWebsocketMessageComplete();
    }
}

void cppmoreexampleSession::AppWebsocketAppInfo(const char * app, class json_io & msg, word base)
{

}

bool cppmoreexampleSession::AppWebsocketConnectComplete(class json_io & msg, word info)
{
    const char * appobj = msg.get_string(info, "appobj");
    if (appobj && !strcmp(appobj, sip)) admin = true;
    if (!strcmp(app, "innovaphone-cppmoreexampleadmin")) adminApp = true;
    return true;
}

void cppmoreexampleSession::AppWebsocketClosed()
{
    closed = true;
    if (currentTask && currentTask->complete) currentTask->Finished();
    TryClose();
}

void cppmoreexampleSession::ResponseSent()
{
    if (currentTask) {
        currentTask->Finished();
    }
    else {
        AppWebsocketMessageComplete();
    }
}

void cppmoreexampleSession::TryClose()
{
    closing = true;
    if (!closed) {
        AppWebsocketClose();
        return;
    }
    if (currentTask) {
        currentTask->Stop();
        return;
    }
    if (closed && !currentTask) {
        instance->cppmoreexampleSessionClosed(this);
    }
}

void cppmoreexampleSession::Close()
{
    TryClose();
}

bool cppmoreexampleSession::CheckSession(class ITask * task)
{
    if (closing) {
        if (task) delete task;
        currentTask = nullptr;
        TryClose();
        return false;
    }
    return true;
}
