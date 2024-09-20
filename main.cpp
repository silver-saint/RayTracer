#include "App/App.h"
#include <iostream>

struct Vector3
{
    int x, y, z;
    Vector3 operator+(const Vector3& u);
    Vector3 operator*(const Vector3& u);
    Vector3 operator-(const Vector3& u);
    Vector3 operator*(const int x);
    int length();
    int normalize();
};
std::ostream& operator<<(const Vector3& u, std::ostream& stream);
bool operator==(const Vector3& u, const Vector3& v) 
{
    return (u.x == v.x && u.y == v.y && u.z == v.z);
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PWSTR pCmdLine, int nCmdShow) {
    Vector3 u = {-1,3,4};
    Vector3 v = { 1,2,3 };
    int Res = u.normalize();
    /*
    dx::engine::App app;
    return EXIT_SUCCESS;
    */
}

Vector3 Vector3::operator+(const Vector3& u)
{
    return { x + u.x, y + u.y, z + u.z };
}

Vector3 Vector3::operator*(const Vector3& u)
{
    return { x * u.x, y * u.y, z * u.z };
}

Vector3 Vector3::operator-(const Vector3& u)
{
    return { x - u.x, y - u.y, z - u.z };
}

Vector3 Vector3::operator*(const int scalar)
{
    return { x * scalar, y * scalar, z * scalar };
}

int Vector3::length()
{
    return std::sqrt((x * x + y * y + z * z));
}

int Vector3::normalize()
{
    return std::sqrt(  x * x + y * y +  z * z ) / length();
}


std::ostream& operator<<(const Vector3& u, std::ostream& stream)
{
   return stream << u.x << ' ' << u.y << ' ' << u.z;
}
