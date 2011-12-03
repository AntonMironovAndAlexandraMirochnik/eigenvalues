#ifndef MATRIX_PRIVATE_H
#define MATRIX_PRIVATE_H

#include "model/matrix/matrix_globals.h"

#include <QtCore>
enum
{
    MatrixFlagInit      = 0,
    MatrixFlagIsValid     = 1 << 0,
    MatrixFlagIsSqure     = 1 << 1
};

class MatrixPrivate
{
public:
    Index sizeM;
    Index sizeN;
    Index elementsCount;
    MatrixFlags matrixFlags;
    QVector<MatrixElement> elementsData;

    //constructors
    MatrixPrivate();
    MatrixPrivate(Index M, Index N,
                  MatrixFlags flags,
                  QVector<MatrixElement> data);
    MatrixPrivate(const MatrixPrivate& matrix);

    MatrixElement& element(Index i, Index j);
    MatrixElement element(Index i, Index j) const;
    void setElement(Index i, Index j, const MatrixElement &value);

    static MatrixPrivate EmptyMatrixPrivate(Index M, Index N);
    static MatrixPrivate FilledMatrixPrivate(Index M, Index N, MatrixElement value);
    static MatrixPrivate IMatrixPrivate(Index N);

    static MatrixPrivate sum(const MatrixPrivate &A, const MatrixPrivate &B);
    static MatrixPrivate dif(const MatrixPrivate &A, const MatrixPrivate &B);
    static MatrixPrivate mult(const MatrixPrivate &A, const MatrixPrivate &B);
    static MatrixPrivate mult(const MatrixPrivate &A, const MatrixElement &value);
    static MatrixPrivate div(const MatrixPrivate &A, const MatrixElement &value);
};

#endif // MATRIX_PRIVATE_H
