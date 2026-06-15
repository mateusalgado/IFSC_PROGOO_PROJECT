#pragma once
#include "StandardPanel.h"
#include "engine/UnaryOperation.h"

/**
 * Painel do modo científico. Herda de StandardPanel e adiciona
 * a grade de botões científicos acima dos botões normais.
 *
 * Gerencia o modo "2nd" (funções inversas) e a unidade de ângulo (Rad/Grau).
 */
class ScientificPanel : public StandardPanel {
    Q_OBJECT
public:
    explicit ScientificPanel(QWidget* parent = nullptr);

    void toggle2nd();
    void setAngleUnit(AngleUnit unit);

protected:
    void buildButtons() override;

private:
    bool is2ndActive_;
    AngleUnit angleUnit_;

    void update2ndLabels();
};
