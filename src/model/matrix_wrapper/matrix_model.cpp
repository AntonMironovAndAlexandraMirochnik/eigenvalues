#include "matrix_model.h"
#include "model/matrix/editable_matrix.h"
#include <QtGui>

MatrixModel::MatrixModel(Matrix *matrix, QObject *parent) :
    QAbstractTableModel(parent), isEditable(false)
{
    this->_matrix = matrix;
}

MatrixModel::MatrixModel(EditableMatrix *matrix, QObject *parent) :
    QAbstractTableModel(parent), isEditable(true)
{
    this->_matrix = matrix;
}

Matrix* MatrixModel::matrix()const
{
    return this->_matrix;
}

int MatrixModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return this->matrix()->M();
}

int MatrixModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return this->matrix()->N();
}

QVariant MatrixModel::data(const QModelIndex &index, int role) const
{
    if (Qt::DisplayRole == role || Qt::EditRole == role)
    {
	return QVariant((double)this->matrix()->element(index.row(), index.column()));
    }
    else
    {
	return QVariant();
    }
}

bool MatrixModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index);
    if (Qt::DisplayRole == role || Qt::EditRole == role)
    {
	((EditableMatrix *)this->matrix())->setElement(index.row(), index.column(), value.value<double>());
	return true;
    }
    else
    {
	return false;
    }
}

Qt::ItemFlags MatrixModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    if (isEditable)
    {
	return Qt::ItemIsEditable | Qt::ItemIsEnabled;
    }
    else
    {
	return Qt::ItemIsEnabled;
    }
}
