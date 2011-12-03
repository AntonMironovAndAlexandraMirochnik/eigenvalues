#include "comparation_controller.h"
#include "ui_comparation_widget.h"
#include "model/matrix/editable_matrix.h"
#include "model/tasks/qr_task.h"
#include "model/tasks/jacobi_task.h"

ComparationController::ComparationController(QObject *parent) :
    QObject(parent)
{
    setupWidget();
}

ComparationController::~ComparationController()
{
    _widget->deleteLater();
    delete _ui;
}

QWidget* ComparationController::widget() const
{
    return _widget.data();
}

Ui::ComparationWidget* ComparationController::ui() const
{
    return _ui;
}

int ComparationController::tasksRunningCount() const
{
    return jacobiTasksRunningCount + qrTasksRunningCount;
}

void ComparationController::setupWidget()
{
    {
	_widget = new QWidget();
	_ui = new Ui::ComparationWidget();
    }

    ui()->setupUi(widget());

    ui()->stopPushButton->setEnabled(false);

    connect(ui()->computePushButton, SIGNAL(pressed()), this, SLOT(compute()));
    connect(ui()->stopPushButton, SIGNAL(pressed()), this, SLOT(stop()));
    connect(this, SIGNAL(taskStartedSignal(EigenvaluesTask*)), this, SLOT(taskDidStart(EigenvaluesTask*)));
    connect(this, SIGNAL(taskFinishedSignal(EigenvaluesTask*)), this, SLOT(taskDidFinish(EigenvaluesTask*)));
}

void ComparationController::compute()
{
    isStopping = false;

    jacobiTasksRunningCount = 0;
    jacobiComputationsCompleted = 0.0;
    jacobiTotalComputionTime = 0.0;
    jacobiTotalIterationsNumber = 0.0;
    jacobiTotalResultAccuracy = 0.0;

    qrTasksRunningCount = 0;
    qrComputationsCompleted = 0.0;
    qrTotalComputionTime = 0.0;
    qrTotalIterationsNumber = 0.0;
    qrTotalResultAccuracy = 0.0;

    numberOfMatrixes = ui()->numberOfMatrixesSpinBox->value();
    matrixDimention = ui()->matrixDimentionSpinBox->value();
    computationAccuracy = ui()->computationAccuracySpinBox->value();

    matrixes = QVector<Matrix *>(numberOfMatrixes, nullptr);

    startTest();
    updateUi();
}

void ComparationController::stop()
{
    isStopping = true;
    ui()->stopPushButton->setEnabled(!isStopping);
}

void ComparationController::taskStarted(Task *task)
{
    emit taskStartedSignal((EigenvaluesTask *)task);
}

void ComparationController::taskFinished(Task *task)
{
    emit taskFinishedSignal((EigenvaluesTask *)task);
}

void ComparationController::taskDidStart(EigenvaluesTask *task)
{
}

void ComparationController::taskDidFinish(EigenvaluesTask *task)
{
    if (task->isDone())
    {
	switch (task->taskType())
	{
	case Task::JacobiTaskType:
	    --jacobiTasksRunningCount;
	    ++jacobiComputationsCompleted;
	    jacobiTotalComputionTime += task->computationTime();
	    jacobiTotalIterationsNumber += task->iterationsNumber();
	    jacobiTotalResultAccuracy +=task->resultAccuracy();
	    break;
	case Task::QRTaskType:
	    --qrTasksRunningCount;
	    ++qrComputationsCompleted;
	    qrTotalComputionTime += task->computationTime();
	    qrTotalIterationsNumber += task->iterationsNumber();
	    qrTotalResultAccuracy +=task->resultAccuracy();
	    break;
	}
    }
    else
    {
	isStopping = true;
	QMessageBox::warning(widget(), tr("Error"), tr("Solution is not found"));
    }

    delete task;
    isStopping |= (jacobiComputationsCompleted == numberOfMatrixes) && (qrComputationsCompleted == numberOfMatrixes);
    startTest();
    updateUi();
}

void ComparationController::updateUi()
{
    bool isRunning = (tasksRunningCount() > 0);

    ui()->computePushButton->setEnabled(!isRunning);
    ui()->stopPushButton->setEnabled(!isStopping);
    ui()->computationAccuracySpinBox->setEnabled(!isRunning);
    ui()->matrixDimentionSpinBox->setEnabled(!isRunning);
    ui()->numberOfMatrixesSpinBox->setEnabled(!isRunning);

    ui()->jacobiProgressBar->setMaximum(numberOfMatrixes);
    ui()->jacobiProgressBar->setValue(jacobiComputationsCompleted);
    ui()->jacobiComputingTimeLineEdit->setText(tr("%1").arg((0 == jacobiComputationsCompleted) ? 0 : jacobiTotalComputionTime / jacobiComputationsCompleted));
    ui()->jacobiResultAccuracyLineEdit->setText(tr("%1").arg((0 == jacobiComputationsCompleted) ? 0 : jacobiTotalResultAccuracy / jacobiComputationsCompleted));
    ui()->jacobiIterationsNumberLineEdit->setText(tr("%1").arg((0 == jacobiComputationsCompleted) ? 0 : jacobiTotalIterationsNumber / jacobiComputationsCompleted));

    ui()->qrProgressBar->setMaximum(numberOfMatrixes);
    ui()->qrProgressBar->setValue(qrComputationsCompleted);
    ui()->qrComputingTimeLineEdit->setText(tr("%1").arg((0 == qrComputationsCompleted) ? 0 : qrTotalComputionTime / qrComputationsCompleted));
    ui()->qrResultAccuracyLineEdit->setText(tr("%1").arg((0 == qrComputationsCompleted) ? 0 : qrTotalResultAccuracy / qrComputationsCompleted));
    ui()->qrIterationsNumberLineEdit->setText(tr("%1").arg((0 == qrComputationsCompleted) ? 0 : qrTotalIterationsNumber / qrComputationsCompleted));
}

void ComparationController::startTest()
{
    if (isStopping)
    {
	//cleanup
	int matrixIndex = (jacobiComputationsCompleted < qrComputationsCompleted) ? jacobiComputationsCompleted : qrComputationsCompleted;
	while (matrixIndex < matrixes.count() && nullptr != matrixes[matrixIndex])
	{
	    delete matrixes[matrixIndex];
	    matrixes[matrixIndex] = nullptr;
	}
	return;
    }

    while (QThreadPool::globalInstance()->activeThreadCount() < QThreadPool::globalInstance()->maxThreadCount())
    {
	EigenvaluesTask *task = nullptr;
	Matrix *matrix = nullptr;
	int matrixIndex = 0;

	matrixIndex = (jacobiComputationsCompleted < qrComputationsCompleted) ? jacobiComputationsCompleted : qrComputationsCompleted;

	if (nullptr == matrixes[matrixIndex])
	{
	    matrixes[matrixIndex] = Matrix::MatrixRandomized(matrixDimention, matrixDimention);
	}
	else
	{
	    int i = matrixIndex - 1;
	    while (i >= 0 && nullptr != matrixes[i])
	    {
		delete matrixes[i];
		matrixes[i] = nullptr;
	    }
	}

	matrix = matrixes[matrixIndex];

	if (jacobiComputationsCompleted < qrComputationsCompleted)
	{
	    ++jacobiTasksRunningCount;
	    task = new JacobiTask(*matrix, computationAccuracy);
	}
	else
	{
	    ++qrTasksRunningCount;
	    task = new QRTask(*matrix, computationAccuracy);
	}

	task->execute(this);
    }
}

