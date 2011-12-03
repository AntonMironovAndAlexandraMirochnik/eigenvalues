#include "QR_task.h"
#include "model/matrix/editable_matrix.h"

QRTask::QRTask(const Matrix &initialMatrix, int computationAccuracy)
    : EigenvaluesTask(initialMatrix, computationAccuracy, Task::QRTaskType)
{
}

void QRTask::run()
{
    startTask();

    //output
    EditableMatrix eigenvaluesMatrix(initialMatrix().M(), 1);
    int iterationsNumber = 0;
    qreal resultAccuracy = HUGE_VALUE;

    Matrix A(initialMatrix());
    Index i, j;

    for (j = 1; j < A.N(); j++)
    {
	for (i = j + 1; i < A.N(); i++)
	{
	    MatrixElement s, c;
	    s = A.element(i, j - 1) / qSqrt(qPow(A.element(j, j-1), 2) + qPow(A.element(i, j-1), 2));
	    c = - A.element(j, j - 1) / qSqrt(qPow(A.element(j, j-1), 2) + qPow(A.element(i, j-1), 2));

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
	if (resultAccuracy < epsilon()) break;

	++iterationsNumber;
	Matrix R(A);
	Matrix Q(*Matrix::MatrixIdentical(A.N(), A.N()));
	for (j = 0; j < A.N() - 1; j++)
	{
	    for (i = j + 1; i < A.N(); i++)
	    {
		MatrixElement s, c;
		s = A.element(i, j) / qSqrt(qPow(A.element(j, i), 2) + qPow(A.element(i, i), 2));
		c = - A.element(i, i) / qSqrt(qPow(A.element(j, i), 2) + qPow(A.element(i, i), 2));

		EditableMatrix T(*Matrix::MatrixIdentical(A.N(), A.N()));
		T.element(i, i) = c;
		T.element(i, j) = -s;
		T.element(j, i) = s;
		T.element(j, j) = c;

		R = T * R;
		Q = Q * T.transposed();
	    }
	}
	A = R * Q;
    }

    for (i = 0; i < A.N(); ++i)
    {
	eigenvaluesMatrix.element(i, 0) = A.element(i, i);
    }
    done(eigenvaluesMatrix, iterationsNumber, resultAccuracy);
}

