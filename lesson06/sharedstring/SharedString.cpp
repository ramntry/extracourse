#include "SharedString.h"

// Reference count in this shared data will not
// change - this is a reentrant shared empty string
SharedString::SharedData SharedString::sharedEmpty = '\0';

SharedString::SharedData::SharedData(char nullForSharedEmpty)
    : len(0)
    , ref(1)
{ s[len] = nullForSharedEmpty; }

SharedString::SharedData::SharedData(size_t strLength)
    : len(strLength)
    , ref(1)
{ s[len] = '\0'; }

void *SharedString::SharedData::operator new(size_t headerSize, size_t strLength)
{
    void *place = ::operator new(headerSize + strLength);
    return ::new(place) SharedData(strLength);
}

void SharedString::SharedData::operator delete(void *p)
{
    ::operator delete(p);
}

SharedString::SharedString()
    : d(&sharedEmpty)
{}

void SharedString::init(const char *src, size_t length)
{
    if (length)
    {
        d = new(length) SharedData;
        memcpy(d->s, src, length);
    }
    else
        d = &sharedEmpty;
}

SharedString::SharedString(const char *cstr)
{
    init(cstr, strlen(cstr));
}

SharedString::SharedString(const char *src, size_t size)
{
    init(src, size);
}

SharedString::SharedString(std::string const& src)
{
    init(src.c_str(), src.size());
}

/*
SharedString::SharedString(char *_ref, size_t size)
    : len(size)
    , ref(_ref)
{
    *ref = 1;
}
*/

SharedString::SharedString(const SharedString &src)
    : d(src.d)
{
    d->ref++;
}

// Think long and hard before changing the code for this method
SharedString &SharedString::operator =(SharedString const& src)
{
    SharedData *tmp = d;

    safeDecRef();
    d = src.d;
    d->ref++;

    if (tmp->ref == 0)
        delete tmp;

    return *this;
}

SharedString::~SharedString()
{
    safeDecRef();

    if (d->ref == 0)
        delete d;
}

// Responsiblity for the decrement the reference count in
// the source-string lies in the calling code
void SharedString::unshare()
{
    SharedData *tmp = new(d->len) SharedData;
    memcpy(tmp->s, d->s, d->len);
    d = tmp;

/* DEBUG */ std::cout << "[ DEBUG: memcpy() in unshare() ]" << std::endl;
}

void SharedString::safeDecRef()
{
    if (d != &sharedEmpty)
        d->ref--;
}

char const& SharedString::operator [](size_t pos) const
{
    return d->s[pos];
}

char &SharedString::operator [](size_t pos)
{
    if (d->ref > 1)
    {
        safeDecRef();
        unshare();
    }

    return d->s[pos];
}

/*
SharedString operator +(SharedString const& left, SharedString const& right)
{
    size_t catLen = left.len + right.len;
    char *dst = new char[catLen + 2];

    memcpy(dst + 1, left.ref + 1, left.len);
    memcpy(dst + left.len + 1, right.ref + 1, right.len + 1);

/* DEBUG *//* std::cout << "[ DEBUG: memcpy() in operator +(...) ]" << std::endl;

    return SharedString(dst, catLen);
}

SharedString &SharedString::operator +=(SharedString const& right)
{
    return *this = *this + right;
}
*/

SharedString SharedString::substr(size_t pos, size_t size) const
{
    return SharedString((const char *)d->s + pos, size);
}

const char *SharedString::local_cstr() const
{
    return d->s;
}

char *SharedString::dynamic_cstr() const
{
    char *tmp = new char[d->len + 1];
    memcpy(tmp, d->s, d->len + 1);

    return tmp;
}

SharedString::operator std::string() const
{
    return std::string(local_cstr());
}

std::ostream &operator <<(std::ostream &os, SharedString const& str)
{
    return os.write(str.d->s, str.d->len);
}
