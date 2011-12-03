#ifndef COMPARATION_CONTROLLER_H
#define COMPARATION_CONTROLLER_H

#include <QtGui>
#include "model/tasks/eigenvalues_task.h"

namespace Ui
{
    class ComparationWidget;
}

class ComparationController : public QObject, public TaskDelegate
{
    Q_OBJECT
private:
    QPointer<QWidget> _widget;
    Ui::ComparationWidget *_ui;
    QVector<Matrix *> matrixes;

    void setupWidget();
    void updateUi();

    int tasksRunningCount() const;
    bool isStopping;

    int numberOfMatrixes;
    int matrixDimention;
    int computationAccuracy;

    int jacobiTasksRunningCount;
    qreal jacobiComputationsCompleted;
    qreal jacobiTotalComputionTime;
    qreal jacobiTotalIterationsNumber;
    qreal jacobiTotalResultAccuracy;

    int qrTasksRunningCount;
    qreal qrComputationsCompleted;
    qreal qrTotalComputionTime;
    qreal qrTotalIterationsNumber;
    qreal qrTotalResultAccuracy;

public:
    explicit ComparationController(QObject *parent = 0);
    virtual ~ComparationController();
    QWidget *widget() const;
    Ui::ComparationWidget *ui() const;
    void taskStarted(Task *task);
    void taskFinished(Task *task);

signals:
    void taskStartedSignal(EigenvaluesTask *task);
    void taskFinishedSignal(EigenvaluesTask *task);

private slots:
    void compute();
    void stop();

    void taskDidStart(EigenvaluesTask *task);
    void taskDidFinish(EigenvaluesTask *task);

    void startTest();
};

#endif // COMPARATION_CONTROLLER_H
