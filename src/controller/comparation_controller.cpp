#include "comparation_controller.h"
#include "ui_comparation_widget.h"
#include "model/matrix/editable_matrix.h"

ComparationController::ComparationController(QObject *parent) :
    QObject(parent)
{
    setupWidget();

    connect(ui()->computePushButton, SIGNAL(pressed()), this, SLOT(compute()));
    connect(ui()->stopPushButton, SIGNAL(pressed()), &qrWatcher, SLOT(cancel()));
    connect(ui()->stopPushButton, SIGNAL(pressed()), &jacobiWatcher, SLOT(cancel()));

    connect(&qrWatcher, SIGNAL(progressRangeChanged(int,int)), ui()->qrProgressBar, SLOT(setRange(int,int)));
    connect(&jacobiWatcher, SIGNAL(progressRangeChanged(int,int)), ui()->jacobiProgressBar, SLOT(setRange(int,int)));

    connect(&qrWatcher, SIGNAL(progressValueChanged(int)), ui()->qrProgressBar, SLOT(setValue(int)));
    connect(&jacobiWatcher, SIGNAL(progressValueChanged(int)), ui()->jacobiProgressBar, SLOT(setValue(int)));

    connect(ui()->qrProgressBar, SIGNAL(valueChanged(int)), this, SLOT(updateQRUi(int)));
    connect(ui()->jacobiProgressBar, SIGNAL(valueChanged(int)), this, SLOT(updateJacobiUi(int)));

    connect(widget(), SIGNAL(destroyed()), &qrWatcher, SLOT(cancel()));
    connect(widget(), SIGNAL(destroyed()), &jacobiWatcher, SLOT(cancel()));
    connect(widget(), SIGNAL(destroyed()), &tasksWatcher, SLOT(cancel()));

    connect(&qrWatcher, SIGNAL(finished()), this , SLOT(updateUi()));
    connect(&jacobiWatcher, SIGNAL(finished()), this , SLOT(updateUi()));

    connect(&qrWatcher, SIGNAL(resultReadyAt(int)), this, SLOT(collectQRData(int)));
    connect(&jacobiWatcher, SIGNAL(resultReadyAt(int)), this, SLOT(collectJacobiData(int)));

    connect(&tasksWatcher, SIGNAL(finished()), this, SLOT(tasksReady()));
    connect(&tasksWatcher, SIGNAL(finished()), this, SLOT(updateUi()));
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

void ComparationController::setupWidget()
{
    {
        _widget = new QWidget();
        _ui = new Ui::ComparationWidget();
    }

    ui()->setupUi(widget());

    ui()->stopPushButton->setEnabled(false);
}

void ComparationController::compute()
{
    jacobiComputationsCompleted = 0.0;
    jacobiTotalComputionTime = 0.0;
    jacobiTotalIterationsNumber = 0.0;
    jacobiTotalResultAccuracy = 0.0;

    qrComputationsCompleted = 0.0;
    qrTotalComputionTime = 0.0;
    qrTotalIterationsNumber = 0.0;
    qrTotalResultAccuracy = 0.0;

    _numberOfMatrixes = ui()->numberOfMatrixesSpinBox->value();
    _matrixDimention = ui()->matrixDimentionSpinBox->value();
    _computationAccuracy = ui()->computationAccuracySpinBox->value();

    tasks = new QList<GeneratedTasks>();
    tasks->reserve(numberOfMatrixes());
    for (int i = 0; i < numberOfMatrixes(); ++i)
    {
        GeneratedTasks *task = new GeneratedTasks();
        task->accuracy = this->computationAccuracy();
        task->dimention = this->matrixDimention();
        task->jacobiTask = nullptr;
        task->qrTask = nullptr;
        *tasks << *task;
    }

    tasksWatcher.setFuture(QtConcurrent::map(*tasks, &GeneratedTasks::generate));
}

void ComparationController::tasksReady()
{
    QList<EigenvaluesTask> *qrTasks = new QList<EigenvaluesTask>();
    qrTasks->reserve(tasks->count());

    QList<EigenvaluesTask> *jacobiTasks = new QList<EigenvaluesTask>();
    jacobiTasks->reserve(tasks->count());

    for (QList<GeneratedTasks>::iterator i = tasks->begin(); i!= tasks->end(); ++i)
    {
        GeneratedTasks &task = *i;
        qrTasks->append(*task.qrTask);
        jacobiTasks->append(*task.jacobiTask);
    }
    delete tasks;
    tasks = nullptr;

    qrWatcher.setFuture(QtConcurrent::mapped(*qrTasks, &EigenvaluesTask::solve));
    jacobiWatcher.setFuture(QtConcurrent::mapped(*jacobiTasks, &EigenvaluesTask::solve));
}

void ComparationController::updateUi()
{
    bool isRunning = qrWatcher.isRunning() || jacobiWatcher.isRunning();

    ui()->computePushButton->setEnabled(!isRunning);
    ui()->stopPushButton->setEnabled(isRunning);
    ui()->computationAccuracySpinBox->setEnabled(!isRunning);
    ui()->matrixDimentionSpinBox->setEnabled(!isRunning);
    ui()->numberOfMatrixesSpinBox->setEnabled(!isRunning);
}

void ComparationController::updateQRUi(int progress)
{
    ui()->qrComputingTimeLineEdit->setText(tr("%1").arg((0 == progress) ? 0 : qrTotalComputionTime / (float)progress));
    ui()->qrResultAccuracyLineEdit->setText(tr("%1").arg((0 == progress) ? 0 : qrTotalResultAccuracy / (float)progress));
    ui()->qrIterationsNumberLineEdit->setText(tr("%1").arg((0 == progress) ? 0 : qrTotalIterationsNumber / (float)progress));
}

void ComparationController::updateJacobiUi(int progress)
{
    ui()->jacobiComputingTimeLineEdit->setText(tr("%1").arg((0 == progress) ? 0 : jacobiTotalComputionTime / (float)progress));
    ui()->jacobiResultAccuracyLineEdit->setText(tr("%1").arg((0 == progress) ? 0 : jacobiTotalResultAccuracy / (float)progress));
    ui()->jacobiIterationsNumberLineEdit->setText(tr("%1").arg((0 == progress) ? 0 : jacobiTotalIterationsNumber / (float)progress));
}

void ComparationController::collectQRData(int index)
{
    EigenvaluesResult result = qrWatcher.resultAt(index);
    qrTotalComputionTime += result.completionTime();
    qrTotalResultAccuracy += result.accuracy();
    qrTotalIterationsNumber += result.iterationsNumber();
}

void ComparationController::collectJacobiData(int index)
{
    EigenvaluesResult result = jacobiWatcher.resultAt(index);
    jacobiTotalComputionTime += result.completionTime();
    jacobiTotalResultAccuracy += result.accuracy();
    jacobiTotalIterationsNumber += result.iterationsNumber();
}

void GeneratedTasks::generate()
{
    Matrix* matrix = Matrix::MatrixSymmetricRandomized(dimention);
    qrTask = new EigenvaluesTask(*matrix, accuracy, QRTaskType);
    jacobiTask = new EigenvaluesTask(*matrix, accuracy, JacobiTaskType);
    delete matrix;
}
