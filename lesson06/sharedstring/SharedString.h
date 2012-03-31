#pragma once
#include <cstring>
#include <string>
#include <iostream>

class SharedString
{
public:
    SharedString()
    : d(&sharedEmpty) {}

    SharedString(const char *cstr)
    { init(cstr, strlen(cstr)); }

    SharedString(const char *src, size_t size)
    { init(src, size); }

    SharedString(std::string const& src)
    { init(src.c_str(), src.size()); }

    SharedString(SharedString const& src)
    : d(src.d) { d->ref++; }

    SharedString &operator =(SharedString const& src);
    ~SharedString();

    operator std::string() const
    { return std::string(local_cstr()); }

    // The returned pointer becomes a dangling when you delete the object
    const char *local_cstr() const
    { return d->s; }

    // Responsible for freeing the memory allocated for the returned
    // object lying on the calling code
    char *dynamic_cstr() const;

    size_t size() const
    { return d->len; }

    char const& operator [](size_t pos) const
    { return d->s[pos]; }

    char &operator [](size_t pos);
    SharedString substr(size_t pos, size_t size) const;
    SharedString &operator +=(SharedString const& right);

private:
#pragma pack(push, 1)
    class SharedData
    {
    public:
        size_t len;
        int ref;
        char s[1];

        SharedData() {}
        SharedData(char nullForSharedEmpty);

        void *operator new(size_t headerSize, size_t strLength);

        void operator delete(void *p)
        { ::operator delete(p); }

    private:
        SharedData(size_t strLength);
    };
#pragma pack(pop)

    SharedData *d;
    static SharedData sharedEmpty;

    void init(const char *src, size_t length);
    SharedString(SharedData *data)
        : d(data)
    {}

    void safeDecRef()
    { if (d != &sharedEmpty) d->ref--; }

    void unshare();

friend SharedString operator +(SharedString const& left, SharedString const& right);

friend std::ostream &operator <<(std::ostream &os, SharedString const& str)
{
    return os.write(str.local_cstr(), str.size());
}

};
