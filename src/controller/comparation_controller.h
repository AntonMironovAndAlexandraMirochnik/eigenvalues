#ifndef COMPARATION_CONTROLLER_H
#define COMPARATION_CONTROLLER_H

#include <QtGui>
#include "model/tasks/eigenvalues_task.h"

namespace Ui
{
    class ComparationWidget;
}

class ComparationController;

struct GeneratedTasks
{
    qreal accuracy;
    int dimention;
    EigenvaluesTask *qrTask;
    EigenvaluesTask *jacobiTask;
    void generate();
};

class ComparationController : public QObject
{
    Q_OBJECT
private:
    QPointer<QWidget> _widget;
    Ui::ComparationWidget *_ui;
    QVector<Matrix *> matrixes;

    void setupWidget();

    int _numberOfMatrixes;
    int _matrixDimention;
    int _computationAccuracy;

    qreal jacobiComputationsCompleted;
    qreal jacobiTotalComputionTime;
    qreal jacobiTotalIterationsNumber;
    qreal jacobiTotalResultAccuracy;

    qreal qrComputationsCompleted;
    qreal qrTotalComputionTime;
    qreal qrTotalIterationsNumber;
    qreal qrTotalResultAccuracy;

    QFutureWatcher<EigenvaluesResult> jacobiWatcher;
    QFutureWatcher<EigenvaluesResult> qrWatcher;
    QFutureWatcher<void> tasksWatcher;

    QList<GeneratedTasks> *tasks;
public:
    explicit ComparationController(QObject *parent = 0);
    virtual ~ComparationController();
    QWidget *widget() const;
    Ui::ComparationWidget *ui() const;

    int numberOfMatrixes() const {return _numberOfMatrixes;}
    int matrixDimention() const {return _matrixDimention;}
    int computationAccuracy() const {return _computationAccuracy;}

private slots:
    void compute();
    void tasksReady();
    void updateUi();

    void updateQRUi(int progress);
    void updateJacobiUi(int progress);

    void collectQRData(int index);
    void collectJacobiData(int index);
};

#endif // COMPARATION_CONTROLLER_H
