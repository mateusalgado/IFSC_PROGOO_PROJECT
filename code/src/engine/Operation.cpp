#include "Operation.h"

Operation::Operation(const QString& name)
    : name_(name)
{}

QString Operation::getName() const
{
    return name_;
}
