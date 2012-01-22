#include "eigenvalues_task.h"
EigenvaluesTask::EigenvaluesTask(const Matrix &initialMatrix, int computationAccuracy, TaskType taskType)
    : _matrix(initialMatrix), _accuracy(computationAccuracy), _taskType(taskType)
{
    _epsilon = qPow(10, -_accuracy);
}

const Matrix& EigenvaluesTask::matrix() const
{
    return _matrix;
}

int EigenvaluesTask::accuracy() const
{
    return _accuracy;
}

qreal EigenvaluesTask::epsilon() const
{
    return _epsilon;
}

EigenvaluesResult EigenvaluesTask::solve() const
{
    Q_ASSERT(taskType() != InvalidTaskType);
    switch (taskType())
    {
    case JacobiTaskType:
        return solveWithJacobi(*this);
    break;
    case QRTaskType:
        return solveWithQR(*this);
    break;
    }
    return EigenvaluesResult();
}

EigenvaluesResult::EigenvaluesResult() : _isValid(false)
{

}

EigenvaluesResult::EigenvaluesResult(const Matrix &eigenvalues, int iterationsNumber, qreal resultAccuracy, int completionTime) : _isValid(true)
{
    _matrix = eigenvalues;
    _iterationsNumber = iterationsNumber;
    _accuracy = resultAccuracy;
    _completionTime = completionTime;
}

const Matrix& EigenvaluesResult::matrix() const
{
    return _matrix;
}

int EigenvaluesResult::iterationsNumber() const
{
    return _iterationsNumber;
}

qreal EigenvaluesResult::accuracy() const
{
    return _accuracy;
}

int EigenvaluesResult::completionTime() const
{
    return _completionTime;
}

bool EigenvaluesResult::isValid() const
{
    return _isValid;
}
