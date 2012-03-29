#pragma once
#include <cstring>
#include <iostream>

class SharedString
{
public:
    SharedString();
    SharedString(const char *cstr);
    SharedString(const char *src, size_t size);
    SharedString(SharedString const& src);
    SharedString &operator =(SharedString const& src);
    ~SharedString();

    char &operator [](size_t pos);
    char const& operator [](size_t pos) const;

    SharedString &operator +=(SharedString const& right);
    SharedString substr(size_t pos, size_t size) const;

    size_t size() const
    { return len; }

private:
    void init(const char *src);
    SharedString(char *_ref, size_t size);

    void unshare();
    void safeIncRef();
    void safeDecRef();

    size_t len;
    char *ref;

    static const char maxRefCounter = 3; // Debug value; Release value 127
    static char sharedEmpty[1];

friend SharedString operator +(SharedString const& left, SharedString const& right);

friend std::ostream &operator <<(std::ostream &os, SharedString const& str);
};
