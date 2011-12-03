#ifndef MATRIXTABLE_H
#define MATRIXTABLE_H

#include <QtGui>

class Matrix;
class MatrixModel;

class MatrixTableView : public QTableView
{
    Q_OBJECT
    Q_PROPERTY (bool openingEnabled READ isOpeningEnabled WRITE setOpeningEnabled)
    Q_PROPERTY (bool saveingEnabled READ isSaveingEnabled WRITE setSaveingEnabled)

private:
    QMenu *menu;
    QAction* openAction;
    QAction* saveAction;

protected:
    void contextMenuEvent(QContextMenuEvent *event);

public:
    explicit MatrixTableView(QWidget *parent = 0);
    virtual ~MatrixTableView();
    void setModel(MatrixModel *model);
    MatrixModel* model()const;

    bool isOpeningEnabled() const;
    void setOpeningEnabled(bool isEnabled);

    bool isSaveingEnabled() const;
    void setSaveingEnabled(bool isEnabled);

private slots:
    void openFromFile();
    void saveToFile();

};

#endif // MATRIXTABLE_H
