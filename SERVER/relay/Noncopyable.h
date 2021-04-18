#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

struct Noncopyable
{
    Noncopyable()                              = default;
    Noncopyable(const Noncopyable&)            = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
};

#endif // NONCOPYABLE_H
