#ifndef EIGENVALUESALGORITHMCONTROLLER_H
#define EIGENVALUESALGORITHMCONTROLLER_H

#include <QtGui>
#include "model/tasks/eigenvalues_task.h"

namespace Ui
{
    class EigenvaluesWidget;
}

class EigenvaluesAlgorithmController : public QObject
{
    Q_OBJECT

private:
    QWidget *_widget;
    Ui::EigenvaluesWidget *_ui;
    QString computePushButtonReadyText;
    QString computePushButtonComputingText;

    void setupWidget();
    QFutureWatcher<EigenvaluesResult> futureWatcher;

protected:
    Ui::EigenvaluesWidget* ui()const;
    virtual EigenvaluesTask* generateTask(const Matrix &initialMatrix, int computationAccuracy);

public:
    explicit EigenvaluesAlgorithmController(QObject *parent = 0);
    virtual ~EigenvaluesAlgorithmController();

    QWidget* widget() const;
    virtual TaskType taskType() const;

private slots:
    void setupMatrix();
    void randomizeMatrix();
    void initialMatrixTableViewModelChanged();
    void finished();
    void updateUi();

protected slots:
    virtual void compute();

};

#endif // EIGENVALUESALGORITHMCONTROLLER_H
