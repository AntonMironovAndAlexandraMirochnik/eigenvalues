#ifndef JACOBI_ALGORITHM_CONTROLLER_H
#define JACOBI_ALGORITHM_CONTROLLER_H

#include "eigenvalues_algorithm_controller.h"

class JacobiAlgorithmController : public EigenvaluesAlgorithmController
{
    Q_OBJECT

public:
    explicit JacobiAlgorithmController(QObject *parent = 0);
    virtual TaskType taskType() const;
public slots:
    virtual void compute();
};

#endif // JACOBI_ALGORITHM_CONTROLLER_H
