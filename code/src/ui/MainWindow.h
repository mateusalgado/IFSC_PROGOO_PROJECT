#pragma once
#include <QMainWindow>

class CalculatorEngine;
class CalcDisplay;
class StandardPanel;
class ScientificPanel;
class QPushButton;
class QPropertyAnimation;

/**
 * Janela principal da calculadora.
 *
 * Coordena o engine, os painéis e o display. Gerencia a animação de
 * expansão/colapso do painel científico e repassa todos os eventos
 * de botão para o CalculatorEngine via slots Qt.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void onButtonClicked(const QString& actionKey);
    void toggleScientificMode();

private:
    void setupUi();
    void applyStyles();
    void connectSignals();
    void updateDisplay(const QString& result);

    CalculatorEngine* engine_;
    CalcDisplay*      display_;
    StandardPanel*    stdPanel_;
    ScientificPanel*  sciPanel_;
    QPushButton*      toggleBtn_;
    QPropertyAnimation* animation_;
    bool              isScientific_;
};
