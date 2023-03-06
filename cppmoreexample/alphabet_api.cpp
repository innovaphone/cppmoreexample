#include "platform/platform.h"
#include "common/interface/json_api.h"
#include "common/ilib/json.h"
#include "common/interface/task.h"
#include "common/interface/database.h"

#include "alphabet_db_task.h"
#include "alphabet_api.h"

using namespace Appcppmoreexample;

AlphabetApiContext::AlphabetApiContext(const char * name, JsonApiContext * jsonApiContext, IDatabase * database)
{
    this->name = _strdup(name);
    this->jsonApiContext = jsonApiContext;
    this->database = database;
    this->jsonApiContext->RegisterJsonApi(this);
}

AlphabetApiContext::~AlphabetApiContext()
{
    free(name);
    delete this->alphabetApi;
}

class JsonApi * AlphabetApiContext::CreateJsonApi(IJsonApiConnection * connection, json_io & msg, word base)
{
    return 0;
}

class JsonApi * AlphabetApiContext::JsonApiRequested(IJsonApiConnection * connection)
{
    this->alphabetApi = new AlphabetApi(name, connection, database);
    return this->alphabetApi;
}

const char * AlphabetApiContext::Name()
{
    return name;
}

AlphabetApi::AlphabetApi(const char * name, IJsonApiConnection * connection, IDatabase * database)
{
    this->name = _strdup(name);
    this->connection = connection;
    this->database = database;
    this->task = nullptr;
}

AlphabetApi::~AlphabetApi()
{
    free(name);
}

const char * AlphabetApi::Name()
{
    return name;
}

void AlphabetApi::Message(json_io & msg, word base, const char * mt, const char * src)
{
    if (!strcmp(mt, "GetLetters")) {
        bool present = false;
        unsigned int limit = msg.get_unsigned(base, "limit", &present);
        if (!present || limit > 10 || limit == 0) limit = 10; // max limit 10
        ulong64 offset = msg.get_ulong64(base, "more", &present);
        if (!present) offset = 0;
        this->task = new GetLetters(connection, database, offset, limit, src);
        this->task->Start(this);
    }
}

void AlphabetApi::TaskComplete(ITask * const task)
{
    this->task = nullptr;
    delete task;
    if (this->connection) {
        this->connection->JsonApiMessageComplete();
    }
    else {
        delete this;
    }
}

void AlphabetApi::JsonApiConnectionClosed()
{
    this->connection = nullptr;
    if (this->task) return;
    delete this;
}