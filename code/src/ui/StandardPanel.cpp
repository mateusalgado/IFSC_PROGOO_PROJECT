#include "StandardPanel.h"
#include "CalculatorButton.h"
#include <QGridLayout>

StandardPanel::StandardPanel(QWidget* parent)
    : QWidget(parent)
{
    layout_ = new QGridLayout(this);
    layout_->setSpacing(10);
    layout_->setContentsMargins(10, 10, 10, 10);
    StandardPanel::buildButtons();
    connectButtons();
}

void StandardPanel::buildButtons()
{
    struct BtnDef { QString label; QString key; ButtonType type; };

    // 5 linhas × 4 colunas — idêntico ao iPhone
    const std::vector<std::vector<BtnDef>> rows = {
        { {"⌫",  "BS",  ButtonType::SPECIAL},
          {"AC",  "AC",  ButtonType::SPECIAL},
          {"%",   "%",   ButtonType::SPECIAL},
          {"÷",   "÷",   ButtonType::OPERATOR} },

        { {"7",   "7",   ButtonType::DIGIT},
          {"8",   "8",   ButtonType::DIGIT},
          {"9",   "9",   ButtonType::DIGIT},
          {"×",   "×",   ButtonType::OPERATOR} },

        { {"4",   "4",   ButtonType::DIGIT},
          {"5",   "5",   ButtonType::DIGIT},
          {"6",   "6",   ButtonType::DIGIT},
          {"-",   "-",   ButtonType::OPERATOR} },

        { {"1",   "1",   ButtonType::DIGIT},
          {"2",   "2",   ButtonType::DIGIT},
          {"3",   "3",   ButtonType::DIGIT},
          {"+",   "+",   ButtonType::OPERATOR} },

        { {"+/-", "+/-", ButtonType::DIGIT},
          {"0",   "0",   ButtonType::DIGIT},
          {",",   ".",   ButtonType::DIGIT},
          {"=",   "=",   ButtonType::OPERATOR} },
    };

    for (int r = 0; r < static_cast<int>(rows.size()); ++r) {
        for (int c = 0; c < static_cast<int>(rows[r].size()); ++c) {
            const auto& d = rows[r][c];
            auto* btn = new CalculatorButton(d.label, d.key, d.type, this);
            layout_->addWidget(btn, r, c);
            buttons_.push_back(btn);
        }
    }
}

void StandardPanel::connectButtons()
{
    for (auto* btn : buttons_) {
        connect(btn, &QPushButton::clicked, this, [this, btn]() {
            emit buttonClicked(btn->getActionKey());
        });
    }
}
