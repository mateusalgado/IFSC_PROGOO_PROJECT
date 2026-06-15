#pragma once
#include <QString>

/**
 * Classe abstrata base para todas as operações da calculadora.
 * Define a interface polimórfica execute() que toda operação deve implementar.
 */
class Operation {
public:
    explicit Operation(const QString& name);
    virtual ~Operation() = default;

    virtual double execute(double a, double b = 0.0) const = 0;

    QString getName() const;

protected:
    QString name_;
};
