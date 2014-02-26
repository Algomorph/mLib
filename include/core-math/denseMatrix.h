
#pragma once
#ifndef DENSE_MATRIX_H
#define DENSE_MATRIX_H

template <class D> class DenseMatrix
{
public:
	DenseMatrix()
	{
		m_rows = 0;
		m_cols = 0;
	}

	DenseMatrix(const DenseMatrix<D>& s)
	{
		m_rows = s.m_rows;
		m_cols = s.m_cols;
		m_data = s.m_data;
		m_dataPtr = m_data.ptr();
	}

	DenseMatrix(DenseMatrix &&s)
	{
		m_rows = s.m_rows;
		m_cols = s.m_cols;
		s.m_rows = 0;
		s.m_cols = 0;
		m_data = std::move(s.m_data);
		m_dataPtr = m_data.ptr();
	}

	explicit DenseMatrix(UINT squareDimension)
	{
		m_rows = squareDimension;
		m_cols = squareDimension;
		m_data.allocate(m_rows * m_cols);
		m_dataPtr = m_data.ptr();
	}

	explicit DenseMatrix(const Vector<D> &diagonal)
	{
		m_rows = (UINT)diagonal.size();
		m_cols = (UINT)diagonal.size();
		m_data.allocate(m_rows * m_cols);
		m_dataPtr = m_data.ptr();
		for(UINT row = 0; row < m_rows; row++)
		{
			for(UINT col = 0; col < m_cols; col++)
				(*this)(row, col) = 0.0;
			(*this)(row, row) = diagonal[row];
		}

	}

	DenseMatrix(UINT rows, UINT cols)
	{
		m_rows = rows;
		m_cols = cols;
		m_data.allocate(m_rows * m_cols);
		m_dataPtr = m_data.ptr();
	}

	DenseMatrix(const String &s, MatrixStringFormat format)
	{
		if(format == MatrixStringFormatMathematica)
		{
			//
			// this is really a dense format and should be loaded as such, then cast into a SparseMatrix
			//
			Vector<String> data = s.split("},{");
			m_rows = (UINT)data.size();
			m_cols = (UINT)data[0].split(",").size();
			m_data.allocate(m_rows * m_cols);
			m_dataPtr = m_data.ptr();

			for(UINT row = 0; row < m_rows; row++)
			{
				Vector<String> values = data[row].split(",");
				for(UINT col = 0; col < values.size(); col++)
				{
					(*this)(row, col) = (D)values[col].findAndReplace("{","").findAndReplace("}","").toDOUBLE();
				}
			}
		}
		else
		{
			MLIB_ERROR("invalid matrix string format");
		}
	}

	void operator=(const DenseMatrix<D>& s)
	{
		m_rows = s.m_rows;
		m_cols = s.m_cols;
		m_data = s.m_data;
		m_dataPtr = m_data.ptr();
	}

	void operator=(DenseMatrix<D>&& s)
	{
		m_rows = s.m_rows;
		m_cols = s.m_cols;
		s.m_rows = 0;
		s.m_cols = 0;
		m_data = std::move(s.m_data);
		m_dataPtr = m_data.ptr();
	}

	//
	// Accessors
	//
	D& operator()(UINT row, UINT col)
	{
		return m_dataPtr[row * m_cols + col];
	}
	D operator()(UINT row, UINT col) const
	{
		return m_dataPtr[row * m_cols + col];
	}
	UINT rows() const
	{
		return m_rows;
	}
	UINT cols() const
	{
		return m_cols;
	}
	bool square() const
	{
		return (m_rows == m_cols);
	}
	Vector<D> diagonal() const
	{
		MLIB_ASSERT(square(), "diagonal called on non-square matrix");
		Vector<D> result(m_rows);
		for(UINT row = 0; row < m_rows; row++)
			result[row] = m_data[row * m_cols + row];
		return result;
	}

	//
	// Math functions
	//
	DenseMatrix<D> transpose() const;
	D maxMagnitude() const;
	DenseMatrix<D> inverse();
	void invertInPlace();
	bool valid() const;

	//
	// overloaded operator helpers
	//
	static DenseMatrix<D> add(const DenseMatrix<D> &A, const DenseMatrix<D> &B);
	static DenseMatrix<D> subtract(const DenseMatrix<D> &A, const DenseMatrix<D> &B);
	static DenseMatrix<D> multiply(const DenseMatrix<D> &A, D c);
	static Vector<D> multiply(const DenseMatrix<D> &A, const Vector<D> &v);
	static DenseMatrix<D> multiply(const DenseMatrix<D> &A, const DenseMatrix<D> &B);

	//
	// common matrices
	//
	static DenseMatrix<D> identity(int n)
	{
		return DenseMatrix<D>(Vector<D>(n, (D)1.0));
	}

private:
	UINT m_rows, m_cols;
	D* m_dataPtr;
    Vector< D > m_data;
};

template<class D>
DenseMatrix<D> operator + (const DenseMatrix<D> &A, const DenseMatrix<D> &B)
{
	return DenseMatrix<D>::add(A, B);
}

template<class D>
DenseMatrix<D> operator - (const DenseMatrix<D> &A, const DenseMatrix<D> &B)
{
	return DenseMatrix<D>::subtract(A, B);
}

template<class D>
DenseMatrix<D> operator * (const DenseMatrix<D> &A, const DenseMatrix<D> &B)
{
	return DenseMatrix<D>::multiply(A, B);
}

template<class D>
Vector<D> operator * (const DenseMatrix<D> &A, const Vector<D> &B)
{
	return DenseMatrix<D>::multiply(A, B);
}

template<class D>
DenseMatrix<D> operator * (const DenseMatrix<D> &A, D val)
{
	return DenseMatrix<D>::multiply(A, val);
}

#include "denseMatrix.cpp"

#endif // SPARSE_MATRIX_H