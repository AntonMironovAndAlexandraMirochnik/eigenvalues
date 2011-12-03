#include "main_controller.h"
#include "ui_main_window.h"
#include "ui_eigenvalues_widget.h"
#include "ui_comparation_widget.h"

#include "qr_algorithm_controller.h"
#include "jacobi_algorithm_controller.h"
#include "comparation_controller.h"

MainController::MainController(QObject *parent) :
    QObject(parent)
{
    setupWindow();
}

MainController::~MainController()
{
    _window->deleteLater();
    qrAlgorithmController->deleteLater();
    jacobiAlgorithmController->deleteLater();
    comparationController->deleteLater();

    delete _ui;
    delete _uiCMP;
}

QMainWindow* MainController::window() const
{
    return _window.data();
}

inline Ui::MainWindow* MainController::ui() const
{
    return _ui;
}

inline Ui::ComparationWidget* MainController::uiCMP() const
{
    return _uiCMP;
}

void MainController::setupWindow()
{
    {
	_window = QPointer<QMainWindow>(new QMainWindow());
	_ui = new Ui::MainWindow();
	_uiCMP = new Ui::ComparationWidget();
    }

    {
	qrAlgorithmController = new QRAlgorithmController(this);
	jacobiAlgorithmController = new JacobiAlgorithmController(this);
	comparationController = new ComparationController(this);
    }

    {
	ui()->setupUi(window());
	ui()->tabWidget->addTab(jacobiAlgorithmController->widget(), jacobiAlgorithmController->widget()->windowTitle());
	ui()->tabWidget->addTab(qrAlgorithmController->widget(), qrAlgorithmController->widget()->windowTitle());
	ui()->tabWidget->addTab(comparationController->widget(), comparationController->widget()->windowTitle());

	//Comparation
    }

}
