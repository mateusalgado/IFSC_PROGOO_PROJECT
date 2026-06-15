#include "MainWindow.h"
#include "CalcDisplay.h"
#include "StandardPanel.h"
#include "ScientificPanel.h"
#include "CalculatorButton.h"
#include "engine/CalculatorEngine.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QFile>
#include <QApplication>
#include <map>

// Modo normal: 4 colunas × 5 linhas, idêntico ao iPhone
static constexpr int COL_W    = 74;   // largura de cada coluna
static constexpr int COL_GAP  = 10;   // espaçamento entre colunas
static constexpr int MARGIN   = 10;   // margem lateral

static constexpr int STD_COLS = 4;
static constexpr int SCI_COLS = 6;

static constexpr int STD_BTN_AREA = STD_COLS * COL_W + (STD_COLS - 1) * COL_GAP + 2 * MARGIN;
static constexpr int SCI_BTN_AREA = SCI_COLS * COL_W + (SCI_COLS - 1) * COL_GAP + MARGIN; // margem direita = 0 (cola no stdPanel)

static constexpr int WIN_H = 620;
static constexpr int DISPLAY_H = 160;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , engine_(new CalculatorEngine())
    , isScientific_(false)
{
    setupUi();
    applyStyles();
    connectSignals();
}

MainWindow::~MainWindow()
{
    delete engine_;
}

void MainWindow::setupUi()
{
    setWindowTitle("Calculadora");
    setFixedSize(STD_BTN_AREA, WIN_H);

    auto* central = new QWidget(this);
    setCentralWidget(central);

    // Layout raiz VERTICAL: display (full width) / linha de botões
    auto* root = new QVBoxLayout(central);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // ── Display: largura total, toggle sobreposto ─────────────────────────
    display_ = new CalcDisplay(this);
    display_->setFixedHeight(DISPLAY_H);

    toggleBtn_ = new QPushButton("Científico ▶", this);
    toggleBtn_->setObjectName("toggleBtn");
    toggleBtn_->setFixedHeight(26);
    display_->setToggleButton(toggleBtn_);

    // ── Linha de botões: [sciPanel | stdPanel] ────────────────────────────
    auto* btnRow = new QHBoxLayout();
    btnRow->setContentsMargins(0, 0, 0, 0);
    btnRow->setSpacing(0);

    sciPanel_ = new ScientificPanel(this);
    sciPanel_->setVisible(false);
    sciPanel_->setFixedWidth(SCI_BTN_AREA);

    stdPanel_ = new StandardPanel(this);

    btnRow->addWidget(sciPanel_);
    btnRow->addWidget(stdPanel_, 1);

    root->addWidget(display_);
    root->addLayout(btnRow, 1);

    animation_ = new QPropertyAnimation(this, "size", this);
    animation_->setDuration(200);
    animation_->setEasingCurve(QEasingCurve::InOutCubic);
}

void MainWindow::applyStyles()
{
    QFile f(":/style.qss");
    if (f.open(QFile::ReadOnly)) {
        qApp->setStyleSheet(QString::fromUtf8(f.readAll()));
        return;
    }

    // Estilo inline de fallback (mesma paleta do style.qss)
    qApp->setStyleSheet(R"(
        QMainWindow, QWidget { background: #000000; color: #FFFFFF; }

        #mainLabel { font-size: 72px; font-weight: 200; color: #FFFFFF; }
        #exprLabel { font-size: 22px; color: #8E8E93; }

        CalculatorButton {
            border-radius: 36px;
            font-size: 28px;
            font-weight: 400;
            color: #FFFFFF;
            border: none;
        }
        CalculatorButton[btnType="digit"]    { background: #333333; }
        CalculatorButton[btnType="digit"]:pressed    { background: #737373; }
        CalculatorButton[btnType="operator"] { background: #FF9500; color: #FFFFFF; }
        CalculatorButton[btnType="operator"]:pressed { background: #FFFFFF; color: #FF9500; }
        CalculatorButton[btnType="special"]  { background: #A5A5A5; color: #000000; }
        CalculatorButton[btnType="special"]:pressed  { background: #FFFFFF; color: #000000; }
        CalculatorButton[btnType="function"] { background: #1C1C1E; color: #FFFFFF; font-size: 15px; border-radius: 26px; }
        CalculatorButton[btnType="function"]:pressed { background: #636366; }
        CalculatorButton[btnType="memory"]   { background: #1C1C1E; color: #FF9500; font-size: 15px; border-radius: 26px; }
        CalculatorButton[btnType="memory"]:pressed   { background: #636366; }
        CalculatorButton[active2nd="true"]   { background: #EBEBF5; color: #1C1C1E; }

        #toggleBtn {
            background: #1C1C1E; color: #FF9500;
            font-size: 12px; font-weight: 600;
            border: 1px solid #3A3A3C; border-radius: 7px;
            padding: 1px 8px;
        }
        #toggleBtn:hover   { background: #2C2C2E; }
        #toggleBtn:pressed { background: #48484A; }
    )");
}

void MainWindow::connectSignals()
{
    connect(stdPanel_, &StandardPanel::buttonClicked,
            this, &MainWindow::onButtonClicked);
    connect(sciPanel_, &StandardPanel::buttonClicked,
            this, &MainWindow::onButtonClicked);
    connect(toggleBtn_, &QPushButton::clicked,
            this, &MainWindow::toggleScientificMode);
}

void MainWindow::onButtonClicked(const QString& key)
{
    QString result;

    // ── Dígitos ──────────────────────────────────────────────────────────────
    if (key.length() == 1 && key[0].isDigit()) {
        result = engine_->inputDigit(key[0]);
        display_->setExpression(engine_->currentExpression());
        updateDisplay(result);
        return;
    }

    if (key == ".") {
        result = engine_->inputDecimal();
        updateDisplay(result);
        return;
    }

    // ── Controles básicos ─────────────────────────────────────────────────────
    if (key == "BS")   { updateDisplay(engine_->backspace());   return; }
    if (key == "AC")   { display_->clear(); engine_->clear();   return; }
    if (key == "CE")   { updateDisplay(engine_->clearEntry());  return; }
    if (key == "+/-")  { updateDisplay(engine_->toggleSign());  return; }
    if (key == "%")    { updateDisplay(engine_->percentage());  return; }

    // ── Operadores binários ───────────────────────────────────────────────────
    static const QStringList binOps = {"+", "-", "×", "÷", "xʸ", "ʸ√x"};
    if (binOps.contains(key)) {
        result = engine_->setPendingOperation(key);
        display_->setExpression(engine_->currentExpression());
        updateDisplay(result);
        return;
    }

    if (key == "=") {
        result = engine_->calculate();
        display_->setExpression(engine_->currentExpression());
        updateDisplay(result);
        return;
    }

    // ── Memória ───────────────────────────────────────────────────────────────
    if (key == "mc")  { engine_->memoryClear();                       return; }
    if (key == "m+")  { engine_->memoryAdd();                         return; }
    if (key == "m-")  { engine_->memorySubtract();                    return; }
    if (key == "mr")  { updateDisplay(engine_->memoryRecall());       return; }

    // ── Modo 2nd e ângulo ─────────────────────────────────────────────────────
    if (key == "2nd") {
        engine_->toggle2nd();
        sciPanel_->toggle2nd();
        return;
    }
    if (key == "Rad") {
        AngleUnit nu = (engine_->angleUnit() == AngleUnit::RADIANS)
                       ? AngleUnit::DEGREES : AngleUnit::RADIANS;
        engine_->setAngleUnit(nu);
        sciPanel_->setAngleUnit(nu);
        return;
    }

    // ── Constantes ────────────────────────────────────────────────────────────
    if (key == "π" || key == "e") {
        updateDisplay(engine_->insertConstant(key));
        return;
    }
    if (key == "Rand") { updateDisplay(engine_->randomNumber()); return; }
    if (key == "EE")   { return; }  // simplificado
    if (key == "(" || key == ")") { return; }

    // ── Funções unárias (com suporte a 2nd) ───────────────────────────────────
    // Mapeamento do label atual para a chave real do engine (no modo 2nd o label mudou)
    static const std::map<QString,QString> labelToKey = {
        {"asin","asin"},{"acos","acos"},{"atan","atan"},
        {"asinh","asinh"},{"acosh","acosh"},{"atanh","atanh"},
        {"ln","ln"},{"log₁₀","log₁₀"},{"²√x","²√x"},{"³√x","³√x"},
    };

    QString opKey = key;
    if (engine_->is2ndActive()) {
        static const std::map<QString,QString> inv = {
            {"sin","asin"},{"cos","acos"},{"tan","atan"},
            {"sinh","asinh"},{"cosh","acosh"},{"tanh","atanh"},
            {"eˣ","ln"},{"10ˣ","log₁₀"},{"x²","²√x"},{"x³","³√x"},
        };
        if (inv.count(key)) opKey = inv.at(key);
    }

    result = engine_->applyUnary(opKey);
    display_->setExpression(engine_->currentExpression());
    updateDisplay(result);

    if (engine_->is2ndActive()) {
        engine_->toggle2nd();
        sciPanel_->toggle2nd();
    }
}

void MainWindow::toggleScientificMode()
{
    isScientific_ = !isScientific_;

    if (isScientific_) {
        sciPanel_->setVisible(true);
        setFixedSize(STD_BTN_AREA + SCI_BTN_AREA, WIN_H);
        toggleBtn_->setText("◀ Normal");
    } else {
        sciPanel_->setVisible(false);
        setFixedSize(STD_BTN_AREA, WIN_H);
        toggleBtn_->setText("Científico ▶");
    }
}

void MainWindow::updateDisplay(const QString& result)
{
    *display_ << result;
}
