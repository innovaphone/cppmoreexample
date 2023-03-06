
/*-----------------------------------------------------------------------------------------------*/
/* Based on innovaphone App template                                                             */
/* copyright (c) innovaphone 2018                                                                */
/*                                                                                               */
/*-----------------------------------------------------------------------------------------------*/

namespace Appcppmoreexample {

    class cppmoreexampleService : public AppService {
        class AppInstance * CreateInstance(AppInstanceArgs * args) override;
        void AppServiceApps(istd::list<AppServiceApp> * appList) override;
        void AppInstancePlugins(istd::list<AppInstancePlugin> * pluginList) override;

    public:
        cppmoreexampleService(class IIoMux * const iomux, class ISocketProvider * localSocketProvider, IWebserverPluginProvider * const webserverPluginProvider, IDatabaseProvider * databaseProvider, AppServiceArgs * args);
        ~cppmoreexampleService();

        class IIoMux * iomux;
        class ISocketProvider * localSocketProvider;
        class IWebserverPluginProvider * webserverPluginProvider;
        class IDatabaseProvider * databaseProvider;
    };

    class cppmoreexample : public AppInstance, public AppUpdates, public UDatabase, public UWebserverPlugin, public JsonApiContext, public ConfigContext
    {
        void DatabaseConnectComplete(IDatabase * const database) override;
        void DatabaseShutdown(IDatabase * const database, db_error_t reason) override;
        void DatabaseError(IDatabase * const database, db_error_t error) override;

        void WebserverPluginClose(IWebserverPlugin * plugin, wsp_close_reason_t reason, bool lastUser) override;
        void WebserverPluginWebsocketListenResult(IWebserverPlugin * plugin, const char * path, const char * registeredPathForRequest, const char * host) override;
        void WebserverPluginHttpListenResult(IWebserverPlugin * plugin, ws_request_type_t requestType, char * resourceName, const char * registeredPathForRequest, ulong64 dataSize) override;

        void ServerCertificateUpdate(const byte * cert, size_t certLen) override;
        void Stop() override;

        void TryStop();

        bool stopping;
        class ITask * currentTask;
        std::list<class cppmoreexampleSession *> sessionList;
        class AlphabetApiContext * alphabetApiContext;

    public:
        cppmoreexample(IIoMux * const iomux, cppmoreexampleService * service, AppInstanceArgs * args);
        ~cppmoreexample();

        void DatabaseInitComplete();
        void cppmoreexampleSessionClosed(class cppmoreexampleSession * session);

        const char * appPwd() { return args.appPassword; };

        class IIoMux * iomux;
        class cppmoreexampleService * service;
        class IWebserverPlugin * webserverPlugin;
        class IDatabase * database;
        long64 count;

        ulong64 currentId;
    };

    class DatabaseInit : public TaskDbInit {
        class cppmoreexample * instance;
    public:
        DatabaseInit(class cppmoreexample * instance, IDatabase * database) : TaskDbInit(database) {
            this->instance = instance;
            Start(nullptr);
        }
        void Complete() { instance->DatabaseInitComplete(); };
    };

    class cppmoreexampleSession : public AppUpdatesSession {
        void AppWebsocketAccept(class UWebsocket * uwebsocket) { instance->webserverPlugin->WebsocketAccept(uwebsocket); };
        char * AppWebsocketPassword() override { return (char *)instance->appPwd(); };
        void AppWebsocketMessage(class json_io & msg, word base, const char * mt, const char * src) override;
        void AppWebsocketAppInfo(const char * app, class json_io & msg, word base) override;
        bool AppWebsocketConnectComplete(class json_io & msg, word info) override;
        void AppWebsocketClosed() override;

        void ResponseSent() override;

        void TryClose();

        bool closed;
        bool closing;
        bool admin;
        bool adminApp;

    public:
        cppmoreexampleSession(class cppmoreexample * instance);
        ~cppmoreexampleSession();

        bool CheckSession(class ITask * task);

        class cppmoreexample * instance;
        char * currentSrc;
        class ITask * currentTask;
        void Close();
    };
}


