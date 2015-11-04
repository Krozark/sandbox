#include "Constructs.h"

#include <cmath>

// Point2i Implementation

Point2i::Point2i()
{
}

Point2i::Point2i(const int &X, const int &Y): x(X), y(Y)
{
}

bool Point2i::operator==(Point2i &other) const
{
    if(x == other.x && y == other.y)
        return true;

    return false;
}

// Point2f Implementation

Point2f::Point2f()
{
}

Point2f::Point2f(const float &X, const float &Y): x(X), y(Y)
{
}

bool Point2f::operator==(Point2f &other) const
{
    if(x == other.x && y == other.y)
        return true;

    return false;
}

// Point2f Implementation

Point3f::Point3f()
{
}

Point3f::Point3f(const float &X, const float &Y, const float &Z) : x(X), y(Y), z(Z)
{
}

bool Point3f::operator==(Point3f &other) const
{
    if(x == other.x && y == other.y && z == other.z)
        return true;

    return false;
}

// Position2D Implementation

Position2D::Position2D()
{
}

Position2D::Position2D(const float &X, const float &Y, const float &Angle) : x(X), y(Y), angle(Angle)
{
}

bool Position2D::operator==(Position2D &other) const
{
    if(x == other.x && y == other.y && angle == other.angle)
        return true;

    return false;
}

// Position3D Implementation

Position3D::Position3D()
{
}

Position3D::Position3D(const float &X, const float &Y, const float &Z,
                       const float &AngleX, const float &AngleY, const float &AngleZ) :
    x(X),
    y(Y),
    z(Z),
    angleX(AngleX),
    angleY(AngleY),
    angleZ(AngleZ)
{
}

bool Position3D::operator==(Position3D &other) const
{
    if(x == other.x && y == other.y && z == other.z && 
       angleX == other.angleX  && angleY == other.angleY  && angleZ == other.angleZ)
        return true;

    return false;
}

Vec2f::Vec2f() 
{
}

Vec2f::Vec2f(const float &X, const float &Y)
{
    x = X;
    y = Y;
}

Vec2f Vec2f::operator*(const float &scale) const 
{
    return Vec2f(x * scale, y * scale);
}

Vec2f Vec2f::operator/(const float &scale) const 
{
    return Vec2f(x / scale, y / scale);
}


Vec2f Vec2f::operator+(const Vec2f &other) const 
{
    return Vec2f(x + other.x, y + other.y);
}

Vec2f Vec2f::operator-(const Vec2f &other) const 
{
    return Vec2f(x - other.x, y - other.y);
}

Vec2f Vec2f::operator-() const {
    return Vec2f(-x, -y);
}

const Vec2f &Vec2f::operator*=(const float &scale) 
{
    x *= scale;
    y *= scale;
    return *this;
}

const Vec2f &Vec2f::operator/=(const float &scale) 
{
    x /= scale;
    y /= scale;
    return *this;
}

const Vec2f &Vec2f::operator+=(const Vec2f &other) 
{
    x += other.x;
    y += other.y;
    return *this;
}

const Vec2f &Vec2f::operator-=(const Vec2f &other) 
{
    x -= other.x;
    y -= other.y;
    return *this;
}

float Vec2f::magnitude() const 
{
    return sqrt(x * x + y * y);
}

float Vec2f::magnitudeSquared() const 
{
    return x * x + y * y;
}

Vec2f Vec2f::normalize() const 
{
    float m = sqrt(x * x + y * y);
    return Vec2f(x / m, y / m);
}

float Vec2f::dot(const Vec2f &other) const 
{
    return x * other.x + y * other.y;
}

float Vec2f::cross(const Vec2f &other) const 
{
    return x * other.y - y * other.x;
}

Vec2f operator*(const float &scale, const Vec2f &v) 
{
    return v * scale;
}

std::ostream &operator<<(std::ostream &output, const Vec2f &v)
{
    std::cout << '(' << v.x << ", " << v.y << ')';
    return output;
}

Vec3f::Vec3f() 
{
}

Vec3f::Vec3f(float x, float y, float z)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

float &Vec3f::operator[](int index) 
{
    return v[index];
}

float Vec3f::operator[](int index) const 
{
    return v[index];
}

Vec3f Vec3f::operator*(float scale) const 
{
    return Vec3f(v[0] * scale, v[1] * scale, v[2] * scale);
}

Vec3f Vec3f::operator/(float scale) const 
{
    return Vec3f(v[0] / scale, v[1] / scale, v[2] / scale);
}

Vec3f Vec3f::operator+(const Vec3f &other) const 
{
    return Vec3f(v[0] + other.v[0], v[1] + other.v[1], v[2] + other.v[2]);
}

Vec3f Vec3f::operator-(const Vec3f &other) const 
{
    return Vec3f(v[0] - other.v[0], v[1] - other.v[1], v[2] - other.v[2]);
}

Vec3f Vec3f::operator-() const {
    return Vec3f(-v[0], -v[1], -v[2]);
}

const Vec3f &Vec3f::operator*=(float scale) 
{
    v[0] *= scale;
    v[1] *= scale;
    v[2] *= scale;
    return *this;
}

const Vec3f &Vec3f::operator/=(float scale) 
{
    v[0] /= scale;
    v[1] /= scale;
    v[2] /= scale;
    return *this;
}

const Vec3f &Vec3f::operator+=(const Vec3f &other) 
{
    v[0] += other.v[0];
    v[1] += other.v[1];
    v[2] += other.v[2];
    return *this;
}

const Vec3f &Vec3f::operator-=(const Vec3f &other) 
{
    v[0] -= other.v[0];
    v[1] -= other.v[1];
    v[2] -= other.v[2];
    return *this;
}

float Vec3f::magnitude() const 
{
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

float Vec3f::magnitudeSquared() const 
{
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

Vec3f Vec3f::normalize() const 
{
    float m = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    return Vec3f(v[0] / m, v[1] / m, v[2] / m);
}

float Vec3f::dot(const Vec3f &other) const 
{
    return v[0] * other.v[0] + v[1] * other.v[1] + v[2] * other.v[2];
}

Vec3f Vec3f::cross(const Vec3f &other) const 
{
    return Vec3f(v[1] * other.v[2] - v[2] * other.v[1],
                 v[2] * other.v[0] - v[0] * other.v[2],
                 v[0] * other.v[1] - v[1] * other.v[0]);
}

Vec3f operator*(float scale, const Vec3f &v) 
{
    return v * scale;
}

void scale_to_01(Vec3f &v)
{
    v.normalize();
    v = v*.5 + Vec3f(0.5f, 0.5f, 0.5f);
}

std::ostream &operator<<(std::ostream &output, const Vec3f &v)
{
    std::cout << '(' << v[0] << ", " << v[1] << ", " << v[2] << ')';
    return output;
}
