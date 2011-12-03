#include "matrix_private.h"

MatrixPrivate::MatrixPrivate() :
    sizeM(0), sizeN(0), elementsCount(0),
    matrixFlags(MatrixFlagInit)
{}

MatrixPrivate::MatrixPrivate(Index M, Index N,
                  MatrixFlags flags,
                  QVector<MatrixElement> data) :
    sizeM(M), sizeN(N), elementsCount(N * M),
    matrixFlags(flags), elementsData(data)
{}

MatrixPrivate::MatrixPrivate(const MatrixPrivate& matrix):
    sizeM(matrix.sizeM), sizeN(matrix.sizeN),
    elementsCount(matrix.elementsCount), matrixFlags(matrix.matrixFlags),
    elementsData(matrix.elementsData)
{}

MatrixElement& MatrixPrivate::element(Index i, Index j)
{
    return elementsData[i * sizeN + j];
}

MatrixElement MatrixPrivate::element(Index i, Index j) const
{
    return elementsData[i * sizeN + j];
}

void MatrixPrivate::setElement(Index i, Index j, const MatrixElement &value)
{
    elementsData[i * sizeN + j] = value;
}

MatrixPrivate MatrixPrivate::EmptyMatrixPrivate(Index M, Index N)
{
    MatrixFlags flags = MatrixFlagInit | MatrixFlagIsValid;
    flags |= (M == N) ? MatrixFlagIsSqure : 0;
    return MatrixPrivate(M, N, flags, QVector<MatrixElement>(M * N));
}

MatrixPrivate MatrixPrivate::FilledMatrixPrivate(Index M, Index N, MatrixElement value)
{
    MatrixPrivate matrix = EmptyMatrixPrivate(M, N);
    for (Index i = 0; i < matrix.elementsCount; ++i)
    {
        matrix.elementsData[i] = value;
    }
    return matrix;
}

MatrixPrivate MatrixPrivate::IMatrixPrivate(Index N)
{
    MatrixPrivate matrix = FilledMatrixPrivate(N, N , 0);
    for (Index i = 0; i < matrix.sizeM; ++i)
    {
        matrix.setElement(i, i, 1);
    }
    return matrix;
}


MatrixPrivate MatrixPrivate::sum(const MatrixPrivate &A, const MatrixPrivate &B)
{
    MatrixPrivate matrix = EmptyMatrixPrivate(A.sizeM, A.sizeN);
    for (Index i = 0; i < matrix.elementsCount; ++i)
    {
        matrix.elementsData[i] = A.elementsData[i] + B.elementsData[i];
    }
    return matrix;
}

MatrixPrivate MatrixPrivate::dif(const MatrixPrivate &A, const MatrixPrivate &B)
{
    MatrixPrivate matrix = EmptyMatrixPrivate(A.sizeM, A.sizeN);
    for (Index i = 0; i < matrix.elementsCount; ++i)
    {
        matrix.elementsData[i] = A.elementsData[i] - B.elementsData[i];
    }
    return matrix;
}

MatrixPrivate MatrixPrivate::mult(const MatrixPrivate &A, const MatrixPrivate &B)
{
    MatrixPrivate matrix = EmptyMatrixPrivate(A.sizeM, B.sizeN);
    MatrixElement sum;
    Index K = A.sizeN;

    for (Index i = 0; i < matrix.sizeM; ++i)
    {
        for (Index j = 0; j < matrix.sizeN; ++j)
        {
            sum = 0;
            for (Index k = 0; k < K; ++k)
            {
                sum += A.element(i, k) * B.element(k, j);
            }
            matrix.setElement(i, j, sum);
        }
    }
    return matrix;
}

MatrixPrivate MatrixPrivate::mult(const MatrixPrivate &A, const MatrixElement &value)
{
    MatrixPrivate matrix = EmptyMatrixPrivate(A.sizeM, A.sizeN);
    for (Index i = 0; i < matrix.elementsCount; ++i)
    {
        matrix.elementsData[i] = A.elementsData[i] * value;
    }
    return matrix;
}

MatrixPrivate MatrixPrivate::div(const MatrixPrivate &A, const MatrixElement &value)
{
    MatrixPrivate matrix = EmptyMatrixPrivate(A.sizeM, A.sizeN);
    for (Index i = 0; i < matrix.elementsCount; ++i)
    {
        matrix.elementsData[i] = A.elementsData[i] / value;
    }
    return matrix;
}
