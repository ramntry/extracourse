#pragma once

template <class T>
class Stack
{
public:
    virtual ~Stack() {}

    virtual void push(T value) = 0;
    virtual T pop() = 0;

    virtual T const& top() const = 0;

    virtual bool isEmpty() const = 0;
    virtual int size() const = 0;
};

class StackUnderflowException {};
class StackOverflowException {};


template <class T>
class DynamicStack : public Stack<T>
{
public:
    virtual int capacity() const = 0;
    virtual void truncate() = 0;
};
