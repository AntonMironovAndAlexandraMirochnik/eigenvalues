#include "matrix.h"
#include "matrix_private.h"
#include <QtCore>

Matrix::Matrix(const MatrixPrivate &privateData)
{
    matrixPrivate = privateData;
}

Matrix::Matrix()
{
}

Matrix::Matrix(const QString &textData)
{
    QString text = textData;
    text.replace(QRegExp("[\\s]"), QString("|"));
    text.replace(',', '.');
    QStringList data = text.split('|', QString::SkipEmptyParts);
    if (data.count() < 3) throw qtTrId("Not enough text parameters");

    QStringList::const_iterator i = data.constBegin();
    bool ok;
    MatrixElement value;

    Index m = (*i++).toDouble(&ok);
    if (!ok) throw qtTrId("can't read data from file");
    Index n = (*i++).toDouble(&ok);
    if (!ok) throw qtTrId("can't read data from file");
    matrixPrivate = MatrixPrivate::EmptyMatrixPrivate(m, n);
    Index j;
    for (j = 0; i != data.constEnd() && j < matrixPrivate.elementsCount; ++i, ++j)
    {
	value = (*i).toDouble(&ok);
	if (!ok) throw qtTrId("can't read data from file");
	matrixPrivate.elementsData[j] = value;
    }

    data.clear();
    if (j < matrixPrivate.elementsCount)
	throw qtTrId("Not enough text parameters");
}

Matrix::Matrix(const Matrix& matrixToCopy)
    : matrixPrivate(matrixToCopy.matrixPrivate)
{
}

Matrix::Matrix(const Matrix& matrixToCopy, int M, int N)
    : matrixPrivate(MatrixPrivate::EmptyMatrixPrivate(M, N))
{
    for (int i = 0; i < M && i < matrixToCopy.M(); ++i)
    {
	for (int j = 0; j < N && j < matrixToCopy.N(); ++j)
	{
	    matrixPrivate.setElement(i, j, matrixToCopy.element(i, j));
	}
    }
}

Matrix::Matrix(Index N)
    : matrixPrivate(MatrixPrivate::EmptyMatrixPrivate(N, N))
{}

Matrix::Matrix(Index N, const MatrixElement &value)
    : matrixPrivate(MatrixPrivate::FilledMatrixPrivate(N, N, value))
{}

Matrix::Matrix(Index M, Index N)
    : matrixPrivate(MatrixPrivate::EmptyMatrixPrivate(M, N))
{}

Matrix::Matrix(Index M, Index N, const MatrixElement &value)
    : matrixPrivate(MatrixPrivate::FilledMatrixPrivate(M, N, value))
{}

Matrix* Matrix::MatrixFromFile(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
    {
	return new Matrix();
    }
    return new Matrix(QString(file.readAll()));
}

Matrix* Matrix::MatrixRandomized(int M, int N, qreal minValue, qreal maxValue)
{
    Matrix* matrix = new Matrix(M, N);
    Index i, j;
    for (i = 0; i < M; ++i)
    {
	for (j = 0; j < N; ++j)
	{
	    matrix->matrixPrivate.setElement(i, j, minValue + ((qreal)qrand() / (qreal)RAND_MAX) * (maxValue - minValue));
	}
    }
    return matrix;
}

Matrix* Matrix::MatrixSymmetricRandomized(int N, qreal minValue, qreal maxValue)
{
    Matrix* matrix = new Matrix(N, N);
    qreal value;
    Index i, j;
    for (i = 0; i < N; ++i)
    {
	for (j = i; j < N; ++j)
	{
	    value = minValue + ((qreal)qrand() / (qreal)RAND_MAX) * (maxValue - minValue);
	    matrix->matrixPrivate.setElement(i, j, value);
	    matrix->matrixPrivate.setElement(j, i, value);
	}
    }
    return matrix;
}

Matrix* Matrix::MatrixIdentical(int M, int N)
{
    Matrix* matrix = new Matrix(N, N, 0.0);
    Index i;
    for (i = 0; i < N && i < M; ++i)
    {
	matrix->matrixPrivate.setElement(i, i, 1.0);
    }
    return matrix;
}


Index Matrix::M() const {return matrixPrivate.sizeM;}
Index Matrix::N() const {return matrixPrivate.sizeN;}

MatrixElement Matrix::element(Index i, Index j) const
{
    return matrixPrivate.element(i, j);
}

void Matrix::setElement(Index i, Index j, MatrixElement value)
{
    matrixPrivate.setElement(i, j, value);
}

Matrix::Element Matrix::maximalNondiagonalElementIndex()
{
    Index i, j;
    MatrixElement value;

    Index maxI = 0, maxJ = 0;
    MatrixElement maxValue = 0;

    for (i = 0; i < N(); ++i)
    {
	for (j = i + 1; j < N(); ++j)
	{
	    value = qAbs(element(i, j));
	    if (value > maxValue)
	    {
		maxI = i;
		maxJ = j;
		maxValue = value;
	    }
	}
    }
    Element element;
    element.i = maxI;
    element.j = maxJ;
    element.owner = this;
    return element;
}

bool Matrix::isValid() const
{
    return (matrixPrivate.matrixFlags & MatrixFlagIsValid);
}

bool Matrix::isSquare() const
{
    return (matrixPrivate.matrixFlags & MatrixFlagIsSqure);
}

bool Matrix::isSymmetric()
{
    if (!(matrixPrivate.matrixFlags & MatrixFlagSymmeticWasChecked))
    {
	bool isSymmetric = true;
	Index i, j;
	for (i = 0; i < N(); ++i)
	{
	for (j = i + 1; j < N(); ++j)
	    {
	    MatrixElement a = element(i, j);
	    MatrixElement b = element(j, i);
	    if (!qFuzzyCompare(a, b))
	    {
		isSymmetric = false;
		break;
	    }
	    }
	}
	if (isSymmetric)
	    matrixPrivate.matrixFlags |= MatrixFlagIsSymmetric;
    }

    return (matrixPrivate.matrixFlags & MatrixFlagIsSymmetric);
}

QString Matrix::toString() const
{
    QString result;
    Index i, j;
    QString numberTemplate("\t%1");
#define addNumber(number) result += numberTemplate.arg(number, 0, 'g', 3, QChar())

    result += numberTemplate.arg(M());
    result += numberTemplate.arg(N());
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

void Matrix::saveToFile(QString fileName) const
{
    QFile file(fileName);
    file.open(QFile::WriteOnly);
    file.write(toString().toLocal8Bit());
}


Matrix Matrix::transposed() const
{
    Matrix matrix(N(), M());
    Index i, j;
    for (i = 0; i < M(); ++i)
    {
	for (j = 0; j < N(); ++j)
	{
	    matrix.setElement(j, i, element(i, j));
	}
    }
    return matrix;
}

Matrix  operator *(const Matrix& A, const Matrix& B)
{
    if (A.N() != B.M()) throw qtTrId("Bad dimentions for mult");
    return Matrix(MatrixPrivate::mult(A.matrixPrivate, B.matrixPrivate));
}

Matrix operator -(const Matrix& A, const Matrix& B)
{
    if (A.M() != B.M() || A.N() != B.N()) throw qtTrId("Bad dimentions for dif");
    return Matrix(MatrixPrivate::dif(A.matrixPrivate, B.matrixPrivate));
}
