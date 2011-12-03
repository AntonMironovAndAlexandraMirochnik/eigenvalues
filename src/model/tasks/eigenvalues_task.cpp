#include "eigenvalues_task.h"

EigenvaluesTask::EigenvaluesTask(const Matrix &initialMatrix, int computationAccuracy, TaskType taskType)
    : Task(taskType), _initialMatrix(initialMatrix)
{
    _computationAccuracy = computationAccuracy;
    _epsilon = qPow(10, -_computationAccuracy);
}

Matrix& EigenvaluesTask::initialMatrix()
{
    QMutexLocker locker(&mutex);
    return _initialMatrix;
}

int EigenvaluesTask::computationAccuracy() const
{
    return _computationAccuracy;
}

qreal EigenvaluesTask::epsilon() const
{
    return _epsilon;
}

void EigenvaluesTask::done(const Matrix &eigenvalues, int iterationsNumber, qreal resultAccuracy)
{
    mutex.lock();
    _eigenvalues = eigenvalues;
    _iterationsNumber = iterationsNumber;
    _resultAccuracy = resultAccuracy;
    mutex.unlock();
    finishTask();
}

const Matrix& EigenvaluesTask::eigenvalues()
{
    QMutexLocker locker(&mutex);
    return _eigenvalues;
}

int EigenvaluesTask::iterationsNumber() const
{
    return _iterationsNumber;
}

qreal EigenvaluesTask::resultAccuracy() const
{
    return _resultAccuracy;
}

