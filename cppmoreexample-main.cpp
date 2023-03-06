
/*-----------------------------------------------------------------------------------------------*/
/* Based on innovaphone App template                                                             */
/* copyright (c) innovaphone 2016                                                                */
/*                                                                                               */
/*-----------------------------------------------------------------------------------------------*/

#include "platform/platform.h"
#include "common/build/release.h"
#include "common/os/iomux.h"
#include "common/interface/task.h"
#include "common/interface/socket.h"
#include "common/interface/webserver_plugin.h"
#include "common/interface/database.h"
#include "common/interface/json_api.h"
#include "common/interface/random.h"
#include "common/lib/appservice.h"
#include "common/lib/config.h"
#include "common/lib/tasks_postgresql.h"
#include "common/lib/appwebsocket.h"
#include "common/lib/app_updates.h"
#include "common/lib/badgecount_signaling.h"
#include "cppmoreexample/cppmoreexample_tasks.h"
#include "cppmoreexample/cppmoreexample.h"

using namespace Appcppmoreexample;

int main(int argc, char *argv[])
{
    IRandom::Init(time(nullptr));
    class IIoMux * iomux = IIoMux::Create();
	ISocketProvider * localSocketProvider = CreateLocalSocketProvider();
    IWebserverPluginProvider * webserverPluginProvider = CreateWebserverPluginProvider();
    IDatabaseProvider * databaseProvider = CreatePostgreSQLDatabaseProvider();

    AppServiceArgs  serviceArgs;
    serviceArgs.serviceID = "cppmoreexample";
    serviceArgs.Parse(argc, argv);
    AppInstanceArgs instanceArgs;
    instanceArgs.appName = "cppmoreexample";
    instanceArgs.appDomain = "example.com";
    instanceArgs.appPassword = "pwd";
    instanceArgs.webserver = "/var/run/webserver/webserver";
    instanceArgs.webserverPath = "/cppmoreexample";
    instanceArgs.dbHost = "";
    instanceArgs.dbName = "cppmoreexample";
    instanceArgs.dbUser = "cppmoreexample";
    instanceArgs.dbPassword = "cppmoreexample";
    instanceArgs.Parse(argc, argv);
	
    cppmoreexampleService * service = new cppmoreexampleService(iomux, localSocketProvider, webserverPluginProvider, databaseProvider, &serviceArgs);
    if (!serviceArgs.manager) service->AppStart(&instanceArgs);
    iomux->Run();

    delete service;
	delete localSocketProvider;
    delete webserverPluginProvider;
    delete databaseProvider;
    delete iomux;
    delete debug;
    
    return 0;
}
