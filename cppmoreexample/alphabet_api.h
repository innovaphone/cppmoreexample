namespace Appcppmoreexample {
    class AlphabetApiContext : public UJsonApiContext {
        class JsonApiContext * jsonApiContext;
        class IDatabase * database;
        char * name;
        class AlphabetApi * alphabetApi;

    public:
        AlphabetApiContext(const char * name, JsonApiContext * jsonApiContext, IDatabase * database);
        virtual ~AlphabetApiContext();

        class JsonApi * CreateJsonApi(class IJsonApiConnection * connection, class json_io & msg, word base) override;
        class JsonApi * JsonApiRequested(class IJsonApiConnection * connection) override;
        const char * Name() override;
    };

    class AlphabetApi : public JsonApi, public UTask {
        char * name;
        IJsonApiConnection * connection;
        IDatabase * database;
        ITask * task;
        void TaskComplete(class ITask * const task) override;
        void TaskFailed(class ITask * const task) override { this->TaskComplete(task); }

    public:
        AlphabetApi(const char * name, IJsonApiConnection * connection, IDatabase * database);
        virtual ~AlphabetApi();

        const char * Name() override;
        void Message(class json_io & msg, word base, const char * mt, const char * src) override;
        void JsonApiConnectionClosed() override;

    };
}