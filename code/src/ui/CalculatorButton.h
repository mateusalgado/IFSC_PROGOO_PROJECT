#pragma once
#include <QPushButton>
#include <QString>

enum class ButtonType {
    DIGIT,     // 0-9, ponto decimal
    OPERATOR,  // +, -, ×, ÷, =
    FUNCTION,  // funções científicas
    SPECIAL,   // AC, +/-, %, 2nd, Rad
    MEMORY     // mc, m+, m-, mr
};

/**
 * Botão personalizado da calculadora.
 * Guarda a chave de ação (actionKey_) que identifica unicamente a operação,
 * independentemente do rótulo exibido (que pode mudar com o modo 2nd).
 *
 * Demonstra sobrecarga de operator== para comparar botões por chave de ação.
 */
class CalculatorButton : public QPushButton {
    Q_OBJECT
public:
    CalculatorButton(const QString& label,
                     const QString& actionKey,
                     ButtonType type,
                     QWidget* parent = nullptr);

    QString    getActionKey() const;
    ButtonType getType()      const;

    void setLabel(const QString& label);

    bool operator==(const CalculatorButton& other) const;

private:
    QString    actionKey_;
    ButtonType type_;
};
