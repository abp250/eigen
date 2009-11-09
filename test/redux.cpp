// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2008 Benoit Jacob <jacob.benoit.1@gmail.com>
//
// Eigen is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// Alternatively, you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// Eigen is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License or the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License and a copy of the GNU General Public License along with
// Eigen. If not, see <http://www.gnu.org/licenses/>.

#include "main.h"

template<typename MatrixType> void matrixRedux(const MatrixType& m)
{
  typedef typename MatrixType::Scalar Scalar;

  int rows = m.rows();
  int cols = m.cols();

  MatrixType m1 = MatrixType::Random(rows, cols);

  VERIFY_IS_MUCH_SMALLER_THAN(MatrixType::Zero(rows, cols).sum(), Scalar(1));
  VERIFY_IS_APPROX(MatrixType::Ones(rows, cols).sum(), Scalar(float(rows*cols))); // the float() here to shut up excessive MSVC warning about int->complex conversion being lossy
  Scalar s(0), p(1), minc(ei_real(m1.coeff(0))), maxc(ei_real(m1.coeff(0)));
  for(int j = 0; j < cols; j++)
  for(int i = 0; i < rows; i++)
  {
    s += m1(i,j);
    p *= m1(i,j);
    minc = std::min(ei_real(minc), ei_real(m1(i,j)));
    maxc = std::max(ei_real(maxc), ei_real(m1(i,j)));
  }
  VERIFY_IS_APPROX(m1.sum(), s);
  VERIFY_IS_APPROX(m1.prod(), p);
  VERIFY_IS_APPROX(m1.real().minCoeff(), ei_real(minc));
  VERIFY_IS_APPROX(m1.real().maxCoeff(), ei_real(maxc));
}

template<typename VectorType> void vectorRedux(const VectorType& w)
{
  typedef typename VectorType::Scalar Scalar;
  typedef typename NumTraits<Scalar>::Real RealScalar;
  int size = w.size();

  VectorType v = VectorType::Random(size);
  for(int i = 1; i < size; i++)
  {
    Scalar s(0), p(1);
    RealScalar minc(ei_real(v.coeff(0))), maxc(ei_real(v.coeff(0)));
    for(int j = 0; j < i; j++)
    {
      s += v[j];
      p *= v[j];
      minc = std::min(minc, ei_real(v[j]));
      maxc = std::max(maxc, ei_real(v[j]));
    }
    VERIFY_IS_APPROX(s, v.start(i).sum());
    VERIFY_IS_APPROX(p, v.start(i).prod());
    VERIFY_IS_APPROX(minc, v.real().start(i).minCoeff());
    VERIFY_IS_APPROX(maxc, v.real().start(i).maxCoeff());
  }

  for(int i = 0; i < size-1; i++)
  {
    Scalar s(0), p(1);
    RealScalar minc(ei_real(v.coeff(i))), maxc(ei_real(v.coeff(i)));
    for(int j = i; j < size; j++)
    {
      s += v[j];
      p *= v[j];
      minc = std::min(minc, ei_real(v[j]));
      maxc = std::max(maxc, ei_real(v[j]));
    }
    VERIFY_IS_APPROX(s, v.end(size-i).sum());
    VERIFY_IS_APPROX(p, v.end(size-i).prod());
    VERIFY_IS_APPROX(minc, v.real().end(size-i).minCoeff());
    VERIFY_IS_APPROX(maxc, v.real().end(size-i).maxCoeff());
  }

  for(int i = 0; i < size/2; i++)
  {
    Scalar s(0), p(1);
    RealScalar minc(ei_real(v.coeff(i))), maxc(ei_real(v.coeff(i)));
    for(int j = i; j < size-i; j++)
    {
      s += v[j];
      p *= v[j];
      minc = std::min(minc, ei_real(v[j]));
      maxc = std::max(maxc, ei_real(v[j]));
    }
    VERIFY_IS_APPROX(s, v.segment(i, size-2*i).sum());
    VERIFY_IS_APPROX(p, v.segment(i, size-2*i).prod());
    VERIFY_IS_APPROX(minc, v.real().segment(i, size-2*i).minCoeff());
    VERIFY_IS_APPROX(maxc, v.real().segment(i, size-2*i).maxCoeff());
  }
}

void test_redux()
{
  for(int i = 0; i < g_repeat; i++) {
    CALL_SUBTEST( matrixRedux(Matrix<float, 1, 1>()) );
    CALL_SUBTEST( matrixRedux(Matrix2f()) );
    CALL_SUBTEST( matrixRedux(Matrix4d()) );
    CALL_SUBTEST( matrixRedux(MatrixXcf(3, 3)) );
    CALL_SUBTEST( matrixRedux(MatrixXd(8, 12)) );
    CALL_SUBTEST( matrixRedux(MatrixXi(8, 12)) );
  }
  for(int i = 0; i < g_repeat; i++) {
    CALL_SUBTEST( vectorRedux(Vector4f()) );
    CALL_SUBTEST( vectorRedux(VectorXd(10)) );
    CALL_SUBTEST( vectorRedux(VectorXf(33)) );
  }
}