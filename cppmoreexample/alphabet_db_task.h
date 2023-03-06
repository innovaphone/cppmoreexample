namespace Appcppmoreexample {
    class TaskReadAlphabet : public ITask, public UDatabase {
    protected:
        void DatabaseExecSQLResult(IDatabase * const database, class IDataSet * dataset) override;
        void DatabaseError(IDatabase * const database, db_error_t error) override;
        virtual void SendGetLettersResponse() = 0;

        class IDatabase * database;
        ulong64 offset;
        unsigned int limit;
        class IDataSet * dataset = 0;
    public:
        TaskReadAlphabet(IDatabase * database, ulong64 offset, unsigned int limit);
        ~TaskReadAlphabet();
        void Start(class UTask * user) override;
    };

    class GetLetters : public TaskReadAlphabet {
        virtual void SendGetLettersResponse() override;
        class IJsonApiConnection * connection;
        char * src;
    public:
        GetLetters(class IJsonApiConnection * connection, IDatabase * database, ulong64 offset, unsigned int limit, const char * src);
        ~GetLetters();
    };
}