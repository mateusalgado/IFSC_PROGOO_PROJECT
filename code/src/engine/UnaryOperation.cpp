#include "UnaryOperation.h"
#include <stdexcept>
#include <cmath>

static constexpr double PI = M_PI;

// ── UnaryOperation ────────────────────────────────────────────────────────────

UnaryOperation::UnaryOperation(const QString& name)
    : Operation(name)
{}

double UnaryOperation::execute(double a, double /*b*/) const
{
    return apply(a);
}

// ── Trigonometric ─────────────────────────────────────────────────────────────

Trigonometric::Trigonometric(const QString& name,
                             std::function<double(double)> func,
                             const AngleUnit* angleUnit,
                             bool isInverse)
    : UnaryOperation(name)
    , func_(std::move(func))
    , angleUnit_(angleUnit)
    , isInverse_(isInverse)
{}

double Trigonometric::apply(double x) const
{
    if (!isInverse_ && angleUnit_ && *angleUnit_ == AngleUnit::DEGREES)
        x = x * PI / 180.0;

    double result = func_(x);

    if (isInverse_ && angleUnit_ && *angleUnit_ == AngleUnit::DEGREES)
        result = result * 180.0 / PI;

    return result;
}

// ── Logarithmic ───────────────────────────────────────────────────────────────

Logarithmic::Logarithmic(LogBase base)
    : UnaryOperation(base == LogBase::NATURAL ? "ln" : "log₁₀")
    , base_(base)
{}

double Logarithmic::apply(double x) const
{
    if (x <= 0.0)
        throw std::domain_error("Logaritmo de número não-positivo");
    return (base_ == LogBase::NATURAL) ? std::log(x) : std::log10(x);
}

// ── PowerUnary ────────────────────────────────────────────────────────────────

PowerUnary::PowerUnary(const QString& name, double exponent)
    : UnaryOperation(name)
    , exponent_(exponent)
{}

double PowerUnary::apply(double x) const
{
    return std::pow(x, exponent_);
}

// ── Exponential ───────────────────────────────────────────────────────────────

Exponential::Exponential() : UnaryOperation("eˣ") {}

double Exponential::apply(double x) const
{
    return std::exp(x);
}

// ── TenPower ──────────────────────────────────────────────────────────────────

TenPower::TenPower() : UnaryOperation("10ˣ") {}

double TenPower::apply(double x) const
{
    return std::pow(10.0, x);
}

// ── RootUnary ─────────────────────────────────────────────────────────────────

RootUnary::RootUnary(const QString& name, double degree)
    : UnaryOperation(name)
    , degree_(degree)
{}

double RootUnary::apply(double x) const
{
    if (x < 0.0)
        throw std::domain_error("Raiz de número negativo");
    return std::pow(x, 1.0 / degree_);
}

// ── Factorial ─────────────────────────────────────────────────────────────────

Factorial::Factorial() : UnaryOperation("x!") {}

double Factorial::apply(double x) const
{
    if (x < 0.0 || x != std::floor(x))
        throw std::domain_error("Fatorial indefinido para não-inteiros ou negativos");
    double result = 1.0;
    for (int i = 2; i <= static_cast<int>(x); ++i)
        result *= i;
    return result;
}

// ── Inverse ───────────────────────────────────────────────────────────────────

Inverse::Inverse() : UnaryOperation("1/x") {}

double Inverse::apply(double x) const
{
    if (x == 0.0)
        throw std::domain_error("Inverso de zero indefinido");
    return 1.0 / x;
}
