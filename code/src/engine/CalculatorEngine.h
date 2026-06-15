#pragma once
#include "Operation.h"
#include "Memory.h"
#include "UnaryOperation.h"

#include <QString>
#include <map>
#include <stack>
#include <vector>
#include <memory>

/**
 * Motor central da calculadora.
 *
 * Mantém o estado da máquina (operando atual, operação pendente, memória,
 * unidade de ângulo e modo 2nd). Todas as operações são despachadas
 * polimorficamente por nome via std::map<QString, Operation*>.
 *
 * Esta classe não tem dependências de Qt widgets — apenas QString para
 * comunicação de resultados formatados.
 */
class CalculatorEngine {
public:
    CalculatorEngine();
    ~CalculatorEngine();

    // Entrada de dígitos e ponto decimal
    QString inputDigit(QChar digit);
    QString inputDecimal();
    QString backspace();

    // Operações binárias e resultado
    QString setPendingOperation(const QString& opName);
    QString calculate();

    // Operações unárias aplicadas ao valor atual
    QString applyUnary(const QString& opName);

    // Controles de estado
    QString clear();
    QString clearEntry();
    QString toggleSign();
    QString percentage();

    // Memória
    void    memoryStore();
    void    memoryAdd();
    void    memorySubtract();
    QString memoryRecall();
    void    memoryClear();
    bool    memoryHasValue() const;

    // Configurações científicas
    void setAngleUnit(AngleUnit unit);
    AngleUnit angleUnit() const;
    void toggle2nd();
    bool is2ndActive() const;

    // Constantes e utilidades
    QString insertConstant(const QString& name);
    QString randomNumber();

    // Leitura do display atual
    QString currentDisplay() const;
    QString currentExpression() const;

private:
    double  currentValue_;
    double  pendingValue_;
    Operation* pendingOp_;

    bool    isNewEntry_;
    bool    hasDecimal_;
    bool    justCalculated_;

    Memory  memory_;
    AngleUnit angleUnit_;
    bool    is2ndActive_;

    std::vector<std::unique_ptr<Operation>> ownedOps_;
    std::map<QString, Operation*> ops_;

    QString displayStr_;
    QString expressionStr_;

    std::vector<double> history_;

    void   registerOps();
    QString formatResult(double v) const;
    void   setDisplay(const QString& s);
};
