// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2009 Gael Guennebaud <g.gael@free.fr>
// Copyright (C) 2009 Hauke Heibel <hauke.heibel@googlemail.com>
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

#ifndef EIGEN_STDVECTOR_H
#define EIGEN_STDVECTOR_H

#include "Eigen/src/StlSupport/details.h"

// Define the explicit instantiation (e.g. necessary for the Intel compiler)
#if defined(__INTEL_COMPILER) || defined(__GNUC__)
  #define EIGEN_EXPLICIT_STL_VECTOR_INSTANTIATION(...) template class std::vector<__VA_ARGS__, Eigen::aligned_allocator<__VA_ARGS__> >;
#else
  #define EIGEN_EXPLICIT_STL_VECTOR_INSTANTIATION(...)
#endif

/**
 * This section contains a convenience MACRO which allows an easy specialization of
 * std::vector such that for data types with alignment issues the correct allocator
 * is used automatically.
 */
#define EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(...) \
EIGEN_EXPLICIT_STL_VECTOR_INSTANTIATION(__VA_ARGS__) \
namespace std \
{ \
  template<typename _Ay> \
  class vector<__VA_ARGS__, _Ay>  \
    : public vector<__VA_ARGS__, Eigen::aligned_allocator<__VA_ARGS__> > \
  { \
    typedef vector<__VA_ARGS__, Eigen::aligned_allocator<__VA_ARGS__> > vector_base; \
  public: \
    typedef __VA_ARGS__ value_type; \
    typedef typename vector_base::allocator_type allocator_type; \
    typedef typename vector_base::size_type size_type;  \
    typedef typename vector_base::iterator iterator;  \
    explicit vector(const allocator_type& a = allocator_type()) : vector_base(a) {}  \
    template<typename InputIterator> \
    vector(InputIterator first, InputIterator last, const allocator_type& a = allocator_type()) : vector_base(first, last, a) {} \
    vector(const vector& c) : vector_base(c) {}  \
    explicit vector(size_type num, const value_type& val = value_type()) : vector_base(num, val) {} \
    vector(iterator start, iterator end) : vector_base(start, end) {}  \
    vector& operator=(const vector& x) {  \
      vector_base::operator=(x);  \
      return *this;  \
    } \
  }; \
}

namespace std {

#define EIGEN_STD_VECTOR_SPECIALIZATION_BODY \
  public:  \
    typedef T value_type; \
    typedef typename vector_base::allocator_type allocator_type; \
    typedef typename vector_base::size_type size_type;  \
    typedef typename vector_base::iterator iterator;  \
    typedef typename vector_base::const_iterator const_iterator;  \
    explicit vector(const allocator_type& a = allocator_type()) : vector_base(a) {}  \
    template<typename InputIterator> \
    vector(InputIterator first, InputIterator last, const allocator_type& a = allocator_type()) \
    : vector_base(first, last, a) {} \
    vector(const vector& c) : vector_base(c) {}  \
    explicit vector(size_type num, const value_type& val = value_type()) : vector_base(num, val) {} \
    vector(iterator start, iterator end) : vector_base(start, end) {}  \
    vector& operator=(const vector& x) {  \
      vector_base::operator=(x);  \
      return *this;  \
    }

template<typename T>
class vector<T,Eigen::aligned_allocator<T> >
  : public vector<EIGEN_WORKAROUND_MSVC_STL_SUPPORT(T),
                  Eigen::aligned_allocator_indirection<EIGEN_WORKAROUND_MSVC_STL_SUPPORT(T)> >
{
  typedef vector<EIGEN_WORKAROUND_MSVC_STL_SUPPORT(T),
                 Eigen::aligned_allocator_indirection<EIGEN_WORKAROUND_MSVC_STL_SUPPORT(T)> > vector_base;
  EIGEN_STD_VECTOR_SPECIALIZATION_BODY

  void resize(size_type new_size)
  { resize(new_size, T()); }

#if defined(_VECTOR_)
  // workaround MSVC std::vector implementation
  void resize(size_type new_size, const value_type& x)
  {
    if (vector_base::size() < new_size)
      vector_base::_Insert_n(vector_base::end(), new_size - vector_base::size(), x);
    else if (new_size < vector_base::size())
      vector_base::erase(vector_base::begin() + new_size, vector_base::end());
  }
  void push_back(const value_type& x)
  { vector_base::push_back(x); } 
  using vector_base::insert;  
  iterator insert(const_iterator position, const value_type& x)
  { return vector_base::insert(position,x); }
  void insert(const_iterator position, size_type new_size, const value_type& x)
  { vector_base::insert(position, new_size, x); }
#elif defined(_GLIBCXX_VECTOR) && EIGEN_GNUC_AT_LEAST(4,2)
  // workaround GCC std::vector implementation
  void resize(size_type new_size, const value_type& x)
  {
    if (new_size < vector_base::size())
      vector_base::_M_erase_at_end(this->_M_impl._M_start + new_size);
    else
      vector_base::insert(vector_base::end(), new_size - vector_base::size(), x);
  }
#else
  // either GCC 4.1 or non-GCC
  // default implementation which should always work.
  void resize(size_type new_size, const value_type& x)
  {
    if (new_size < vector_base::size())
      vector_base::erase(vector_base::begin() + new_size, vector_base::end());
    else if (new_size > vector_base::size())
      vector_base::insert(vector_base::end(), new_size - vector_base::size(), x);
  }
#endif

};

}

#endif // EIGEN_STDVECTOR_H
