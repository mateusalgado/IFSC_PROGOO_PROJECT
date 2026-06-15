#pragma once
#include <QWidget>
#include <QString>
#include <vector>

class CalculatorButton;
class QGridLayout;

/**
 * Painel com os botões do modo normal da calculadora.
 *
 * Contém: AC/CE, +/-, %, ÷, ×, -, +, =, dígitos 0-9, ponto decimal,
 * e os botões de memória (mc, m+, m-, mr) na linha superior.
 */
class StandardPanel : public QWidget {
    Q_OBJECT
public:
    explicit StandardPanel(QWidget* parent = nullptr);
    virtual ~StandardPanel() = default;

signals:
    void buttonClicked(const QString& actionKey);

protected:
    virtual void buildButtons();
    void connectButtons();

    std::vector<CalculatorButton*> buttons_;
    QGridLayout* layout_;
};
