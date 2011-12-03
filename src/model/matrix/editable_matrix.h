#ifndef EDITABLEMATRIX_H
#define EDITABLEMATRIX_H

#include "matrix.h"

class EditableMatrix : public Matrix
{
protected:
    QVector<Index> rowOrder;
    QVector<Index> colOrder;
protected:
    void setupDefaultRowOrder();
    void setupDefaultColOrder();
public:
    EditableMatrix(const EditableMatrix& matrixToCopy);
    EditableMatrix(const QString &textData);
    EditableMatrix(const Matrix& matrixToCopy);
    EditableMatrix(const Matrix& matrixToCopy, Index M, Index N);
    EditableMatrix(Index N);
    EditableMatrix(Index N, const MatrixElement &value);
    EditableMatrix(Index M, Index N);
    EditableMatrix(Index M, Index N, const MatrixElement &value);
    EditableMatrix();

    Matrix matrix() const;
    Matrix matrixWithUnswapedElements() const;
    Matrix matrixWithUnswapedRows() const;
    Matrix matrixWithUnswapedCols() const;

    MatrixElement element(Index i, Index j) const;
    MatrixElement& element(Index i, Index j);
    void setElement(Index i, Index j, MatrixElement value);
    void randomizeValues(qreal minValue = 0, qreal maxValue = 1);

    void swapRows(Index rowOne, Index rowTwo);
    void swapCols(Index colOne, Index colTwo);

    virtual QString toString() const;
};

#endif // EDITABLEMATRIX_H
