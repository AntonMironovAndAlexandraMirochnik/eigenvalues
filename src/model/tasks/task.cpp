#include "task.h"

Task::Task(TaskType taskType) : QRunnable()
{
    setAutoDelete(false);
    _taskType = taskType;
    _isDone = false;
    _computionTime = 0;
    delegate = nullptr;
}

bool Task::isDone() const
{
    return _isDone;
}

Task::TaskType Task::taskType() const
{
    return _taskType;
}

int Task::elapsedTime()
{
    QMutexLocker locker(&mutex);
    return _startTime.elapsed();
}

void Task::startTask()
{
    mutex.lock();
    _startTime.start();
    mutex.unlock();
    if (nullptr != delegate)	delegate->taskStarted(this);
}

void Task::finishTask()
{
    mutex.lock();
    _computionTime = _startTime.elapsed();
    mutex.unlock();
    _isDone = true;
    if (nullptr != delegate)	delegate->taskFinished(this);
}

int Task::computationTime() const
{
    return _computionTime;
}

void Task::execute(TaskDelegate *delegate)
{
    if (nullptr == this->delegate)
    {
	this->delegate = delegate;
    }
    QThreadPool::globalInstance()->start(this, QThread::LowPriority);
}
