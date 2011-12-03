#include "jacobi_task.h"
#include "model/matrix/editable_matrix.h"

JacobiTask::JacobiTask(const Matrix &initialMatrix, int computationAccuracy)
    : EigenvaluesTask(initialMatrix, computationAccuracy, Task::JacobiTaskType)
{
}

void JacobiTask::run()
{
    startTask();

    //output
    Matrix eigenvaluesMatrix(initialMatrix().M(), 1);
    int iterationsNumber = 0;
    qreal resultAccuracy = HUGE_VALUE;

    {
/*
	ToDo: insert your solution code here
*/
	mutex.lock();
	QWaitCondition condition;
	condition.wait(&mutex, qrand() % 1000);
	mutex.unlock();
    }

    done(eigenvaluesMatrix, iterationsNumber, resultAccuracy);
}
