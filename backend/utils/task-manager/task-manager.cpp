#include "task-manager.h"
#include <QPointer>


namespace GeneralUtils
{
    TaskManager::TaskManager(QObject *parent)
        : QObject{parent}
    {}

    TaskManager::~TaskManager()
    {
        for (auto &futWatcher : _activeTaskList)
        {
            futWatcher->future().waitForFinished();
            futWatcher->deleteLater();
        }

        _activeTaskList.clear();
    }

    void TaskManager::runAsync(const Priority &p, std::function<void()> &&function)
    {
        QMutexLocker locker(&_taskAccessMutex);

        std::function<void()> wrapper = [this, func = std::forward<std::function<void()>>(function)]() mutable {
            auto task = QtConcurrent::run(std::move(func));

            _activeTaskCount++;
            _addTaskToWatchList(task);
        };

        if (_activeTaskCount < _maxConcurrentTaskCount)
        {
            locker.unlock();
            wrapper();
        }
        else
        {
            _taskQueue[p].enqueue(wrapper);
        }
    }

    void TaskManager::_addTaskToWatchList(QFuture<void> task)
    {
        QMutexLocker locker(&_taskAccessMutex);

        QFutureWatcher<void>* watcher = new QFutureWatcher<void>(this);

        connect(watcher, &QFutureWatcher<void>::finished, [taskManager = QPointer<TaskManager>(this), watcher]()
        {
            if (!taskManager)
            {
                watcher->deleteLater();

                return;
            }

            QMutexLocker locker(&taskManager->_taskAccessMutex);

            taskManager->_activeTaskList.removeAll(watcher);
            watcher->deleteLater();
            --taskManager->_activeTaskCount;

            locker.unlock();

            taskManager->_tryStartNext();
        });

        watcher->setFuture(task);
        _activeTaskList.append(watcher);
    }

    void TaskManager::_tryStartNext()
    {
        QMutexLocker locker(&_taskAccessMutex);

        for (Priority p : {Priority::High, Priority::Normal, Priority::Low})
        {
            if (_activeTaskCount < _maxConcurrentTaskCount && !_taskQueue[p].isEmpty())
            {
                auto fn = _taskQueue[p].dequeue();
                locker.unlock();
                fn();

                return;
            }
        }
    }
}
