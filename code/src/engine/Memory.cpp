#include "Memory.h"

Memory::Memory()
    : value_(0.0)
    , hasValue_(false)
{}

void Memory::store(double v)
{
    value_    = v;
    hasValue_ = true;
}

void Memory::clear()
{
    value_    = 0.0;
    hasValue_ = false;
}

double Memory::recall() const
{
    return value_;
}

bool Memory::hasValue() const
{
    return hasValue_;
}

Memory& Memory::operator+=(double v)
{
    value_    += v;
    hasValue_  = true;
    return *this;
}

Memory& Memory::operator-=(double v)
{
    value_    -= v;
    hasValue_  = true;
    return *this;
}
