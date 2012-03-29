#include "SharedString.h"

SharedString::SharedString(const char *cstr)
    : len(strlen(cstr))
    , ref(new char[len + 1])
{
    *ref = 1;
    memcpy(ref + 1, cstr, len);
}

SharedString::SharedString(const char *src, size_t size)
    : len(size)
    , ref(new char[len + 1])
{
    *ref = 1;
    memcpy(ref + 1, src, len);
}

SharedString::SharedString(char *_ref, size_t size)
    : len(size)
    , ref(_ref)
{
    *ref = 1;
}

SharedString::SharedString(const SharedString &src)
    : len(src.len)
    , ref(src.ref)
{
    safeIncRef();
}

// Think long and hard before changing the code for this method
SharedString &SharedString::operator =(SharedString const& src)
{
    char *tmp = ref;

    --*ref;
    ref = src.ref;
    len = src.len;

    safeIncRef();

    if (*tmp == 0)
        delete[] tmp;

    return *this;
}

SharedString::~SharedString()
{
    if (--*ref == 0)
        delete[] ref;
}

// Responsible for the decrement the reference count in
// the source-string lies in the calling code
void SharedString::unshare()
{
    char *tmp = new char[len + 1];
    memcpy(tmp + 1, ref + 1, len);

    *tmp = 1;
    ref = tmp;

/* DEBUG */ std::cout << "[ DEBUG: memcpy() in unshare() ]" << std::endl;
}

void SharedString::safeIncRef()
{
    if (*ref == maxRefCounter)
        unshare();
    else
        ++*ref;
}

char const& SharedString::operator [](size_t pos) const
{
    return ref[pos + 1];
}

char &SharedString::operator [](size_t pos)
{
    if (*ref > 1) // change to *ref != 1 if you want to set
    {             // ... maxRefCounter at 255
        --*ref;
        unshare();
    }

    return ref[pos + 1];
}

SharedString operator +(SharedString const& left, SharedString const& right)
{
    size_t catLen = left.len + right.len;
    char *dst = new char[catLen + 1];

    memcpy(dst + 1, left.ref + 1, left.len);
    memcpy(dst + left.len + 1, right.ref + 1, right.len);

/* DEBUG */ std::cout << "[ DEBUG: memcpy() in operator +(...) ]" << std::endl;

    return SharedString(dst, catLen);
}

SharedString &SharedString::operator +=(SharedString const& right)
{
    return *this = *this + right;
}

SharedString SharedString::substr(size_t pos, size_t size)
{
    return SharedString((const char *)ref + pos + 1, size);
}

std::ostream &operator <<(std::ostream &os, SharedString const& str)
{
    return os.write(str.ref + 1, str.len);
}
