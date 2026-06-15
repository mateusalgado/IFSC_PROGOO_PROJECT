#pragma once
#include "Operation.h"

/**
 * Classe abstrata intermediária para operações binárias (dois operandos).
 * Herda de Operation e serve de base para +, -, ×, ÷, xʸ e ʸ√x.
 */
class BinaryOperation : public Operation {
public:
    explicit BinaryOperation(const QString& name);
    double execute(double a, double b = 0.0) const override = 0;
};

// ── Subclasses concretas ──────────────────────────────────────────────────────

class Addition : public BinaryOperation {
public:
    Addition();
    double execute(double a, double b = 0.0) const override;
};

class Subtraction : public BinaryOperation {
public:
    Subtraction();
    double execute(double a, double b = 0.0) const override;
};

class Multiplication : public BinaryOperation {
public:
    Multiplication();
    double execute(double a, double b = 0.0) const override;
};

class Division : public BinaryOperation {
public:
    Division();
    /** @throws std::domain_error quando b == 0 */
    double execute(double a, double b = 0.0) const override;
};

/** a elevado a b */
class Power : public BinaryOperation {
public:
    Power();
    double execute(double a, double b = 0.0) const override;
};

/** Raiz b de a (b-ésima raiz de a) */
class NthRoot : public BinaryOperation {
public:
    NthRoot();
    /** @throws std::domain_error quando b == 0 ou resultado é imaginário */
    double execute(double a, double b = 0.0) const override;
};
