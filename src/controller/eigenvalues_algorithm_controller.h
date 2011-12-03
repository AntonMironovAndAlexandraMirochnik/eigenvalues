#ifndef EIGENVALUESALGORITHMCONTROLLER_H
#define EIGENVALUESALGORITHMCONTROLLER_H

#include <QtGui>
#include "model/tasks/eigenvalues_task.h"

namespace Ui
{
    class EigenvaluesWidget;
}

class EigenvaluesAlgorithmController : public QObject, public TaskDelegate
{
    Q_OBJECT
    Q_PROPERTY (bool processing READ isProcessing WRITE setProcessing)

private:
    QWidget *_widget;
    Ui::EigenvaluesWidget *_ui;
    QString computePushButtonReadyText;
    QString computePushButtonComputingText;
    bool _isProcessing;

    void setupWidget();

protected:
    Ui::EigenvaluesWidget* ui()const;
    virtual EigenvaluesTask* generateTask(const Matrix &initialMatrix, int computationAccuracy) = 0;

public:
    explicit EigenvaluesAlgorithmController(QObject *parent = 0);
    virtual ~EigenvaluesAlgorithmController();

    QWidget* widget() const;

    void taskStarted(Task *task);
    void taskFinished(Task *task);

    bool isProcessing() const;
    void setProcessing(bool isProcessing);

signals:
    void taskStartedSignal(EigenvaluesTask *task);
    void taskFinishedSignal(EigenvaluesTask *task);

private slots:
    void compute();
    void setupMatrix();
    void randomizeMatrix();

    void taskDidStart(EigenvaluesTask *task);
    void taskDidFinish(EigenvaluesTask *task);

    void initialMatrixTableViewModelChanged();
};

#endif // EIGENVALUESALGORITHMCONTROLLER_H
