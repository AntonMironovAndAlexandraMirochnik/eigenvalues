#ifndef QR_TASK_H
#define QR_TASK_H

#include "eigenvalues_task.h"

class QRTask : public EigenvaluesTask
{
private:
    virtual void run();
public:
    QRTask(const Matrix &initialMatrix, int computationAccuracy);
};

#endif // QR_TASK_H
