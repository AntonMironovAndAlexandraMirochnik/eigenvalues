#ifndef QR_ALGORITHM_CONTROLLER_H
#define QR_ALGORITHM_CONTROLLER_H

#include "eigenvalues_algorithm_controller.h"

class QRAlgorithmController : public EigenvaluesAlgorithmController
{
    Q_OBJECT

public:
    explicit QRAlgorithmController(QObject *parent = 0);
    virtual EigenvaluesTask* generateTask(const Matrix &initialMatrix, int computationAccuracy);
};

#endif // QR_ALGORITHM_CONTROLLER_H
