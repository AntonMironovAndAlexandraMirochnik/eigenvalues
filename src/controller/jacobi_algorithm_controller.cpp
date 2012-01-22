#include "jacobi_algorithm_controller.h"
#include "model/tasks/eigenvalues_task.h"
#include "model/matrix_wrapper/matrix_model.h"
#include "model/matrix/editable_matrix.h"

JacobiAlgorithmController::JacobiAlgorithmController(QObject *parent)
    :EigenvaluesAlgorithmController(parent)
{
    widget()->setWindowTitle(tr("Jakobi algorithm"));
}

TaskType JacobiAlgorithmController::taskType() const
{
    return JacobiTaskType;
}
