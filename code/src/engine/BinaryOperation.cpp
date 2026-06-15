#include "BinaryOperation.h"
#include <stdexcept>
#include <cmath>

// ── BinaryOperation ───────────────────────────────────────────────────────────

BinaryOperation::BinaryOperation(const QString& name)
    : Operation(name)
{}

// ── Addition ──────────────────────────────────────────────────────────────────

Addition::Addition() : BinaryOperation("+") {}

double Addition::execute(double a, double b) const
{
    return a + b;
}

// ── Subtraction ───────────────────────────────────────────────────────────────

Subtraction::Subtraction() : BinaryOperation("-") {}

double Subtraction::execute(double a, double b) const
{
    return a - b;
}

// ── Multiplication ────────────────────────────────────────────────────────────

Multiplication::Multiplication() : BinaryOperation("×") {}

double Multiplication::execute(double a, double b) const
{
    return a * b;
}

// ── Division ──────────────────────────────────────────────────────────────────

Division::Division() : BinaryOperation("÷") {}

double Division::execute(double a, double b) const
{
    if (b == 0.0)
        throw std::domain_error("Divisão por zero");
    return a / b;
}

// ── Power ─────────────────────────────────────────────────────────────────────

Power::Power() : BinaryOperation("xʸ") {}

double Power::execute(double a, double b) const
{
    return std::pow(a, b);
}

// ── NthRoot ───────────────────────────────────────────────────────────────────

NthRoot::NthRoot() : BinaryOperation("ʸ√x") {}

double NthRoot::execute(double a, double b) const
{
    if (b == 0.0)
        throw std::domain_error("Grau de raiz zero");
    if (a < 0.0 && std::fmod(b, 2.0) == 0.0)
        throw std::domain_error("Raiz par de número negativo");
    return std::pow(a, 1.0 / b);
}
