#ifndef TASK_H
#define TASK_H

#include <QtCore>

class TaskDelegate;

class Task : public QRunnable
{
public:
    enum
    {
	InvalidTaskType = 0,
	JacobiTaskType,
	QRTaskType
    };

    typedef int TaskType;

private:
    volatile bool _isDone;
    QTime _startTime;
    volatile int _computionTime;
    volatile TaskType _taskType;

protected:
    QMutex mutex;

    virtual void startTask();
    virtual void finishTask();
    int elapsedTime();
    Task(TaskType taskType = InvalidTaskType);

public:
    TaskDelegate* delegate;

    int computationTime() const;

    void execute(TaskDelegate *delegate = nullptr);

    bool isDone() const;
    TaskType taskType() const;
};

class TaskDelegate
{
public:
    virtual void taskStarted(Task *task) = 0;
    virtual void taskFinished(Task *task) = 0;
};

#endif // TASK_H
