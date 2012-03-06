#include "matrix_table_view.h"
#include "model/matrix/editable_matrix.h"
#include "matrix_model.h"
#include "matrix_table_item_delegate.h"

MatrixTableView::MatrixTableView(QWidget *parent) :
    QTableView(parent)
{
    menu = new QMenu(this);
    openAction = menu->addAction(tr("Open from file..."));
    saveAction = menu->addAction(tr("Save to file..."));

    connect(openAction, SIGNAL(triggered()), this, SLOT(openFromFile()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveToFile()));

    setOpeningEnabled(false);
    setSaveingEnabled(true);
    setItemDelegate(new MatrixTableItemDelegate(this));
}

MatrixTableView::~MatrixTableView()
{
    menu->deleteLater();
}

void MatrixTableView::setModel(MatrixModel *model)
{
    QTableView::setModel(model);
    int M = model->matrix()->M();
    for (int i = 0; i < M; ++i)
    {
        setColumnWidth(i, rowHeight(i) + 20);
    }
    emit modelChanged();
}

MatrixModel* MatrixTableView::model()const
{
    return (MatrixModel*)QTableView::model();
}

bool MatrixTableView::isOpeningEnabled() const
{
    return openAction->isEnabled();
}
void MatrixTableView::setOpeningEnabled(bool isEnabled)
{
    openAction->setEnabled(isEnabled);
}

bool MatrixTableView::isSaveingEnabled() const
{
    return saveAction->isEnabled();
}

void MatrixTableView::setSaveingEnabled(bool isEnabled)
{
    saveAction->setEnabled(isEnabled);
}

void MatrixTableView::openFromFile()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
	 tr("Open Matrix"), QApplication::applicationDirPath(), tr("Text Files (*.txt)"));
    if (!fileName.isNull())
    {
	try
	{
	    QFile file(fileName);
	    if (!file.open(QFile::ReadOnly)) throw tr("Opening file failed");
	    QString data(file.readAll());
	    file.close();
	    setModel(new MatrixModel(new EditableMatrix(data)));
	}
	catch (QString msg)
	{
	    QMessageBox::warning(this, tr("File opening error"), tr("Failed to load matrix from file %1.").arg(fileName));
	}
    }
}

void MatrixTableView::saveToFile()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this,
	 tr("Save Matrix"), QApplication::applicationDirPath(), tr("Text Files (*.txt)"));
    if (!fileName.isNull())
    {
	try
	{
	    QFile file(fileName);
	    if (!file.open(QFile::WriteOnly)) throw tr("Saving file failed");
	    file.write(model()->matrix()->toString().toUtf8());
	    file.close();
	}
	catch (QString msg)
	{
	    QMessageBox::warning(this, tr("File saving error"), tr("Failed to save matrix to file %1.").arg(fileName));
	}
    }
}

void MatrixTableView::contextMenuEvent(QContextMenuEvent *event)
{
    if (isSaveingEnabled() || isOpeningEnabled())
    {
	menu->popup(event->globalPos());
	event->accept();
    }
}
