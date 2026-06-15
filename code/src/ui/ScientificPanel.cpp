#include "ScientificPanel.h"
#include "CalculatorButton.h"
#include <QGridLayout>
#include <QStyle>

ScientificPanel::ScientificPanel(QWidget* parent)
    : StandardPanel(parent)
    , is2ndActive_(false)
    , angleUnit_(AngleUnit::RADIANS)
{
    // StandardPanel() chamou StandardPanel::buildButtons() com botões normais.
    // Limpamos e reconstruímos com apenas os 6 botões científicos por linha.
    for (auto* btn : buttons_) {
        layout_->removeWidget(btn);
        btn->deleteLater();
    }
    buttons_.clear();
    // Reajusta espaçamento para botões menores
    layout_->setSpacing(8);
    layout_->setContentsMargins(10, 10, 4, 10);
    buildButtons();
}

void ScientificPanel::buildButtons()
{
    struct BtnDef { QString label; QString key; ButtonType type; };

    // 5 linhas × 6 colunas — mesmo número de linhas que o StandardPanel
    const std::vector<std::vector<BtnDef>> rows = {
        { {"(",    "(",     ButtonType::FUNCTION},
          {")",    ")",     ButtonType::FUNCTION},
          {"mc",   "mc",    ButtonType::MEMORY},
          {"m+",   "m+",    ButtonType::MEMORY},
          {"m-",   "m-",    ButtonType::MEMORY},
          {"mr",   "mr",    ButtonType::MEMORY} },

        { {"2nd",  "2nd",   ButtonType::SPECIAL},
          {"x²",   "x²",    ButtonType::FUNCTION},
          {"x³",   "x³",    ButtonType::FUNCTION},
          {"xʸ",   "xʸ",    ButtonType::FUNCTION},
          {"eˣ",   "eˣ",    ButtonType::FUNCTION},
          {"10ˣ",  "10ˣ",   ButtonType::FUNCTION} },

        { {"¹/x",  "1/x",   ButtonType::FUNCTION},
          {"²√x",  "²√x",   ButtonType::FUNCTION},
          {"³√x",  "³√x",   ButtonType::FUNCTION},
          {"ʸ√x",  "ʸ√x",   ButtonType::FUNCTION},
          {"ln",   "ln",    ButtonType::FUNCTION},
          {"log₁₀","log₁₀", ButtonType::FUNCTION} },

        { {"x!",   "x!",    ButtonType::FUNCTION},
          {"sin",  "sin",   ButtonType::FUNCTION},
          {"cos",  "cos",   ButtonType::FUNCTION},
          {"tan",  "tan",   ButtonType::FUNCTION},
          {"e",    "e",     ButtonType::FUNCTION},
          {"EE",   "EE",    ButtonType::FUNCTION} },

        { {"Rand", "Rand",  ButtonType::FUNCTION},
          {"sinh", "sinh",  ButtonType::FUNCTION},
          {"cosh", "cosh",  ButtonType::FUNCTION},
          {"tanh", "tanh",  ButtonType::FUNCTION},
          {"π",    "π",     ButtonType::FUNCTION},
          {"Rad",  "Rad",   ButtonType::SPECIAL} },
    };

    for (int r = 0; r < static_cast<int>(rows.size()); ++r) {
        for (int c = 0; c < static_cast<int>(rows[r].size()); ++c) {
            const auto& d = rows[r][c];
            auto* btn = new CalculatorButton(d.label, d.key, d.type, this);
            layout_->addWidget(btn, r, c);
            buttons_.push_back(btn);
        }
    }

    connectButtons();
}

void ScientificPanel::toggle2nd()
{
    is2ndActive_ = !is2ndActive_;
    update2ndLabels();
}

void ScientificPanel::setAngleUnit(AngleUnit unit)
{
    angleUnit_ = unit;
    for (auto* btn : buttons_) {
        if (btn->getActionKey() == "Rad")
            btn->setLabel(unit == AngleUnit::RADIANS ? "Rad" : "Grau");
    }
}

void ScientificPanel::update2ndLabels()
{
    static const std::vector<std::pair<QString,QString>> pairs = {
        {"sin",  "asin"},  {"cos",  "acos"},  {"tan",  "atan"},
        {"sinh", "asinh"}, {"cosh", "acosh"}, {"tanh", "atanh"},
        {"eˣ",  "ln"},     {"10ˣ", "log₁₀"}, {"x²",  "²√x"},
        {"x³",  "³√x"},
    };

    for (auto* btn : buttons_) {
        for (const auto& [normal, inv] : pairs) {
            if (btn->getActionKey() == normal)
                btn->setLabel(is2ndActive_ ? inv : normal);
        }
        if (btn->getActionKey() == "2nd") {
            btn->setProperty("active2nd", is2ndActive_);
            btn->style()->unpolish(btn);
            btn->style()->polish(btn);
        }
    }
}
