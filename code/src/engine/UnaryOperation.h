#pragma once
#include "Operation.h"
#include <functional>
#include <cmath>

enum class AngleUnit { RADIANS, DEGREES };
enum class LogBase   { NATURAL, TEN };

/**
 * Classe abstrata intermediária para operações unárias (um operando).
 * Implementa execute() delegando para o método abstrato apply().
 */
class UnaryOperation : public Operation {
public:
    explicit UnaryOperation(const QString& name);
    double execute(double a, double b = 0.0) const override final;

    virtual double apply(double x) const = 0;
};

// ── Subclasses concretas ──────────────────────────────────────────────────────

/**
 * Encapsula funções trigonométricas e hiperbólicas via std::function.
 * Aplica conversão de graus para radianos quando necessário.
 */
class Trigonometric : public UnaryOperation {
public:
    Trigonometric(const QString& name,
                  std::function<double(double)> func,
                  const AngleUnit* angleUnit,
                  bool isInverse = false);

    double apply(double x) const override;

private:
    std::function<double(double)> func_;
    const AngleUnit* angleUnit_;
    bool isInverse_;
};

/** Logaritmo natural (ln) ou na base 10 (log₁₀) */
class Logarithmic : public UnaryOperation {
public:
    explicit Logarithmic(LogBase base);
    /** @throws std::domain_error quando x <= 0 */
    double apply(double x) const override;

private:
    LogBase base_;
};

/** Potência de expoente fixo: x², x³, eˣ, 10ˣ */
class PowerUnary : public UnaryOperation {
public:
    PowerUnary(const QString& name, double exponent);
    double apply(double x) const override;

private:
    double exponent_;
};

/** Exponencial com base fixa: eˣ */
class Exponential : public UnaryOperation {
public:
    Exponential();
    double apply(double x) const override;
};

/** 10 elevado a x */
class TenPower : public UnaryOperation {
public:
    TenPower();
    double apply(double x) const override;
};

/** Raiz de grau fixo: ²√x, ³√x */
class RootUnary : public UnaryOperation {
public:
    RootUnary(const QString& name, double degree);
    /** @throws std::domain_error quando x < 0 e degree for par */
    double apply(double x) const override;

private:
    double degree_;
};

/** x! (fatorial) — aceita apenas inteiros não-negativos */
class Factorial : public UnaryOperation {
public:
    Factorial();
    /** @throws std::domain_error quando x < 0 ou não é inteiro */
    double apply(double x) const override;
};

/** 1/x (inverso multiplicativo) */
class Inverse : public UnaryOperation {
public:
    Inverse();
    /** @throws std::domain_error quando x == 0 */
    double apply(double x) const override;
};
