#ifndef JACOBI_TASK_H
#define JACOBI_TASK_H

#include "eigenvalues_task.h"

class JacobiTask : public EigenvaluesTask
{
protected:
    virtual void run();

public:
    JacobiTask(const Matrix &initialMatrix, int computationAccuracy);
};

#endif // JACOBI_TASK_H
