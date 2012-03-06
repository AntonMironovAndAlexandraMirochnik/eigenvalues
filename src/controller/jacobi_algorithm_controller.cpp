#include "jacobi_algorithm_controller.h"
#include "model/tasks/eigenvalues_task.h"
#include "model/matrix_wrapper/matrix_model.h"
#include "model/matrix/editable_matrix.h"
#include "ui_eigenvalues_widget.h"

JacobiAlgorithmController::JacobiAlgorithmController(QObject *parent)
    :EigenvaluesAlgorithmController(parent)
{
    widget()->setWindowTitle(tr("Jakobi algorithm"));
}

TaskType JacobiAlgorithmController::taskType() const
{
    return JacobiTaskType;
}

void JacobiAlgorithmController::compute()
{
    MatrixModel* model = (MatrixModel*) ui()->initialMatrixTableView->model();
    if (model->matrix()->isSymmetric())
    {
        EigenvaluesAlgorithmController::compute();
    }
    else
    {
        QMessageBox::warning(this->widget()->window(), tr("Can't perform calculation"), tr("Matrix is not symmetric"));
    }
}
