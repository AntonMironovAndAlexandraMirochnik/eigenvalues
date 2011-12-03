#include "matrix.h"
#include "editable_matrix.h"

void Matrix::solveLinearByGauss(Matrix &matrixA, Matrix &matrixB)
{
    if (!matrixA.isValid()) throw qtTrId("A is invalid");
    if (!matrixB.isValid()) throw qtTrId("B is invalid");

    if (matrixA.M() != matrixB.M()) throw qtTrId("Bad dimentions");

    EditableMatrix A(matrixA);
    EditableMatrix B(matrixB);
    Index currentRow;
    Index currentCol;
    Index i, j, k;
    Index maxRow, maxCol;
    MatrixElement maxValue, currentValue, keyValue;

    for (i = 0; i < A.M() && i < A.N(); ++i)
    {
        currentRow = i;
        currentCol = i;

        maxValue = qAbs(A.element(i, i));
        maxRow = currentRow;
        maxCol = currentCol;
        for (j = i; j < A.M(); ++j)
        {
            for (k = i; k < A.N(); ++k)
            {
                currentValue = qAbs(A.element(j, k));
                if (maxValue < currentValue)
                {
                    maxValue = currentValue;
                    maxRow = j;
                    maxCol = k;
                }
            }
        }
        if (qFuzzyIsNull((double)maxValue)) throw qtTrId("No solution");

        A.swapRows(currentRow, maxRow);
        B.swapRows(currentRow, maxRow);

        A.swapCols(currentCol, maxCol);

        currentValue = A.element(currentRow, currentCol);

        {
            keyValue = 1 / currentValue;
            for (k = currentCol; k < A.N(); ++k)
            {
                A.setElement(currentRow, k, A.element(currentRow, k) * keyValue);
            }

            for (k = 0; k < B.N(); ++k)
            {
                B.setElement(currentRow, k, B.element(currentRow, k) * keyValue);
            }
        }

        for (j = 0; j < A.M(); ++j)
        {
            if (j == currentRow)
            {
                continue;
            }
            else
            {
                keyValue = A.element(j, currentCol);
                //A.setElement(j, currentCol, 0);

                for (k = currentCol; k < A.N(); ++k)
                {
                    A.setElement(j, k, A.element(j, k) - A.element(currentRow, k) * keyValue);
                }

                for (k = 0; k < B.N(); ++k)
                {
                    B.setElement(j, k, B.element(j, k) - B.element(currentRow, k) * keyValue);
                }
            }
        }
    }

    matrixA = A.matrixWithUnswapedElements().transposed();
    matrixB = B.matrixWithUnswapedElements();
    matrixB = matrixA * matrixB;
}

Matrix Matrix::inverted() const
{
    if (!isSquare()) throw qtTrId("Can't invert non square matrix.");
    Matrix result(MatrixPrivate::IMatrixPrivate(M()));
    Matrix A(*this);

    Matrix::solveLinearByGauss(A, result);

    return result;
}

