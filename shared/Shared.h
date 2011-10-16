#ifndef Shared_h
#define Shared_h

#include <string>
#include <stdlib.h>

static inline bool parseLocation(const std::string &string,
                                 std::string &file, unsigned &line, unsigned &col)
{
    file = string;
    size_t colon = file.find_last_of(':');
    if (colon == std::string::npos)
        return false;
    col = atoi(string.c_str() + colon + 1);
    if (!col)
        return false;
    file.resize(colon);
    colon = file.find_last_of(':');
    if (colon == std::string::npos)
        return false;
    line = atoi(string.c_str() + colon + 1);
    if (!line)
        return false;
    file.resize(colon);
    return true;
}
#endif
