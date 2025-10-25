#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <QObject>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrentRun>
#include <QMutex>
#include <QList>
#include <functional>
#include <QQueue>


namespace GeneralUtils
{
    /**
     * @brief Task priority levels for scheduling.
     */
    enum class Priority
    {
        High,
        Normal,
        Low
    };

    /**
     * @brief Asynchronous task manager with priorities and concurrency control
     *
     * Tasks are executed in a `QtConcurrent::run` call and monitored via `QFutureWatcher`.
     */
    class TaskManager : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Constructs a TaskManager instance.
         * @param parent - Optional parent QObject.
         */
        explicit TaskManager(QObject *parent = nullptr);

        /**
         * @brief Destructor, waits for all active tasks to finish.
         *
         * Ensures that all currently running tasks complete
         * before releasing resources.
         */
        ~TaskManager();

        /**
         * @brief Schedules a task for asynchronous execution.
         *
         * If there is an available execution slot, the task starts immediately.
         * Otherwise, it is placed into the queue corresponding to the given priority.
         *
         * @param p - Task priority.
         * @param function - The callable (lambda, functor, or std::function) with no parameters to execute.
         */
        void runAsync(const Priority &p, std::function<void()> &&function);

    private:
        /**
         * @brief Mutex to protect access to internal task queues and active task list.
         */
        QMutex _taskAccessMutex;

        /**
         * @brief List of currently active tasks being monitored.
         *
         * Stores QFutureWatcher pointers to track completion of running tasks.
         */
        QList<QFutureWatcher<void>*> _activeTaskList;

        /**
         * @brief Map of task queues, one per priority level.
         *
         * Keys are priorities; values are queues of functions awaiting execution.
         */
        QMap<Priority, QQueue<std::function<void()>>>_taskQueue;

        /**
         * @brief Maximum number of tasks allowed to run at the same time.
         *
         * Defaults to the number of ideal threads for the current system
         * as returned by `QThread::idealThreadCount()`.
         */
        int _maxConcurrentTaskCount = QThread::idealThreadCount();

        /**
         * @brief Current number of tasks actively running.
         */
        int _activeTaskCount = 0;

        /**
         * @brief Adds a task to the watch list and sets up completion handling.
         *
         * When the task finishes, it is removed from the active list,
         * resources are freed, and the next queued task (if any) is started.
         *
         * @param task - The QFuture representing the running task.
         */
        void _addTaskToWatchList(QFuture<void> task);

        /**
         * @brief Attempts to start the next available task from the queues.
         *
         * Checks the queues in the order: High → Normal → Low
         * and starts the first available task if an execution slot is free.
         */
        void _tryStartNext();
    };
}

#endif // TASK_MANAGER_H
