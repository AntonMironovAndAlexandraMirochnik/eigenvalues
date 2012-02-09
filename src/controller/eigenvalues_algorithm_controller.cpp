#include "eigenvalues_algorithm_controller.h"
#include "ui_eigenvalues_widget.h"
#include "model/matrix_wrapper/matrix_table_view.h"
#include "model/matrix_wrapper/matrix_model.h"
#include "model/matrix/editable_matrix.h"

EigenvaluesAlgorithmController::EigenvaluesAlgorithmController(QObject *parent) :
    QObject(parent)
{
    computePushButtonReadyText = tr("Compute");
    computePushButtonComputingText = tr("Computing...");
    setupWidget();
    connect(&futureWatcher, SIGNAL(finished()), this, SLOT(finished()));
    connect(&futureWatcher, SIGNAL(finished()), this, SLOT(updateUi()));
    connect(&futureWatcher, SIGNAL(canceled()), this, SLOT(updateUi()));

    connect(ui()->computePushButton, SIGNAL(pressed()), this, SLOT(compute()));
    connect(ui()->setupMatrixPushButton, SIGNAL(pressed()), this, SLOT(setupMatrix()));
    connect(ui()->randomizeMatrixPushButton, SIGNAL(pressed()), this, SLOT(randomizeMatrix()));
    connect(ui()->initialMatrixTableView, SIGNAL(modelChanged()), this, SLOT(initialMatrixTableViewModelChanged()));
}

EigenvaluesAlgorithmController::~EigenvaluesAlgorithmController()
{
    _widget->deleteLater();
    delete _ui;
}

Ui::EigenvaluesWidget* EigenvaluesAlgorithmController::ui()const
{
    return _ui;
}

QWidget* EigenvaluesAlgorithmController::widget() const
{
    return _widget;
}

void EigenvaluesAlgorithmController::setupWidget()
{
    {
        _widget = new QWidget();
        _ui = new Ui::EigenvaluesWidget();
    }

    ui()->setupUi(widget());
    ui()->computePushButton->setEnabled(false);
    ui()->computePushButton->setText(computePushButtonReadyText);
    ui()->initialMatrixTableView->setOpeningEnabled(true);
    ui()->initialMatrixTableView->setSaveingEnabled(true);
    ui()->resultMatrixTableView->setSaveingEnabled(true);
}

void EigenvaluesAlgorithmController::compute()
{
    MatrixModel* model = (MatrixModel*) ui()->initialMatrixTableView->model();
    EigenvaluesTask* task = generateTask(*(model->matrix()), ui()->calculationAccuracySpinBox->value());

    futureWatcher.setFuture(QtConcurrent::run<EigenvaluesResult>(task, &EigenvaluesTask::solve));
}

void EigenvaluesAlgorithmController::setupMatrix()
{
    MatrixModel *oldModel = (MatrixModel *)ui()->initialMatrixTableView->model();
    MatrixModel *model = nullptr;
    int N = ui()->matrixDimentionSpinBox->value();

    if (nullptr == oldModel)
    {
	model = new MatrixModel(new EditableMatrix(N, N));
    }
    else
    {
	model = new MatrixModel(new EditableMatrix(*oldModel->matrix(), N, N));
	oldModel->deleteLater();
    }

    ui()->initialMatrixTableView->setModel(model);
}

void EigenvaluesAlgorithmController::randomizeMatrix()
{
    MatrixModel *oldModel = (MatrixModel *)ui()->initialMatrixTableView->model();
    MatrixModel *model = nullptr;
    int N = ui()->matrixDimentionSpinBox->value();

    if (nullptr == oldModel)
    {
	EditableMatrix *matrix = new EditableMatrix(N, N);
        matrix->randomizeSymmetricValues();
	model = new MatrixModel(matrix);
    }
    else
    {
	EditableMatrix *matrix = new EditableMatrix(*oldModel->matrix(), N, N);
    matrix->randomizeSymmetricValues();
	model = new MatrixModel(matrix);
	oldModel->deleteLater();
    }

    ui()->initialMatrixTableView->setModel(model);
}

void EigenvaluesAlgorithmController::initialMatrixTableViewModelChanged()
{
    ui()->computePushButton->setEnabled(true);
}

void EigenvaluesAlgorithmController::finished()
{
    EigenvaluesResult result = futureWatcher.result();
    ui()->resultAccuracy->setText(tr("%1").arg(result.accuracy()));
    ui()->computingTimeLineEdit->setText(tr("%1").arg(result.completionTime()));
    ui()->iterationsNumberLineEdit->setText(tr("%1").arg(result.iterationsNumber()));

    MatrixModel *resultModel = new MatrixModel(new Matrix(result.matrix()), ui()->resultMatrixTableView);
	ui()->resultMatrixTableView->setModel(resultModel);
}

void EigenvaluesAlgorithmController::updateUi()
{
    bool isProcessing = futureWatcher.isRunning();

    ui()->initialMatrixTableView->setEnabled(!isProcessing);
    ui()->resultMatrixTableView->setEnabled(!isProcessing);
    ui()->matrixDimentionSpinBox->setEnabled(!isProcessing);
    ui()->calculationAccuracySpinBox->setEnabled(!isProcessing);
    ui()->setupMatrixPushButton->setEnabled(!isProcessing);
    ui()->randomizeMatrixPushButton->setEnabled(!isProcessing);

    ui()->computePushButton->setEnabled(!isProcessing);
    if (isProcessing)
    {
        ui()->computePushButton->setText(computePushButtonComputingText);
        ui()->resultAccuracy->setText(tr("Computing..."));
        ui()->computingTimeLineEdit->setText(tr("Computing..."));
        ui()->iterationsNumberLineEdit->setText(tr("Computing..."));
    }
    else
    {
        ui()->computePushButton->setText(computePushButtonReadyText);
    }
}

TaskType EigenvaluesAlgorithmController::taskType() const
{
    return InvalidTaskType;
}

EigenvaluesTask* EigenvaluesAlgorithmController::generateTask(const Matrix &initialMatrix, int computationAccuracy)
{
    return new EigenvaluesTask(initialMatrix, computationAccuracy, taskType());
}
