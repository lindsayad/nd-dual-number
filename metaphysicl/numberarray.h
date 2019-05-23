//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
//
// MetaPhysicL - A metaprogramming library for physics calculations
//
// Copyright (C) 2013 The PECOS Development Team
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the Version 2.1 GNU Lesser General
// Public License as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc. 51 Franklin Street, Fifth Floor,
// Boston, MA  02110-1301  USA
//
//-----------------------------------------------------------------------el-
//
// $Id$
//
//--------------------------------------------------------------------------


#ifndef METAPHYSICL_NUMBERARRAY_H
#define METAPHYSICL_NUMBERARRAY_H

#include <algorithm>
#include <ostream>

#include "metaphysicl/compare_types.h"
#include "metaphysicl/ct_types.h"
#include "metaphysicl/raw_type.h"

namespace MetaPhysicL {

// Forward declarations
template<std::size_t N, typename T>
class NumberArray;

template<std::size_t N, typename S, typename T, bool reverseorder>
struct DotType<NumberArray<N,S>, NumberArray<N,T>, reverseorder> {
  typedef NumberArray<N, typename DotType<S,T,reverseorder>::supertype> supertype;
};

template<std::size_t N, typename S, typename T, bool reverseorder>
struct OuterProductType<NumberArray<N,S>, NumberArray<N,T>, reverseorder> {
  typedef
  NumberArray<N, typename OuterProductType<S,T,reverseorder>::supertype> supertype;
};

template<std::size_t N, typename S>
struct SumType<NumberArray<N,S> > {
  typedef NumberArray<N, typename SumType<S>::supertype> supertype;
};

template <std::size_t N, typename T>
class NumberArray
{
public:
  typedef T value_type;

  template <std::size_t i>
  struct entry_type {
    typedef value_type type;
  };

  template <typename T2>
  struct rebind {
    typedef NumberArray<N, T2> other;
  };

  template <size_t N2>
  struct resize {
    typedef NumberArray<N2, T> other;
  };

  NumberArray() = default;

  NumberArray(const T& val)
    { std::fill(_data, _data+N, val); }

  NumberArray(const T* vals)
    { std::copy(vals, vals+N, _data); }

  template <typename T2>
  NumberArray(NumberArray<N, T2> src)
    { if (N) std::copy(&src[0], &src[0]+N, _data); }

  template <typename T2>
  NumberArray(const T2& val)
    { std::fill(_data, _data+N, T(val)); }

  template <typename T2,
            typename std::enable_if<ScalarTraits<T2>::value,
                                    int>::type = 0>
  NumberArray & operator=(const T2 & val)
    {
      std::fill(_data, _data+N, T(val));
      return *this;
    }

  T& operator[](std::size_t i)
    { return _data[i]; }

  const T& operator[](std::size_t i) const
    { return _data[i]; }

  T& raw_at (std::size_t i)
    { return _data[i]; }

  const T& raw_at (std::size_t i) const
    { return _data[i]; }

  template <std::size_t i>
  typename entry_type<i>::type& get()
    { return _data[i]; }

  template <std::size_t i>
  const typename entry_type<i>::type& get() const
    { return _data[i]; }

  template <std::size_t i>
  typename entry_type<i>::type& insert()
    { return _data[i]; }

  std::size_t size() const
    { return N; }

  NumberArray<N,T> operator- () const {
    NumberArray<N,T> returnval;
    for (std::size_t i=0; i != N; ++i) returnval[i] = -_data[i];
    return returnval;
  }

  NumberArray<N,T> operator! () const {
    NumberArray<N,T> returnval;
    for (std::size_t i=0; i != N; ++i) returnval[i] = !_data[i];
    return returnval;
  }

  template <typename T2>
  NumberArray<N,T>& operator+= (const NumberArray<N,T2>& a)
    { for (std::size_t i=0; i != N; ++i) _data[i] += a[i]; return *this; }

  template <typename T2>
  NumberArray<N,T>& operator+= (const T2& a)
    { for (std::size_t i=0; i != N; ++i) _data[i] += a; return *this; }

  template <typename T2>
  NumberArray<N,T>& operator-= (const NumberArray<N,T2>& a)
    { for (std::size_t i=0; i != N; ++i) _data[i] -= a[i]; return *this; }

  template <typename T2>
  NumberArray<N,T>& operator-= (const T2& a)
    { for (std::size_t i=0; i != N; ++i) _data[i] -= a; return *this; }

  template <typename T2>
  NumberArray<N,T>& operator*= (const NumberArray<N,T2>& a)
    { for (std::size_t i=0; i != N; ++i) _data[i] *= a[i]; return *this; }

  template <typename T2>
  NumberArray<N,T>& operator*= (const T2& a)
    { for (std::size_t i=0; i != N; ++i) _data[i] *= a; return *this; }

  template <typename T2>
  NumberArray<N,T>& operator/= (const NumberArray<N,T2>& a)
    { for (std::size_t i=0; i != N; ++i) _data[i] /= a[i]; return *this; }

  template <typename T2>
  NumberArray<N,T>& operator/= (const T2& a)
    { for (std::size_t i=0; i != N; ++i) _data[i] /= a; return *this; }

  template <typename T2>
  NumberArray<N, typename DotType<T,T2>::supertype>
  dot (const NumberArray<N,T2>& a) const
  {
    NumberArray<N, typename DotType<T,T2>::supertype> returnval;
    for (std::size_t i=0; i != N; ++i)
      returnval[i] = _data[i].dot(a[i]);
    return returnval;
  }

  template <typename T2>
  typename OuterProductType<NumberArray<N,T>,NumberArray<N,T2> >::supertype
  outerproduct (const NumberArray<N,T2>& a) const
  {
    typename OuterProductType<NumberArray<N,T>,NumberArray<N,T2> >::supertype
      returnval;

    for (std::size_t i=0; i != N; ++i)
      returnval[i] = _data[i].outerproduct(a[i]);

    return returnval;
  }

  void zero()
  {
    std::fill(_data, _data+N, T());
  }

private:
  T _data[N];
};



//
// Non-member functions
//

template <std::size_t N, typename B, typename T, typename T2>
inline
typename CompareTypes<T,T2>::supertype
if_else (const NumberArray<N,B> & condition, const T & if_true, const T2 & if_false)
{
  typename CompareTypes<T,T2>::supertype returnval(if_true);
  for (std::size_t i = 0; i != N; ++i)
    if (!condition[i])
      returnval[i] = if_false[i];

  return returnval;
}



template <std::size_t N,
          unsigned int index1=0, typename Data1=void,
          unsigned int index2=0, typename Data2=void,
          unsigned int index3=0, typename Data3=void,
          unsigned int index4=0, typename Data4=void,
          unsigned int index5=0, typename Data5=void,
          unsigned int index6=0, typename Data6=void,
          unsigned int index7=0, typename Data7=void,
          unsigned int index8=0, typename Data8=void>
struct NumberArrayOf
{
  typedef
  typename CompareTypes<Data1,
    typename CompareTypes<Data2,
      typename CompareTypes<Data3,
        typename CompareTypes<Data4,
          typename CompareTypes<Data5,
            typename CompareTypes<Data6,
              typename CompareTypes<Data7,Data8>::supertype
            >::supertype
          >::supertype
        >::supertype
      >::supertype
    >::supertype
  >::supertype supertype;

  typedef NumberArray<N, supertype> type;
};



template <std::size_t N, std::size_t index, typename T>
struct NumberArrayUnitVector
{
  typedef NumberArray<N, T> type;

  static type value() {
    type returnval = 0;
    returnval[index] = 1;
    return returnval;
  }
};


template <std::size_t N, typename T>
struct NumberArrayFullVector
{
  typedef NumberArray<N,T> type;

  static type value() {
    type returnval;
    for (std::size_t i=0; i != N; ++i)
      returnval[i] = 1;
    return returnval;
  }
};



template <std::size_t N, typename T>
inline
NumberArray<N, T>
transpose(NumberArray<N, T> a)
{
  NumberArray<N, T> returnval;
  for (std::size_t i=0; i != N; ++i)
      returnval[i] = transpose(a[i]);

  return returnval;
}

template<std::size_t N, typename T>
NumberArray<N, typename SumType<T>::supertype>
sum (const NumberArray<N, T>& a)
{
  NumberArray<N, typename SumType<T>::supertype>
    returnval = 0;

  for (std::size_t i=0; i != N; ++i)
    returnval[i] = a[i].sum();

  return returnval;
}

#define NumberArray_op(opname, functorname)                                                        \
  template <std::size_t N, typename T, typename T2>                                                \
  inline auto operator opname(const NumberArray<N, T> & a, const NumberArray<N, T2> & b)           \
      ->NumberArray<N, decltype(a[0] opname b[0])>                                                 \
  {                                                                                                \
    NumberArray<N, decltype(a[0] opname b[0])> returnval;                                          \
    for (std::size_t i = 0; i != N; ++i)                                                           \
      returnval[i] = a[i] opname b[i];                                                             \
                                                                                                   \
    return returnval;                                                                              \
  }                                                                                                \
  template <std::size_t N, typename T, typename T2>                                                \
  inline auto operator opname(const T & a, const NumberArray<N, T2> & b)                           \
      ->NumberArray<N, decltype(a opname b[0])>                                                    \
  {                                                                                                \
    NumberArray<N, decltype(a opname b[0])> returnval;                                             \
    for (std::size_t i = 0; i != N; ++i)                                                           \
      returnval[i] = a opname b[i];                                                                \
                                                                                                   \
    return returnval;                                                                              \
  }                                                                                                \
  template <std::size_t N, typename T, typename T2>                                                \
  inline auto operator opname(const NumberArray<N, T> & a, const T2 & b)                           \
      ->NumberArray<N, decltype(a[0] opname b)>                                                    \
  {                                                                                                \
    NumberArray<N, decltype(a[0] * b)> returnval;                                                  \
    for (std::size_t i = 0; i != N; ++i)                                                           \
      returnval[i] = a[i] opname b;                                                                \
                                                                                                   \
    return returnval;                                                                              \
  }

NumberArray_op(+, Plus)
NumberArray_op(-, Minus)
NumberArray_op(*, Multiplies)
NumberArray_op(/, Divides)


#define NumberArray_operator_binary_abab(opname, atype, btype, aarg, barg) \
template <std::size_t N, typename T, typename T2> \
inline \
NumberArray<N, bool> \
operator opname (const atype& a, const btype& b) \
{ \
  NumberArray<N, bool> returnval; \
 \
  for (std::size_t i=0; i != N; ++i) \
    returnval[i] = (aarg opname barg); \
 \
  return returnval; \
}

#define NumberArray_operator_binary(opname) \
NumberArray_operator_binary_abab(opname, NumberArray<N MacroComma T>, NumberArray<N MacroComma T2>, a[i], b[i]) \
NumberArray_operator_binary_abab(opname,                             T , NumberArray<N MacroComma T2>, a,    b[i]) \
NumberArray_operator_binary_abab(opname, NumberArray<N MacroComma T>,                             T2 , a[i], b)

NumberArray_operator_binary(<)
NumberArray_operator_binary(<=)
NumberArray_operator_binary(>)
NumberArray_operator_binary(>=)
NumberArray_operator_binary(==)
NumberArray_operator_binary(!=)
NumberArray_operator_binary(&&)
NumberArray_operator_binary(||)

template <std::size_t N, typename T>
inline
std::ostream&
operator<< (std::ostream& output, const NumberArray<N,T>& a)
{
  output << '{';
  if (N)
    output << a[0];
  for (std::size_t i=1; i<N; ++i)
    output << ',' << a[i];
  output << '}';
  return output;
}


// CompareTypes, RawType, ValueType specializations

#define NumberArray_comparisons(templatename) \
template<std::size_t N, typename T, bool reverseorder> \
struct templatename<NumberArray<N,T>, NumberArray<N,T>, reverseorder> { \
  typedef NumberArray<N, T> supertype; \
}; \
 \
template<std::size_t N, typename T, bool reverseorder> \
struct templatename<NumberArray<N,T>, NullType, reverseorder> { \
  typedef NumberArray<N, T> supertype; \
}; \
 \
template<std::size_t N, typename T, bool reverseorder> \
struct templatename<NullType, NumberArray<N,T>, reverseorder> { \
  typedef NumberArray<N, T> supertype; \
}; \
 \
template<std::size_t N, typename T, typename T2, bool reverseorder> \
struct templatename<NumberArray<N,T>, NumberArray<N,T2>, reverseorder> { \
  typedef NumberArray<N, typename Symmetric##templatename<T, T2, reverseorder>::supertype> supertype; \
}; \
 \
template<std::size_t N, std::size_t N2, typename T, typename T2, bool reverseorder> \
struct templatename<NumberArray<N,T>, NumberArray<N2,T2>, reverseorder> { \
  typedef NumberArray<0, int> supertype; \
}; \
 \
template<std::size_t N, typename T, typename T2, bool reverseorder> \
struct templatename<NumberArray<N, T>, T2, reverseorder> { \
  typedef NumberArray<N, typename Symmetric##templatename<T, T2, reverseorder>::supertype> supertype; \
}

NumberArray_comparisons(CompareTypes);
NumberArray_comparisons(PlusType);
NumberArray_comparisons(MinusType);
NumberArray_comparisons(MultipliesType);
NumberArray_comparisons(DividesType);
NumberArray_comparisons(AndType);
NumberArray_comparisons(OrType);

template <std::size_t N, typename T>
struct RawType<NumberArray<N, T> >
{
  typedef NumberArray<N, typename RawType<T>::value_type> value_type;

  static value_type value(const NumberArray<N, T>& a)
    {
      value_type returnval;
      for (std::size_t i=0; i != N; ++i)
        returnval[i] = RawType<T>::value(a[i]);
      return returnval;
    }
};

template <std::size_t N, typename T>
struct ValueType<NumberArray<N, T> >
{
  typedef typename ValueType<T>::type type;
};

} // namespace MetaPhysicL



namespace std {

using MetaPhysicL::NumberArray;
using MetaPhysicL::CompareTypes;

#define NumberArray_std_unary(funcname) \
template <std::size_t N, typename T> \
inline \
NumberArray<N, T> \
funcname (NumberArray<N, T> a) \
{ \
  for (std::size_t i=0; i != N; ++i) \
    a[i] = std::funcname(a[i]); \
 \
  return a; \
}


#define NumberArray_fl_unary(funcname) \
NumberArray_std_unary(funcname##f) \
NumberArray_std_unary(funcname##l)


#define NumberArray_stdfl_unary(funcname) \
NumberArray_std_unary(funcname) \
NumberArray_fl_unary(funcname)


#define NumberArray_std_binary_abab(funcname, atype, btype, abtypes, aarg, barg) \
template <std::size_t N, typename T, typename T2> \
inline \
typename CompareTypes<abtypes>::supertype \
funcname (const atype& a, const btype& b) \
{ \
  typedef typename CompareTypes<abtypes>::supertype TS; \
  TS returnval; \
 \
  for (std::size_t i=0; i != N; ++i) \
    returnval[i] = std::funcname(aarg, barg); \
 \
  return returnval; \
}

#define NumberArray_std_binary_aa(funcname, atype) \
template <std::size_t N, typename T> \
inline \
atype \
funcname (const atype& a, const atype& b) \
{ \
  atype returnval; \
 \
  for (std::size_t i=0; i != N; ++i) \
    returnval[i] = std::funcname(a[i], b[i]); \
 \
  return returnval; \
}


#define NumberArray_std_binary(funcname) \
NumberArray_std_binary_abab(funcname, NumberArray<N MacroComma T>, NumberArray<N MacroComma T2>, \
                            NumberArray<N MacroComma T> MacroComma NumberArray<N MacroComma T2>, a[i], b[i]) \
NumberArray_std_binary_abab(funcname,                             T , NumberArray<N MacroComma T2>, \
                            NumberArray<N MacroComma T2> MacroComma T,                              a,    b[i]) \
NumberArray_std_binary_abab(funcname, NumberArray<N MacroComma T>,                             T2 , \
                            NumberArray<N MacroComma T> MacroComma T2,                              a[i],    b) \
NumberArray_std_binary_aa(funcname, NumberArray<N MacroComma T>)


#define NumberArray_fl_binary(funcname) \
NumberArray_std_binary(funcname##f) \
NumberArray_std_binary(funcname##l)


#define NumberArray_stdfl_binary(funcname) \
NumberArray_std_binary(funcname) \
NumberArray_fl_binary(funcname)


NumberArray_std_binary(pow)
NumberArray_std_unary(exp)
NumberArray_std_unary(log)
NumberArray_std_unary(log10)
NumberArray_std_unary(sin)
NumberArray_std_unary(cos)
NumberArray_std_unary(tan)
NumberArray_std_unary(asin)
NumberArray_std_unary(acos)
NumberArray_std_unary(atan)
NumberArray_std_binary(atan2)
NumberArray_std_unary(sinh)
NumberArray_std_unary(cosh)
NumberArray_std_unary(tanh)
NumberArray_std_unary(sqrt)
NumberArray_std_unary(abs)
NumberArray_std_unary(fabs)
NumberArray_std_binary(max)
NumberArray_std_binary(min)
NumberArray_std_unary(ceil)
NumberArray_std_unary(floor)
NumberArray_std_binary(fmod)

#if __cplusplus >= 201103L
NumberArray_std_unary(llabs)
NumberArray_std_unary(imaxabs)
NumberArray_fl_unary(fabs)
NumberArray_fl_unary(exp)
NumberArray_stdfl_unary(exp2)
NumberArray_stdfl_unary(expm1)
NumberArray_fl_unary(log)
NumberArray_fl_unary(log10)
NumberArray_stdfl_unary(log2)
NumberArray_stdfl_unary(log1p)
NumberArray_fl_unary(sqrt)
NumberArray_stdfl_unary(cbrt)
NumberArray_fl_unary(sin)
NumberArray_fl_unary(cos)
NumberArray_fl_unary(tan)
NumberArray_fl_unary(asin)
NumberArray_fl_unary(acos)
NumberArray_fl_unary(atan)
NumberArray_fl_unary(sinh)
NumberArray_fl_unary(cosh)
NumberArray_fl_unary(tanh)
NumberArray_stdfl_unary(asinh)
NumberArray_stdfl_unary(acosh)
NumberArray_stdfl_unary(atanh)
NumberArray_stdfl_unary(erf)
NumberArray_stdfl_unary(erfc)
NumberArray_stdfl_unary(tgamma)
NumberArray_stdfl_unary(lgamma)
NumberArray_fl_unary(ceil)
NumberArray_fl_unary(floor)
NumberArray_stdfl_unary(trunc)
NumberArray_stdfl_unary(round)
NumberArray_stdfl_unary(nearbyint)
NumberArray_stdfl_unary(rint)

NumberArray_fl_binary(pow)
NumberArray_fl_binary(fmod)
NumberArray_stdfl_binary(remainder)
NumberArray_stdfl_binary(fmax)
NumberArray_stdfl_binary(fmin)
NumberArray_stdfl_binary(fdim)
NumberArray_stdfl_binary(hypot)
NumberArray_fl_binary(atan2)
#endif // __cplusplus >= 201103L



template <std::size_t N, typename T>
class numeric_limits<NumberArray<N, T> > :
  public MetaPhysicL::raw_numeric_limits<NumberArray<N, T>, T> {};

} // namespace std


#endif // METAPHYSICL_NUMBERARRAY_H
