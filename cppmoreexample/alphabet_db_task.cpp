#include "platform/platform.h"
#include "common/interface/database.h"
#include "common/interface/task.h"
#include "common/interface/json_api.h"
#include "common/ilib/json.h"
#include "common/lib/tasks_postgresql.h"
#include "common/lib/database_switch.h"

#include "alphabet_db_task.h"
#include "alphabet_api.h"

using namespace Appcppmoreexample;

TaskReadAlphabet::TaskReadAlphabet(IDatabase * database, ulong64 offset, unsigned int limit)
{
    this->database = database;
    this->offset = offset;
    this->limit = limit;
}

TaskReadAlphabet::~TaskReadAlphabet()
{
    delete dataset;
}

void TaskReadAlphabet::Start(UTask * user)
{
    this->user = user;
    //  The database table alphabet is initialized in the constructor of TaskDbInit class, but is not filled with data.
    //  Following statement could be used on the psql console to insert the entries
    //  (SSH to AP, use pass ipapps; 'su -' to root, use pass iplinux; psql -d databasename):
    //  INSERT INTO alphabet (letter) VALUES ('Alpha'), ('Bravo'), ('Charlie'), ('Delta'), ('Echo'), ('Foxtrot'), ('Golf'),
    //  ('Hotel'), ('India'), ('Juliet'), ('Kilo'), ('Lima'), ('Mike'), ('November'), ('Oscar'), ('Papa'), ('Quebec'),
    //  ('Romeo'), ('Sierra'), ('Tango'), ('Uniform'), ('Victor'), ('Whiskey'), ('Xray'), ('Yankee'), ('Zulu');
    database->ExecSQL(this, DB_EXEC_FETCH_ALL, "SELECT * FROM alphabet WHERE id>%llu ORDER BY id ASC LIMIT %llu::BIGINT", offset, limit);
}

void TaskReadAlphabet::DatabaseExecSQLResult(IDatabase * const database, IDataSet * dataset)
{
    this->dataset = dataset;
    this->SendGetLettersResponse();
}

void TaskReadAlphabet::DatabaseError(IDatabase * const database, db_error_t error)
{
    Failed();
}


GetLetters::GetLetters(IJsonApiConnection * connection, IDatabase * database, ulong64 offset, unsigned int limit, const char * src) 
    : TaskReadAlphabet(database, offset, limit)
{
    this->connection = connection;
    this->src = _strdup(src);
}

GetLetters::~GetLetters()
{
    free(src);
}

void GetLetters::SendGetLettersResponse()
{
    if (this->connection) {
        const char * name = ((class AlphabetApi *)this->user)->Name();

        char sb[1000];
        char b[300]; // must fit all letters
        char * tmp = b;
        class json_io send(sb);
        word base = send.add_object(0xFFFF, 0);
        send.add_string(base, "api", name);
        send.add_string(base, "mt", "GetLettersResult");
        word letters = 0;
        ulong64 more = 0;
        if (this->dataset != nullptr) {
            while (!this->dataset->Eot()) { // check for the end of table
                if (letters == 0) letters = send.add_array(base, "letters"); // add letters array only at the first run and only if dataset is not empty
                send.add_printf(letters, 0, tmp, "%s", this->dataset->GetStringValue("letter")); // must use printf, cause calling Next invalidates pointer
                more = this->dataset->GetULong64Value("id"); // the value of the last id is stored in the local variable on stack
                this->dataset->Next(); // the pointers returned by the dataset Get functions are not more valid after the call of Next
            }
        }
        if (more) send.add_ulong64(base, "more", more, tmp);
        if (this->src) send.add_string(base, "src", this->src);
        this->connection->JsonApiMessage(send, sb);
    }
    this->Complete();
}