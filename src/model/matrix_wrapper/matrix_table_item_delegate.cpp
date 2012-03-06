#include "matrix_table_item_delegate.h"
#include "constants.h"

MatrixTableItemDelegate::MatrixTableItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget* MatrixTableItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = new QDoubleSpinBox(parent);
    spinBox->setDecimals(TableSpinBoxPrec);
    spinBox->setValue(index.data().toDouble());
    spinBox->setRange(TableSpinBoxMin, TableSpinBoxMax);
    spinBox->setGeometry(option.rect);
    return spinBox;
}

void MatrixTableItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox *>(editor);
    spinBox->setValue(index.data().toDouble());
}

void MatrixTableItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox *>(editor);
    model->setData(index, spinBox->value());
}

void MatrixTableItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox *>(editor);
    spinBox->setGeometry(option.rect);
}

