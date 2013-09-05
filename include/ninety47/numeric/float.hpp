#ifndef INCLUDE_NINETY47_NUMERIC_FLOAT_H_
#define INCLUDE_NINETY47_NUMERIC_FLOAT_H_

/**
 * Copyright 2013 Michael O'Keeffe.
 */

#include <cmath>
#include <limits>

namespace n47 {
namespace numeric {

template <typename T>
struct policy {
    static T zero();
    static T abs(T);
};

template <>
struct policy<float> {
    static float zero() { return 0.0f; }
    static float abs(float f) { return std::abs(f); }
};

template <>
struct policy<double> {
    static double zero() { return 0.0; }
    static double abs(double d) { return std::abs(d); }
};


template<typename T>
bool almostEqual(T a, T b, T eps) {
    T diff = policy<T>::abs(a - b);
    bool result = false;
    if ( a == b ) {
        result = true;
    } else if ( a == policy<T>::zero() ||
                b == policy<T>::zero() ||
                diff < std::numeric_limits<T>::epsilon() ) {
        result = diff < eps * std::numeric_limits<T>::epsilon();
    } else {
        result = (diff / (policy<T>::abs(a) + policy<T>::abs(b)) ) < eps;
    }
    return result;
}

}  // namespace numeric
}  // namespace n47

#endif  // INCLUDE_NINETY47_NUMERIC_FLOAT_H_

