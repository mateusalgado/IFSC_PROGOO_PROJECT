#include "CalcDisplay.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QResizeEvent>

CalcDisplay::CalcDisplay(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(14, 6, 14, 4);
    layout->setSpacing(0);

    exprLabel_ = new QLabel("", this);
    exprLabel_->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    exprLabel_->setObjectName("exprLabel");
    exprLabel_->setWordWrap(false);

    mainLabel_ = new QLabel("0", this);
    mainLabel_->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    mainLabel_->setObjectName("mainLabel");
    mainLabel_->setWordWrap(false);

    layout->addStretch(1);
    layout->addWidget(exprLabel_);
    layout->addWidget(mainLabel_);
}

void CalcDisplay::setToggleButton(QPushButton* btn)
{
    toggleBtn_ = btn;
    btn->setParent(this);
    // Posiciona no canto superior esquerdo
    btn->move(10, 8);
    btn->raise();
}

void CalcDisplay::resizeEvent(QResizeEvent* e)
{
    QWidget::resizeEvent(e);
    if (toggleBtn_)
        toggleBtn_->move(10, 8);
}

void CalcDisplay::setValue(const QString& v)
{
    mainLabel_->setText(v);
    int len = v.length();
    int fs = (len > 11) ? 36 : (len > 8) ? 48 : (len > 6) ? 58 : 72;
    mainLabel_->setStyleSheet(
        QString("font-size: %1px; font-weight: 200; color: #FFFFFF;").arg(fs));
}

void CalcDisplay::setExpression(const QString& expr)
{
    exprLabel_->setText(expr);
}

void CalcDisplay::clear()
{
    mainLabel_->setText("0");
    mainLabel_->setStyleSheet("");
    exprLabel_->setText("");
}

CalcDisplay& CalcDisplay::operator<<(const QString& value)
{
    setValue(value);
    return *this;
}
