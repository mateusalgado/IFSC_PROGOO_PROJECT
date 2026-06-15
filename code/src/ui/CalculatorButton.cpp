#include "CalculatorButton.h"

CalculatorButton::CalculatorButton(const QString& label,
                                   const QString& actionKey,
                                   ButtonType type,
                                   QWidget* parent)
    : QPushButton(label, parent)
    , actionKey_(actionKey)
    , type_(type)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(40, 52);

    // Propriedade usada pelo QSS para estilização por tipo
    QString typeStr;
    switch (type_) {
        case ButtonType::DIGIT:    typeStr = "digit";    break;
        case ButtonType::OPERATOR: typeStr = "operator"; break;
        case ButtonType::FUNCTION: typeStr = "function"; break;
        case ButtonType::SPECIAL:  typeStr = "special";  break;
        case ButtonType::MEMORY:   typeStr = "memory";   break;
    }
    setProperty("btnType", typeStr);
}

QString CalculatorButton::getActionKey() const { return actionKey_; }
ButtonType CalculatorButton::getType()   const { return type_; }

void CalculatorButton::setLabel(const QString& label)
{
    setText(label);
}

bool CalculatorButton::operator==(const CalculatorButton& other) const
{
    return actionKey_ == other.actionKey_;
}
