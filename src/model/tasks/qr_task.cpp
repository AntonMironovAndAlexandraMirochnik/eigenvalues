#include "eigenvalues_task.h"
#include "model/matrix/editable_matrix.h"

EigenvaluesResult solveWithQR(const EigenvaluesTask &task)
{
    QTime time;
    time.start();

    //output
    EditableMatrix eigenvaluesMatrix(task.matrix().M(), 1);
    int iterationsNumber = 0;
    qreal resultAccuracy = HUGE_VALUE;

    Matrix A(task.matrix());
    Index i, j;

    for (j = 1; j < A.N() - 1; j++)
    {
        for (i = j + 1; i < A.N(); i++)
        {
            MatrixElement s, c;
            s = A.element(i, j - 1) / qSqrt(qPow(A.element(j, j - 1), 2) + qPow(A.element(i, j - 1), 2));
            c = - (A.element(j, j - 1) / qSqrt(qPow(A.element(j, j - 1), 2) + qPow(A.element(i, j - 1), 2)));

            EditableMatrix T(*Matrix::MatrixIdentical(A.N(), A.N()));
            T.element(i, i) = c;
            T.element(i, j) = -s;
            T.element(j, i) = s;
            T.element(j, j) = c;

            A = T.transposed() * A * T;
        }
    }

    for (;;)
    {
        Matrix::Element maxElementIndex = A.maximalNondiagonalElementIndex();
        resultAccuracy = qPow(A.element(maxElementIndex.i, maxElementIndex.j), 2);
        if (resultAccuracy < task.epsilon()) break;

        ++iterationsNumber;
        Matrix R(A);
        Matrix Q(*Matrix::MatrixIdentical(A.N(), A.N()));
        for (i = 0; i < A.N() - 1; i++)
            {
                for (j = i + 1; j < A.N(); j++)
                    {
                        MatrixElement s, c;

                        MatrixElement a_ii = R.element(i, i);
                        MatrixElement a_ji = R.element(j, i);
                        MatrixElement div = qSqrt(qPow(a_ii, 2) + qPow(a_ji, 2));

                        s = a_ji / div;
                        c = a_ii / div;

                        EditableMatrix T(*Matrix::MatrixIdentical(A.N(), A.N()));
                        T.element(i, i) = c;
                        T.element(i, j) = -s;
                        T.element(j, i) = s;
                        T.element(j, j) = c;

                        R = T.transposed() * R;
                        Q = Q * T;
                    }
            }
        A = R * Q;
    }

    for (i = 0; i < A.N(); ++i)
    {
        eigenvaluesMatrix.element(i, 0) = A.element(i, i);
    }

    return EigenvaluesResult(eigenvaluesMatrix, iterationsNumber, resultAccuracy, time.elapsed());
}

