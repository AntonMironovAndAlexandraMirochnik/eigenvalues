#ifndef MATRIXMODEL_H
#define MATRIXMODEL_H

#include <QtGui>

class Matrix;
class EditableMatrix;

class MatrixModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    Matrix *_matrix;
    const bool isEditable;
public:
    explicit MatrixModel(Matrix *matrix, QObject *parent = 0);
    explicit MatrixModel(EditableMatrix *matrix, QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    Q_PROPERTY (Matrix* matrix READ matrix)
    Matrix* matrix()const;
signals:

public slots:

};

#endif // MATRIXMODEL_H
