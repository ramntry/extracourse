#include "SharedString.h"

// Reference count in this shared data will not
// change - this is a reentrant shared empty string
SharedString::SharedData::SharedData(char nullForSharedEmpty)
    : len(0)
    , ref(1)
{
    s[len] = nullForSharedEmpty;
}
SharedString::SharedData SharedString::sharedEmpty = '\0';

SharedString::SharedData::SharedData(size_t strLength)
    : len(strLength)
    , ref(1)
{
    s[len] = '\0';
}

void *SharedString::SharedData::operator new(size_t headerSize, size_t strLength)
{
    void *place = ::operator new(headerSize + strLength);
    return ::new(place) SharedData(strLength);
}

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

void SharedString::unshare()
{
    SharedData *tmp = new(d->len) SharedData;
    memcpy(tmp->s, d->s, d->len);
    safeDecRef();
    d = tmp;
}

char &SharedString::operator [](size_t pos)
{
    if (d->ref > 1)
        unshare();

    return d->s[pos];
}

SharedString operator +(SharedString const& left, SharedString const& right)
{
    size_t catSize = left.size() + right.size();
    SharedString::SharedData *dst = new(catSize) SharedString::SharedData;

    memcpy(dst->s, left.local_cstr(), left.size());
    memcpy(dst->s + left.size(), right.local_cstr(), right.size());

    return SharedString(dst);
}

SharedString &SharedString::operator +=(SharedString const& right)
{
    return *this = *this + right;
}

SharedString SharedString::substr(size_t pos, size_t size) const
{
    return SharedString(local_cstr() + pos, size);
}

char *SharedString::dynamic_cstr() const
{
    char *tmp = new char[size() + 1];
    memcpy(tmp, local_cstr(), size() + 1);

    return tmp;
}
