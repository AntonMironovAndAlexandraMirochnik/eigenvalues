#ifndef MATRIX_H
#define MATRIX_H

#include "model/matrix/matrix_globals.h"
#include "model/matrix/matrix_private.h"

class QString;

class Matrix
{
    friend class EditableMatrix;
protected:
    MatrixPrivate matrixPrivate;
    Matrix(const MatrixPrivate &privateData);
    void setElement(Index i, Index j, MatrixElement value);
public:
    Matrix();
    Matrix(const QString &textData);
    Matrix(const Matrix& matrixToCopy);
    Matrix(const Matrix& matrixToCopy, int M, int N);
    Matrix(Index N);
    Matrix(Index N, const MatrixElement &value);
    Matrix(Index M, Index N);
    Matrix(Index M, Index N, const MatrixElement &value);

    static Matrix* MatrixFromFile(QString fileName);
    static Matrix* MatrixRandomized(int M, int N, qreal minValue = 0, qreal maxValue = 1);

    Index M() const;
    Index N() const;

    MatrixElement element(Index i, Index j) const;

    Matrix transposed() const;
    Matrix inverted() const;

    bool isValid() const;
    bool isSquare() const;

    static void solveLinearByGauss(Matrix &A, Matrix &B);

    virtual QString toString() const;
    virtual void saveToFile(QString fileName) const;

    friend Matrix operator *(const Matrix& A, const Matrix& B);
    friend Matrix operator -(const Matrix& A, const Matrix& B);
};

#endif // MATRIX_H
