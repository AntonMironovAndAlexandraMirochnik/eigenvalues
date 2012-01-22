#ifndef EIGENVALUES_TASK_H
#define EIGENVALUES_TASK_H

#include "model/matrix/matrix.h"

#define HUGE_VALUE 1000000.0

enum
{
InvalidTaskType = 0,
JacobiTaskType,
QRTaskType
};

typedef int TaskType;

class EigenvaluesResult
{
private:
    Matrix _matrix;
    volatile int _iterationsNumber;
    volatile qreal _accuracy;
    volatile int _completionTime;
    volatile bool _isValid;

public:
    EigenvaluesResult();
    EigenvaluesResult(const Matrix &eigenvalues, int iterationsNumber, qreal resultAccuracy, int completionTime);

    const Matrix& matrix() const;
    int iterationsNumber() const;
    qreal accuracy() const;
    int completionTime() const;
    bool isValid() const;
};

class EigenvaluesTask
{
private:
    Matrix _matrix;
    volatile int _accuracy;
    volatile qreal _epsilon;
    volatile TaskType _taskType;

public:
    EigenvaluesTask(const Matrix &initialMatrix, int computationAccuracy, TaskType taskType_ = InvalidTaskType);

    TaskType taskType() const {return _taskType;}
    const Matrix& matrix() const;
    int accuracy() const;
    qreal epsilon() const;

    EigenvaluesResult solve() const;
};

EigenvaluesResult solveWithJacobi(const EigenvaluesTask &task);
EigenvaluesResult solveWithQR(const EigenvaluesTask &task);

#endif // EIGENVALUES_TASK_H
