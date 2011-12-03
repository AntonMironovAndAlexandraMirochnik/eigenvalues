#include "matrix.h"
#include "editable_matrix.h"
#include "matrix_private.h"

EditableMatrix::EditableMatrix(const EditableMatrix& matrixToCopy)
    : Matrix(matrixToCopy)
{
    rowOrder = QVector<Index>(matrixToCopy.rowOrder);
    colOrder = QVector<Index>(matrixToCopy.colOrder);
}

EditableMatrix::EditableMatrix(const QString &textData)
    : Matrix(textData)
{
    setupDefaultRowOrder();
    setupDefaultColOrder();
}

EditableMatrix::EditableMatrix(const Matrix& matrixToCopy)
    : Matrix(matrixToCopy)
{
    setupDefaultRowOrder();
    setupDefaultColOrder();
}

EditableMatrix::EditableMatrix(const Matrix& matrixToCopy, Index M, Index N)
    : Matrix(matrixToCopy, M, N)
{
    setupDefaultRowOrder();
    setupDefaultColOrder();
}

EditableMatrix::EditableMatrix(Index N) : Matrix(N)
{
    setupDefaultRowOrder();
    setupDefaultColOrder();
}

EditableMatrix::EditableMatrix(Index N, const MatrixElement &value)
    : Matrix(N, value)
{
    setupDefaultRowOrder();
    setupDefaultColOrder();
}

EditableMatrix::EditableMatrix(Index M, Index N) : Matrix(M, N)
{
    setupDefaultRowOrder();
    setupDefaultColOrder();
}

EditableMatrix::EditableMatrix(Index M, Index N, const MatrixElement &value)
    : Matrix(M, N, value)
{
    setupDefaultRowOrder();
    setupDefaultColOrder();
}

EditableMatrix::EditableMatrix() : Matrix()
{
}

void EditableMatrix::setupDefaultRowOrder()
{
    rowOrder = QVector<Index>(M());
    for (Index i = 0; i < M(); ++i)
    {
	rowOrder[i] = i;
    }
}

void EditableMatrix::setupDefaultColOrder()
{
    colOrder = QVector<Index>(N());
    for (Index i = 0; i < N(); ++i)
    {
	colOrder[i] = i;
    }
}

Matrix EditableMatrix::matrix() const
{
    Matrix matrix(M(), N());
    Index i, j;
    for (i = 0; i < M(); ++i)
    {
	for (j = 0; j < N(); ++j)
	{
	    matrix.setElement(i, j, element(i, j));
	}
    }
    return matrix;
}

Matrix EditableMatrix::matrixWithUnswapedElements() const
{
    return Matrix(matrixPrivate);
}

Matrix EditableMatrix::matrixWithUnswapedRows() const
{
    Matrix matrix(M(), N());
    Index i, j;
    for (i = 0; i < M(); ++i)
    {
	for (j = 0; j < N(); ++j)
	{
	    matrix.setElement(i, colOrder[j], element(i, j));
	}
    }
    return matrix;
}

Matrix EditableMatrix::matrixWithUnswapedCols() const
{
    Matrix matrix(M(), N());
    Index i, j;
    for (i = 0; i < M(); ++i)
    {
	for (j = 0; j < N(); ++j)
	{
	    matrix.setElement(rowOrder[i], j, element(i, j));
	}
    }
    return matrix;
}

MatrixElement& EditableMatrix::element(Index i, Index j)
{
    return matrixPrivate.element(rowOrder[i], colOrder[j]);
}

void EditableMatrix::setElement(Index i, Index j, MatrixElement value)
{
    matrixPrivate.setElement(rowOrder[i], colOrder[j], value);
}

void EditableMatrix::randomizeValues(qreal minValue, qreal maxValue)
{
    Index i, j;
    for (i = 0; i < M(); ++i)
    {
        for (j = 0; j < N(); ++j)
        {
            element(i, j) = minValue + ((qreal)qrand() / (qreal)RAND_MAX) * (maxValue - minValue);
        }
    }
}

void EditableMatrix::randomizeSymmetricValues(qreal minValue, qreal maxValue)
{
    Index i, j;
    qreal value;
    for (i = 0; i < M(); ++i)
    {
        for (j = 0; j < N(); ++j)
        {
            value = minValue + ((qreal)qrand() / (qreal)RAND_MAX) * (maxValue - minValue);
            element(i, j) = value;
            element(j, i) = value;
        }
    }
}

MatrixElement EditableMatrix::element(Index i, Index j) const
{
    return matrixPrivate.element(rowOrder[i], colOrder[j]);
}

void EditableMatrix::swapRows(Index rowOne, Index rowTwo)
{
    if (rowOne == rowTwo) return;
    Index T = rowOrder[rowOne];
    rowOrder[rowOne] = rowOrder[rowTwo];
    rowOrder[rowTwo] = T;
}

void EditableMatrix::swapCols(Index colOne, Index colTwo)
{
    if (colOne == colTwo) return;
    Index T = colOrder[colOne];
    colOrder[colOne] = colOrder[colTwo];
    colOrder[colTwo] = T;
}

QString EditableMatrix::toString() const
{
    QString result;
    Index i, j;
    QString numberTemplate("\t%1");
#define addNumber(number) result += numberTemplate.arg(number, 0, 'f', 2, QChar())

    addNumber(M());
    addNumber(N());
    result += "\n";

    for (i = 0; i < M(); ++i)
    {
	for (j = 0; j < N(); ++j)
	{
	    addNumber(element(i, j));
	}
	result += "\n";
    }
#undef addNumber
    return result;
}
