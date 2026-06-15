#pragma once
#include <QWidget>
#include <QString>

class QLabel;
class QPushButton;

/**
 * Widget de display da calculadora.
 *
 * Exibe duas linhas: a expressão em construção (topo, menor) e o
 * valor atual (base, maior). O botão de toggle do modo científico
 * é sobreposto no canto superior esquerdo do display.
 * Demonstra sobrecarga de operator<< para interface fluente.
 */
class CalcDisplay : public QWidget {
    Q_OBJECT
public:
    explicit CalcDisplay(QWidget* parent = nullptr);

    void setValue(const QString& v);
    void setExpression(const QString& expr);
    void clear();
    void setToggleButton(QPushButton* btn);

    /** Atualiza o valor principal — permite: display << "3.5" */
    CalcDisplay& operator<<(const QString& value);

protected:
    void resizeEvent(QResizeEvent* e) override;

private:
    QLabel*      mainLabel_;
    QLabel*      exprLabel_;
    QPushButton* toggleBtn_ = nullptr;
};
