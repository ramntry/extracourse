#include "SharedString.h"

// Reference count in this shared string will not
// change - this is a reentrant shared empty string
char SharedString::sharedEmpty[] = "\1";

SharedString::SharedString()
    : len(0)
    , ref(sharedEmpty)
{}

// len should be initialized
void SharedString::init(const char *src)
{
    if (len)
    {
        ref = new char[len + 2];
        memcpy(ref + 1, src, len);
        ref[len + 1] = '\0';
        *ref = 1;
    }
    else
        ref = sharedEmpty;
}

SharedString::SharedString(const char *cstr)
    : len(strlen(cstr))
{
    init(cstr);
}

SharedString::SharedString(const char *src, size_t size)
    : len(size)
{
    init(src);
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

    safeDecRef();
    ref = src.ref;
    len = src.len;

    safeIncRef();

    if (*tmp == 0)
        delete[] tmp;

    return *this;
}

SharedString::~SharedString()
{
    safeDecRef();

    if (*ref == 0)
        delete[] ref;
}

// Responsible for the decrement the reference count in
// the source-string lies in the calling code
void SharedString::unshare()
{
    char *tmp = new char[len + 2];
    memcpy(tmp + 1, ref + 1, len + 1);

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

void SharedString::safeDecRef()
{
    if (ref != sharedEmpty)
        --*ref;
}

char const& SharedString::operator [](size_t pos) const
{
    return ref[pos + 1];
}

char &SharedString::operator [](size_t pos)
{
    if (*ref > 1) // change to *ref != 1 if you want to set
    {             // ... maxRefCounter at 255
        safeDecRef();
        unshare();
    }

    return ref[pos + 1];
}

SharedString operator +(SharedString const& left, SharedString const& right)
{
    size_t catLen = left.len + right.len;
    char *dst = new char[catLen + 2];

    memcpy(dst + 1, left.ref + 1, left.len);
    memcpy(dst + left.len + 1, right.ref + 1, right.len + 1);

/* DEBUG */ std::cout << "[ DEBUG: memcpy() in operator +(...) ]" << std::endl;

    return SharedString(dst, catLen);
}

SharedString &SharedString::operator +=(SharedString const& right)
{
    return *this = *this + right;
}

SharedString SharedString::substr(size_t pos, size_t size) const
{
    return SharedString((const char *)ref + pos + 1, size);
}

const char *SharedString::local_cstr() const
{
    return ref + 1;
}

char *SharedString::dynamic_cstr() const
{
    char *tmp = new char[len + 1];
    memcpy(tmp, ref + 1, len + 1);

    return tmp;
}

std::ostream &operator <<(std::ostream &os, SharedString const& str)
{
    return os.write(str.ref + 1, str.len);
}
