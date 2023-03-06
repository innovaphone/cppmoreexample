
/*-----------------------------------------------------------------------------------------------*/
/* Based on innovaphone App template                                                             */
/* copyright (c) innovaphone 2018                                                                */
/*                                                                                               */
/*-----------------------------------------------------------------------------------------------*/

#include "platform/platform.h"
#include "common/interface/database.h"
#include "common/interface/task.h"
#include "common/lib/tasks_postgresql.h"
#include "common/lib/database_switch.h"
#include "cppmoreexample_tasks.h"

using namespace Appcppmoreexample;

TaskDbInit::TaskDbInit(IDatabase * database)
	: initAlphabet(database, "alphabet")
{
    initAlphabet.AddColumn("id", "BIGSERIAL PRIMARY KEY NOT NULL");
    initAlphabet.AddColumn("letter", "VARCHAR(32) UNIQUE NOT NULL");
}

TaskDbInit::~TaskDbInit()
{
}

void TaskDbInit::Start(class UTask * user)
{
    this->user = user;
    initAlphabet.Start(this);
}

void TaskDbInit::TaskComplete(class ITask * const task)
{
    if (task == &initAlphabet) Complete();
    else ASSERT(false, "TaskDbInit::TaskComplete");
}

void TaskDbInit::TaskFailed(class ITask * const task)
{
    Failed();
}