#pragma once
#include <cstring>
#include <string>
#include <iostream>

class SharedString
{
public:
    SharedString();
    SharedString(const char *cstr);
    SharedString(const char *src, size_t size);
    SharedString(std::string const& src);
    SharedString(SharedString const& src);
    SharedString &operator =(SharedString const& src);
    ~SharedString();

    char &operator [](size_t pos);
    char const& operator [](size_t pos) const;

    SharedString &operator +=(SharedString const& right);
    SharedString substr(size_t pos, size_t size) const;

    size_t size() const
    { return len; }

    // The returned pointer becomes a dangling when you delete the object
    const char *local_cstr() const;

    // Responsible for freeing the memory allocated for the returned
    // object lying on the calling code
    char *dynamic_cstr() const;

    operator std::string() const;

private:
#pragma pack(push, 1)
    class SharedData
    {
    public:
        size_t ref;
        size_t len;
        char s[1];

        SharedData() {}
        SharedData(char nullForSharedEmpty);

        void *operator new(size_t headerSize, size_t strLength);
        void operator delete(void *p);

    private:
        SharedData(size_t strLength);
    };
#pragma pack(pop)

    void init(const char *src);
    SharedString(char *_ref, size_t size);

    void unshare();
    void safeIncRef();
    void safeDecRef();

    size_t len;
    char *ref;

    static const char maxRefCounter = 3; // Debug value; Release value 127
    static char sharedEmpty[2];

friend SharedString operator +(SharedString const& left, SharedString const& right);

friend std::ostream &operator <<(std::ostream &os, SharedString const& str);
};
