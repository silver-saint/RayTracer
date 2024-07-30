#include "stdlibrary.h"

int stdlib::StringLength(const char* str)
{
    int count = 0;
    while (*str != '\0') {
        count++;
        str++;
    }
    return count;
}
bool stdlib::StringCompare(const char* str1, const char* str2)
{
    const int strLen1 = StringLength(str1);
    const int strLen2 = StringLength(str2);
    if (strLen1 != strLen2) {
        return false;
    }
    while (*str1 != '\0') {
        if (*str1 != *str2) {
            return false;
        }
        str1++;
        str2++;
    }
    return true;
}
/*
std::vector<int> stdlib::BackInserter(std::vector<int> listToInsertIn, std::vector<int> list)
{
    if (list.size() == 0)
    {
        return {};
    }
    for (size_t i = 0; i < list.size(); i++)
    {
        listToInsertIn.push_back(list[i]);
    }
    return listToInsertIn;
}

*/