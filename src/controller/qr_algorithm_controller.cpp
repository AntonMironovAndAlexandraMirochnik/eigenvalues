#include "qr_algorithm_controller.h"
#include "model/tasks/qr_task.h"

QRAlgorithmController::QRAlgorithmController(QObject *parent)
    :EigenvaluesAlgorithmController(parent)
{
    widget()->setWindowTitle(tr("QR-algorithm"));
}

EigenvaluesTask* QRAlgorithmController::generateTask(const Matrix &initialMatrix, int computationAccuracy)
{
    return new QRTask(initialMatrix, computationAccuracy);
}

