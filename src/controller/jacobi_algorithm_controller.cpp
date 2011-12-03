#include "jacobi_algorithm_controller.h"
#include "model/tasks/jacobi_task.h"

JacobiAlgorithmController::JacobiAlgorithmController(QObject *parent)
    :EigenvaluesAlgorithmController(parent)
{
    widget()->setWindowTitle(tr("Jakobi algorithm"));
}

EigenvaluesTask* JacobiAlgorithmController::generateTask(const Matrix &initialMatrix, int computationAccuracy)
{
    return new JacobiTask(initialMatrix, computationAccuracy);
}

