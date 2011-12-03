#ifndef JACOBI_ALGORITHM_CONTROLLER_H
#define JACOBI_ALGORITHM_CONTROLLER_H

#include "eigenvalues_algorithm_controller.h"

class JacobiAlgorithmController : public EigenvaluesAlgorithmController
{
    Q_OBJECT

public:
    explicit JacobiAlgorithmController(QObject *parent = 0);
    virtual EigenvaluesTask* generateTask(const Matrix &initialMatrix, int computationAccuracy);
};

#endif // JACOBI_ALGORITHM_CONTROLLER_H
