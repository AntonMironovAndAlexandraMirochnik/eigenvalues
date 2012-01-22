#include "qr_algorithm_controller.h"
#include "model/tasks/eigenvalues_task.h"

QRAlgorithmController::QRAlgorithmController(QObject *parent)
    :EigenvaluesAlgorithmController(parent)
{
    widget()->setWindowTitle(tr("QR-algorithm"));
}

TaskType QRAlgorithmController::taskType() const
{
    return QRTaskType;
}

