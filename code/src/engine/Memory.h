#pragma once

/**
 * Gerencia a memória da calculadora (mc, m+, m-, mr).
 * Demonstra sobrecarga de operadores += e -= para acumular valores.
 */
class Memory {
public:
    Memory();

    void store(double v);
    void clear();
    double recall() const;
    bool hasValue() const;

    Memory& operator+=(double v);
    Memory& operator-=(double v);

private:
    double value_;
    bool   hasValue_;
};
