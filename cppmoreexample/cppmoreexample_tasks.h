/*-----------------------------------------------------------------------------------------------*/
/* Based on innovaphone App template                                                             */
/* copyright (c) innovaphone 2018                                                                */
/*                                                                                               */
/*-----------------------------------------------------------------------------------------------*/

namespace Appcppmoreexample {

    class TaskDbInit : public ITask, public UTask {
        class TaskPostgreSQLInitTable initAlphabet;

        void TaskComplete(class ITask * const task) override;
        void TaskFailed(class ITask * const task) override;

    public:
        TaskDbInit(IDatabase * database);
        virtual ~TaskDbInit();

        void Start(class UTask * user) override;
    };
}
