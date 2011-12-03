#include "jacobi_task.h"
#include "model/matrix/editable_matrix.h"

JacobiTask::JacobiTask(const Matrix &initialMatrix, int computationAccuracy)
    : EigenvaluesTask(initialMatrix, computationAccuracy, Task::JacobiTaskType)
{
}

qreal sign(qreal value)
{
    if (value > 0) return 1;
    if (value < 0) return -1;
    return 0;
}

void JacobiTask::run()
{
    startTask();

    //output
    EditableMatrix eigenvaluesMatrix(initialMatrix().M(), 1);
    int iterationsNumber = 0;
    qreal resultAccuracy = HUGE_VALUE;

    if (!initialMatrix().isSymmetric())
    {
	throw tr("Matrix is not symmetric.");
    }

    Matrix A(initialMatrix());

    for (;;)
    {
	Matrix::Element maxElementIndex = A.maximalNondiagonalElementIndex();
	resultAccuracy = qAbs(A.element(maxElementIndex.i, maxElementIndex.j));
	if (resultAccuracy < epsilon()) break;

	++iterationsNumber;
	double mu = 2 * A.element(maxElementIndex.i, maxElementIndex.j) / (A.element(maxElementIndex.i, maxElementIndex.i) - A.element(maxElementIndex.j, maxElementIndex.j));
	MatrixElement c, s;
	c = qSqrt((1 + 1/qSqrt(1 + qPow(mu, 2))) * 0.5);
	s = sign(mu) * qSqrt((1 - 1/qSqrt(1 + qPow(mu, 2))) * 0.5);

	EditableMatrix T(*Matrix::MatrixIdentical(A.N(), A.N()));
	T.element(maxElementIndex.i, maxElementIndex.i) = c;
	T.element(maxElementIndex.i, maxElementIndex.j) = -s;
	T.element(maxElementIndex.j, maxElementIndex.i) = s;
	T.element(maxElementIndex.j, maxElementIndex.j) = c;

	A = T.transposed() * A * T;
    }

    for (Index i = 0; i < A.N(); ++i)
    {
	eigenvaluesMatrix.element(i, 0) = A.element(i, i);
    }
    done(eigenvaluesMatrix, iterationsNumber, resultAccuracy);
}
