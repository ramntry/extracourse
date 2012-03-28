#pragma once

#include <iostream>
#include "Stack.h"

bool const release = false;  // true -> supressing debug messages

template <class T>
class StretchableStack : public DynamicStack<T>
{
public:
    StretchableStack(int capacity = defaultStackCapacity);
    ~StretchableStack();

    void push(T value);
    T pop();
    T const& top();

    bool isEmpty() const;

    int size() const
    { return accumulate(&StackBlock::length); }

    void truncate();

    int capacity() const
    { return accumulate(&StackBlock::capacity); }

private:
    int accumulate(int StretchableStack::StackBlock::*field) const;

    struct StackBlock
    {
        StackBlock(int newCapacity, StackBlock *prevBl);
        ~StackBlock();

        T *array;

        int length;
        int capacity;

        StackBlock *next;
        StackBlock *prev;
    };

    StackBlock *mHead;
    StackBlock *mHigh;

public:
    class Iterator
    {
    public:
        Iterator(StretchableStack<T> &s)
            : block(s.mHead)
            , nextPos(0)
        {}

        void toTop(StretchableStack<T> &s);
        bool hasNext();

        T &next()
        { return block->array[nextPos++]; }

        T &getNext()
        { return block->array[nextPos]; }

        bool operator !=(Iterator const& right)
        { return nextPos != right.nextPos || block != right.block; }

    private:
        StackBlock *block;
        int nextPos;
    };

    static const int defaultStackCapacity = 2;
    static const int capacityMultiplier = 2;
};


template <class T>
StretchableStack<T>::StackBlock::StackBlock(int newCapacity, StackBlock *prevBl) :
    array(new T[newCapacity]),
    length(0),
    capacity(newCapacity),
    next(NULL),
    prev(prevBl)
{}

template <class T>
StretchableStack<T>::StackBlock::~StackBlock()
{
    delete[] array;
    delete next;
}

template <class T>
StretchableStack<T>::StretchableStack(int capacity) :
    mHead(new StackBlock(capacity, NULL)),
    mHigh(mHead)
{}

template <class T>
StretchableStack<T>::~StretchableStack()
{
    delete mHead;
}

template <class T>
void StretchableStack<T>::truncate()
{
    delete mHigh->next;
    mHigh->next = 0;
}

template <class T>
void StretchableStack<T>::Iterator::toTop(StretchableStack<T> &s)
{
    block = s.mHigh;
    nextPos = block->length;
}

template <class T>
bool StretchableStack<T>::Iterator::hasNext()
{
    if (nextPos == block->length && block->next)
    {
        block = block->next;
        nextPos = 0;
    }

    if (nextPos == block->length)
        return false;
    return true;
}

template <class T>
void StretchableStack<T>::push(T value)
{
    if (mHigh->length == mHigh->capacity)
    {
        if (mHigh->next == NULL)
        {
            mHigh->next = new StackBlock(mHigh->capacity * capacityMultiplier, mHigh);
            release || std::cout << "alloc new block in StretchableStack" << std::endl;
        } else
            release || std::cout << "reuse block in StretchableStack" << std::endl;

        mHigh = mHigh->next;
    }
    mHigh->array[mHigh->length++] = value;
}

template <class T>
T StretchableStack<T>::pop()
{
    if (mHigh->length == 0)
        mHigh = mHigh->prev;

    if (mHigh == NULL)
        throw new StackUnderflowException();

    return mHigh->array[--mHigh->length];
}

template <class T>
T const& StretchableStack<T>::top()
{
    StackBlock *high = mHigh;
    if (high->length == 0)
        high = high->prev;

    if (high == NULL)
        throw new StackUnderflowException();

    return high->array[high->length - 1];
}

template <class T>
int StretchableStack<T>::accumulate(int StretchableStack::StackBlock::*field) const
{
    int accumulator = 0;
    StackBlock *current = mHead;
    while (current != NULL)
    {
        accumulator += current->*field;
        current = current->next;
    }
    return accumulator;
}

template <class T>
bool StretchableStack<T>::isEmpty() const
{
    return mHigh->length == 0 && mHigh->prev == NULL;
}
