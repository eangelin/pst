#pragma once

#ifndef PST_TEST_TRACER_H__
#define PST_TEST_TRACER_H__

#include <ostream>

template <class T> struct tracer
{
  tracer(T const& t) : t(t) { ++lref_t_constr; }
  tracer(T&& t) : t(t) { ++rref_t_constr; }

  tracer(tracer const& o) : t(o.t) { ++lref_constr; }
  tracer(tracer&& o) : t(o.t) { ++rref_constr; }

  tracer& operator=(T const& t) { this->t = t; ++lref_t_copy; return *this; }
  tracer& operator=(T&& t) { this->t = t; ++rref_t_copy; return *this; }

  tracer& operator=(tracer const& o) { this->t = o.t; ++lref_copy; return *this; }
  tracer& operator=(tracer&& o) { this->t = o.t; ++rref_copy; return *this; }

  friend bool operator<(tracer const& lhs, tracer const& rhs) {
    ++lt;
    return lhs.t < rhs.t;
  }

  friend bool operator==(tracer const& lhs, tracer const& rhs) {
    ++eq;
    return lhs.t == rhs.t;
  }

  friend bool operator!=(tracer const& lhs, tracer const& rhs) {
    return !(lhs == rhs);
  }

  friend std::ostream& operator<<(std::ostream& os, tracer const& t) {
    os << t.t;
    return os;
  }

  ~tracer() { ++destr; }

  static int lref_t_constr;
  static int rref_t_constr;

  static int lref_constr;
  static int rref_constr;

  static int lref_t_copy;
  static int rref_t_copy;

  static int lref_copy;
  static int rref_copy;

  static int lt;
  static int eq;

  static int destr;


  static void report(std::ostream& s) {
    std::string ttype = typeid(T).name();
    std::string type = typeid(tracer).name();
    s << "tracer(" << ttype <<" const&): " << lref_t_constr << std::endl;
    s << "tracer(" << ttype <<"&&): " << rref_t_constr << std::endl;
    s << "tracer(" << type <<" const&): " << lref_constr << std::endl;
    s << "tracer(" << type <<"&&): " << rref_constr << std::endl;
    s << "tracer& operator=(" << ttype <<" const&): " << lref_t_copy << std::endl;
    s << "tracer& operator=(" << ttype <<"&&): " << rref_t_copy << std::endl;
    s << "tracer& operator=(" << type <<" const&): " << lref_copy << std::endl;
    s << "tracer& operator=(" << type <<"&&): " << rref_copy << std::endl;
    s << "bool operator<(" << type <<" const&, " << type << " const&): " << lt << std::endl;
    s << "bool operator==(" << type <<" const&, " << type << " const&): " << eq << std::endl;
  }

  static void reset() {
    lref_t_constr = 0;
    rref_t_constr = 0;
    lref_constr = 0;
    rref_constr = 0;
    lref_t_copy = 0;
    rref_t_copy = 0;
    lref_copy = 0;
    rref_copy = 0;
    lt = 0;
    eq = 0;
    destr = 0;
  }

  T t;
};

template <class T> int tracer<T>::lref_t_constr;
template <class T> int tracer<T>::rref_t_constr;
template <class T> int tracer<T>::lref_constr;
template <class T> int tracer<T>::rref_constr;
template <class T> int tracer<T>::lref_t_copy;
template <class T> int tracer<T>::rref_t_copy;
template <class T> int tracer<T>::lref_copy;
template <class T> int tracer<T>::rref_copy;
template <class T> int tracer<T>::lt;
template <class T> int tracer<T>::eq;
template <class T> int tracer<T>::destr;

#endif // PST_TEST_TRACER_H__
