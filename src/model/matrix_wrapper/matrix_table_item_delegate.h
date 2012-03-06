#ifndef MATRIX_TABLE_ITEM_DELEGATE_H
#define MATRIX_TABLE_ITEM_DELEGATE_H

#include <QtGui>

class MatrixTableItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit MatrixTableItemDelegate(QObject *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:
    
public slots:
    
};

#endif // MATRIX_TABLE_ITEM_DELEGATE_H
