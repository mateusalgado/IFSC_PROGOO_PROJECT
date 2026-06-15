#include "CalculatorEngine.h"
#include "BinaryOperation.h"
#include "UnaryOperation.h"

#include <cmath>
#include <stdexcept>
#include <random>
#include <QLocale>

// ── Construtor / Destrutor ────────────────────────────────────────────────────

CalculatorEngine::CalculatorEngine()
    : currentValue_(0.0)
    , pendingValue_(0.0)
    , pendingOp_(nullptr)
    , isNewEntry_(true)
    , hasDecimal_(false)
    , justCalculated_(false)
    , angleUnit_(AngleUnit::RADIANS)
    , is2ndActive_(false)
    , displayStr_("0")
    , expressionStr_("")
{
    registerOps();
}

CalculatorEngine::~CalculatorEngine() = default;

// ── Registro de operações ─────────────────────────────────────────────────────

void CalculatorEngine::registerOps()
{
    auto addOp = [&](std::unique_ptr<Operation> op) {
        Operation* raw = op.get();
        ownedOps_.push_back(std::move(op));
        ops_[raw->getName()] = raw;
    };

    // Binárias
    addOp(std::make_unique<Addition>());
    addOp(std::make_unique<Subtraction>());
    addOp(std::make_unique<Multiplication>());
    addOp(std::make_unique<Division>());
    addOp(std::make_unique<Power>());
    addOp(std::make_unique<NthRoot>());

    // Unárias — trigonométricas (precisam do ponteiro para angleUnit_)
    const AngleUnit* au = &angleUnit_;

    addOp(std::make_unique<Trigonometric>("sin",  [](double x){ return std::sin(x);  }, au));
    addOp(std::make_unique<Trigonometric>("cos",  [](double x){ return std::cos(x);  }, au));
    addOp(std::make_unique<Trigonometric>("tan",  [](double x){ return std::tan(x);  }, au));
    addOp(std::make_unique<Trigonometric>("asin", [](double x){ return std::asin(x); }, au, true));
    addOp(std::make_unique<Trigonometric>("acos", [](double x){ return std::acos(x); }, au, true));
    addOp(std::make_unique<Trigonometric>("atan", [](double x){ return std::atan(x); }, au, true));
    addOp(std::make_unique<Trigonometric>("sinh", [](double x){ return std::sinh(x); }, nullptr));
    addOp(std::make_unique<Trigonometric>("cosh", [](double x){ return std::cosh(x); }, nullptr));
    addOp(std::make_unique<Trigonometric>("tanh", [](double x){ return std::tanh(x); }, nullptr));
    addOp(std::make_unique<Trigonometric>("asinh",[](double x){ return std::asinh(x);}, nullptr, true));
    addOp(std::make_unique<Trigonometric>("acosh",[](double x){ return std::acosh(x);}, nullptr, true));
    addOp(std::make_unique<Trigonometric>("atanh",[](double x){ return std::atanh(x);}, nullptr, true));

    // Logarítmicas
    addOp(std::make_unique<Logarithmic>(LogBase::NATURAL));
    addOp(std::make_unique<Logarithmic>(LogBase::TEN));

    // Potências unárias
    addOp(std::make_unique<PowerUnary>("x²", 2.0));
    addOp(std::make_unique<PowerUnary>("x³", 3.0));
    addOp(std::make_unique<Exponential>());
    addOp(std::make_unique<TenPower>());

    // Raízes unárias
    addOp(std::make_unique<RootUnary>("²√x", 2.0));
    addOp(std::make_unique<RootUnary>("³√x", 3.0));

    // Outras
    addOp(std::make_unique<Factorial>());
    addOp(std::make_unique<Inverse>());
}

// ── Entrada de dígitos ────────────────────────────────────────────────────────

QString CalculatorEngine::inputDigit(QChar digit)
{
    if (isNewEntry_ || justCalculated_) {
        displayStr_    = (digit == '0') ? "0" : QString(digit);
        isNewEntry_    = false;
        justCalculated_= false;
        hasDecimal_    = false;
    } else {
        if (displayStr_ == "0" && digit != '.')
            displayStr_ = QString(digit);
        else
            displayStr_ += digit;
    }
    currentValue_ = displayStr_.toDouble();
    return displayStr_;
}

QString CalculatorEngine::backspace()
{
    if (justCalculated_ || isNewEntry_) {
        return clear();
    }
    if (displayStr_.length() > 1) {
        if (displayStr_.back() == '.') hasDecimal_ = false;
        displayStr_.chop(1);
    } else {
        displayStr_ = "0";
        isNewEntry_  = true;
    }
    currentValue_ = displayStr_.toDouble();
    return displayStr_;
}

QString CalculatorEngine::inputDecimal()
{
    if (justCalculated_ || isNewEntry_) {
        displayStr_    = "0.";
        isNewEntry_    = false;
        justCalculated_= false;
        hasDecimal_    = true;
    } else if (!hasDecimal_) {
        displayStr_ += '.';
        hasDecimal_  = true;
    }
    currentValue_ = displayStr_.toDouble();
    return displayStr_;
}

// ── Operações binárias ────────────────────────────────────────────────────────

QString CalculatorEngine::setPendingOperation(const QString& opName)
{
    if (pendingOp_ && !isNewEntry_) {
        try {
            currentValue_ = pendingOp_->execute(pendingValue_, currentValue_);
            setDisplay(formatResult(currentValue_));
        } catch (const std::exception&) {
            pendingOp_    = nullptr;
            setDisplay("Erro");
            isNewEntry_   = true;
            return displayStr_;
        }
    }
    pendingValue_ = currentValue_;
    pendingOp_    = ops_.count(opName) ? ops_.at(opName) : nullptr;
    expressionStr_= formatResult(pendingValue_) + " " + opName;
    isNewEntry_   = true;
    hasDecimal_   = false;
    return displayStr_;
}

QString CalculatorEngine::calculate()
{
    if (!pendingOp_)
        return displayStr_;

    try {
        double result = pendingOp_->execute(pendingValue_, currentValue_);
        expressionStr_ = formatResult(pendingValue_) + " " +
                         pendingOp_->getName() + " " +
                         formatResult(currentValue_) + " =";
        currentValue_  = result;
        pendingOp_     = nullptr;
        justCalculated_= true;
        isNewEntry_    = true;
        history_.push_back(result);
        setDisplay(formatResult(result));
    } catch (const std::exception&) {
        pendingOp_  = nullptr;
        isNewEntry_ = true;
        setDisplay("Erro");
    }
    return displayStr_;
}

// ── Operações unárias ─────────────────────────────────────────────────────────

QString CalculatorEngine::applyUnary(const QString& opName)
{
    if (!ops_.count(opName))
        return displayStr_;

    try {
        double result  = ops_.at(opName)->execute(currentValue_);
        expressionStr_ = opName + "(" + formatResult(currentValue_) + ")";
        currentValue_  = result;
        justCalculated_= true;
        isNewEntry_    = true;
        setDisplay(formatResult(result));
    } catch (const std::exception&) {
        isNewEntry_ = true;
        setDisplay("Erro");
    }
    return displayStr_;
}

// ── Controles de estado ───────────────────────────────────────────────────────

QString CalculatorEngine::clear()
{
    currentValue_  = 0.0;
    pendingValue_  = 0.0;
    pendingOp_     = nullptr;
    isNewEntry_    = true;
    hasDecimal_    = false;
    justCalculated_= false;
    expressionStr_ = "";
    setDisplay("0");
    return displayStr_;
}

QString CalculatorEngine::clearEntry()
{
    currentValue_ = 0.0;
    isNewEntry_   = true;
    hasDecimal_   = false;
    setDisplay("0");
    return displayStr_;
}

QString CalculatorEngine::toggleSign()
{
    currentValue_ = -currentValue_;
    if (displayStr_.startsWith('-'))
        displayStr_.remove(0, 1);
    else if (currentValue_ != 0.0)
        displayStr_.prepend('-');
    return displayStr_;
}

QString CalculatorEngine::percentage()
{
    currentValue_ /= 100.0;
    setDisplay(formatResult(currentValue_));
    return displayStr_;
}

// ── Memória ───────────────────────────────────────────────────────────────────

void CalculatorEngine::memoryStore()     { memory_.store(currentValue_); }
void CalculatorEngine::memoryAdd()       { memory_ += currentValue_; }
void CalculatorEngine::memorySubtract()  { memory_ -= currentValue_; }
void CalculatorEngine::memoryClear()     { memory_.clear(); }
bool CalculatorEngine::memoryHasValue() const { return memory_.hasValue(); }

QString CalculatorEngine::memoryRecall()
{
    currentValue_ = memory_.recall();
    isNewEntry_   = true;
    setDisplay(formatResult(currentValue_));
    return displayStr_;
}

// ── Configurações científicas ─────────────────────────────────────────────────

void CalculatorEngine::setAngleUnit(AngleUnit unit) { angleUnit_ = unit; }
AngleUnit CalculatorEngine::angleUnit() const       { return angleUnit_; }

void CalculatorEngine::toggle2nd()     { is2ndActive_ = !is2ndActive_; }
bool CalculatorEngine::is2ndActive() const { return is2ndActive_; }

// ── Constantes ────────────────────────────────────────────────────────────────

QString CalculatorEngine::insertConstant(const QString& name)
{
    if (name == "π")
        currentValue_ = M_PI;
    else if (name == "e")
        currentValue_ = M_E;
    else
        return displayStr_;

    isNewEntry_    = true;
    justCalculated_= true;
    setDisplay(formatResult(currentValue_));
    return displayStr_;
}

QString CalculatorEngine::randomNumber()
{
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    currentValue_  = dist(rng);
    isNewEntry_    = true;
    justCalculated_= true;
    setDisplay(formatResult(currentValue_));
    return displayStr_;
}

// ── Display ───────────────────────────────────────────────────────────────────

QString CalculatorEngine::currentDisplay()    const { return displayStr_; }
QString CalculatorEngine::currentExpression() const { return expressionStr_; }

void CalculatorEngine::setDisplay(const QString& s)
{
    displayStr_ = s;
}

QString CalculatorEngine::formatResult(double v) const
{
    if (std::isnan(v) || std::isinf(v))
        return "Erro";

    // Sem casas decimais desnecessárias
    if (v == std::floor(v) && std::abs(v) < 1e15)
        return QString::number(static_cast<long long>(v));

    // Notação científica para números muito grandes/pequenos
    if (std::abs(v) >= 1e10 || (std::abs(v) < 1e-6 && v != 0.0))
        return QString::number(v, 'e', 6);

    QString s = QString::number(v, 'f', 10);
    // Remove zeros à direita após o ponto decimal
    if (s.contains('.')) {
        s = s.trimmed();
        while (s.endsWith('0')) s.chop(1);
        if (s.endsWith('.')) s.chop(1);
    }
    return s;
}
