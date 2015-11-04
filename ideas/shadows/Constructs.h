#ifndef CONSTRUCTS_H
#define CONSTRUCTS_H

#include <iostream>

class Point2i
{
    public:
        int x,  y;

        Point2i(const int &X, const int &Y);
        Point2i();

        bool operator==(Point2i &other) const;
};

class Point2f
{
    public:
        float x, y;

        Point2f(const float &X, const float &Y);
        Point2f();

        bool operator==(Point2f &other) const;
};

struct Point3f
{
    public:
        float x, y, z;

        Point3f(const float &X, const float &Y, const float &Z);
        Point3f();

        bool operator==(Point3f &other) const;
};

class Position2D
{
    public:
        float x, y;
        float angle;

        Position2D(const float &X, const float &Y, const float &Angle);
        Position2D();

        bool operator==(Position2D &other) const;
};

struct Position3D
{
    float x, y, z;
    float angleX, angleY, angleZ;

    Position3D(const float &X, const float &Y, const float &Z,
               const float &AngleX, const float &AngleY, const float &AngleZ);
    Position3D();

    bool operator==(Position3D &other) const;
};

class Vec2f 
{
    public:
        float x, y;

        Vec2f();
        Vec2f(const float &X, const float &Y);

        Vec2f operator*(const float &scale) const;
        Vec2f operator/(const float &scale) const;
        Vec2f operator+(const Vec2f &other) const;
        Vec2f operator-(const Vec2f &other) const;
        Vec2f operator-() const;

        const Vec2f &operator*=(const float &scale);
        const Vec2f &operator/=(const float &scale);
        const Vec2f &operator+=(const Vec2f &other);
        const Vec2f &operator-=(const Vec2f &other);

        float magnitude() const;
        float magnitudeSquared() const;
        Vec2f normalize() const;
        float dot(const Vec2f &other) const;
        float cross(const Vec2f &other) const;
};

Vec2f operator*(const float &scale, const Vec2f &v);
std::ostream &operator<<(std::ostream &output, const Vec2f &v);

class Vec3f 
{
    private:
        float v[3];
    public:
        Vec3f();
        Vec3f(float x, float y, float z);

        float &operator[](int index);
        float operator[](int index) const;

        Vec3f operator*(float scale) const;
        Vec3f operator/(float scale) const;
        Vec3f operator+(const Vec3f &other) const;
        Vec3f operator-(const Vec3f &other) const;
        Vec3f operator-() const;

        const Vec3f &operator*=(float scale);
        const Vec3f &operator/=(float scale);
        const Vec3f &operator+=(const Vec3f &other);
        const Vec3f &operator-=(const Vec3f &other);

        float magnitude() const;
        float magnitudeSquared() const;
        Vec3f normalize() const;
        float dot(const Vec3f &other) const;
        Vec3f cross(const Vec3f &other) const;
};

void scale_to_01(Vec3f &v);

Vec3f operator*(float scale, const Vec3f &v);
std::ostream &operator<<(std::ostream &output, const Vec3f &v);

#endif
