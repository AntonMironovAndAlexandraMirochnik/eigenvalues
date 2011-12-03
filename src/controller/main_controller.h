#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include <QtGui>

namespace Ui
{
    class MainWindow;
    class ComparationWidget;
};

class QRAlgorithmController;
class JacobiAlgorithmController;
class ComparationController;

class MainController : public QObject
{
    Q_OBJECT
private:
    QPointer<QMainWindow> _window;
    Ui::MainWindow *_ui;
    Ui::ComparationWidget *_uiCMP;

    QPointer<QRAlgorithmController> qrAlgorithmController;
    QPointer<JacobiAlgorithmController> jacobiAlgorithmController;
    QPointer<ComparationController> comparationController;

    void setupWindow();

protected:
    inline Ui::MainWindow *ui() const;
    inline Ui::ComparationWidget *uiCMP() const;

public:
    explicit MainController(QObject *parent = 0);
    virtual ~MainController();

    QMainWindow* window() const;

signals:

public slots:
};

#endif // MAIN_CONTROLLER_H
