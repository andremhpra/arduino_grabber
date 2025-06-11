#ifndef VECTOR2_H
#define VECTOR2_H

#include <Arduino.h>

template <typename T>
class Vector2
{
public:
    T x, y;

    // Constructors
    Vector2() : x(0), y(0) {} // default.
    Vector2(T _x, T _y) : x(_x), y(_y) {}

    // Equality operator
    bool operator==(const Vector2<T> &other) const
    {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Vector2<T> &other) const
    {
        return x != other.x || y != other.y;
    }

    bool is_zero()
    {
        return x == 0 and y == 0;
    }

    String to_string()
    {
        return "(" + String(x) + ", " + String(y) + ")";
    }
};

#endif
