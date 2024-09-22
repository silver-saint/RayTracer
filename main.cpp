#include "App/App.h"
#include <iostream>

struct Vector3
{
    int x, y, z;
    Vector3 operator+(const Vector3& u);
    Vector3 operator*(const Vector3& u);
    Vector3 operator-(const Vector3& u);
    Vector3 operator*(const int x);
};
int length();
int normalize();
f32 DotProduct(const Vector3& u, const Vector3& v);

bool AreOrthogonal(const Vector3& u, const Vector3& v)
{
    if (DotProduct(u, v) == 0)
    {
        return true;
    }
    return false;
}
Vector3 CrossProduct(const Vector3& v, const Vector3& u)
{
    return { v.y * u.z - v.z * u.y, v.z * u.x - v.x * u.z, v.x * u.y - v.y * u.x };
}

std::ostream& operator<<(const Vector3& u, std::ostream& stream);
bool operator==(const Vector3& u, const Vector3& v) 
{
    return (u.x == v.x && u.y == v.y && u.z == v.z);
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PWSTR pCmdLine, int nCmdShow) {
  
    Vector3 u = {2,1,3};
    Vector3 v = { 2,0,0 };
    Vector3 ResW = CrossProduct(u, v);
    Vector3 ResV = CrossProduct(v, u);
    bool areOth = AreOrthogonal(ResW, u);
    //   std::wstring strRes = std::to_wstring(result);
  //  OutputDebugString(strRes.c_str());
    //int Res = u.normalize();
    
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

f32 DotProduct(const Vector3& u, const Vector3& v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

int length(const Vector3& u)
{
    return std::sqrt((u.x * u.x + u.y * u.y + u.z * u.z));
}

int normalize(const Vector3& u)
{
    return std::sqrt(  u.x * u.x + u.y * u.y +  u.z * u.z ) / length(u);
}


std::ostream& operator<<(const Vector3& u, std::ostream& stream)
{
   return stream << u.x << ' ' << u.y << ' ' << u.z;
}
