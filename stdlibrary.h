#pragma once

namespace stdlib
{
    //functions
    int StringLength(const char* str);
    bool StringCompare(const char* str1, const char* str2);
    //Data structures

    /*
    template<typename T>
    class optional
    {
    public:
        optional() = default;
        optional(T data);
        bool HasValue();
    private:
        T* Data;
    };
    template<typename T>
    inline optional<T>::optional(T data)
    {
        *Data = data;
    }
    template<typename T>
    inline bool optional<T>::HasValue()
    {
        if (Data)
        {
            return true;
        }
        return false;
    }
    */
}
