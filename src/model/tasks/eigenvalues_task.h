#ifndef EIGENVALUES_TASK_H
#define EIGENVALUES_TASK_H

#include "task.h"
#include "model/matrix/matrix.h"

#define HUGE_VALUE 1000000.0

class EigenvaluesTask : public Task
{
private:
    Matrix _initialMatrix;
    volatile int _computationAccuracy;
    volatile qreal _epsilon;

    Matrix _eigenvalues;
    volatile int _iterationsNumber;
    volatile qreal _resultAccuracy;

protected:
    EigenvaluesTask(const Matrix &initialMatrix, int computationAccuracy, TaskType taskType = InvalidTaskType);
    void done(const Matrix &eigenvalues, int iterationsNumber, qreal resultAccuracy);

public:
    //input
    Matrix& initialMatrix();
    int computationAccuracy() const;
    qreal epsilon() const;

    //output
    const Matrix& eigenvalues();
    int iterationsNumber() const;
    qreal resultAccuracy() const;
};

#endif // EIGENVALUES_TASK_H
