#include <QtTest>
#include "engine/CalculatorEngine.h"
#include "engine/BinaryOperation.h"
#include "engine/UnaryOperation.h"
#include "engine/Memory.h"

class TestEngine : public QObject {
    Q_OBJECT

private slots:
    void testAddition();
    void testSubtraction();
    void testMultiplication();
    void testDivision();
    void testDivisionByZero();
    void testNthRoot();
    void testPower();
    void testFactorial();
    void testFactorialInvalid();
    void testInverse();
    void testInverseZero();
    void testLogarithmNatural();
    void testLogarithmBase10();
    void testLogarithmInvalid();
    void testSine();
    void testMemoryOperators();
    void testEngineFullCalculation();
    void testEngineUnary();
    void testEngineToggleSign();
    void testEnginePercentage();
    void testEngineClear();
};

// ── Testes de operações binárias ──────────────────────────────────────────────

void TestEngine::testAddition() {
    Addition op;
    QCOMPARE(op.execute(3.0, 5.0), 8.0);
    QCOMPARE(op.execute(-1.0, 1.0), 0.0);
}

void TestEngine::testSubtraction() {
    Subtraction op;
    QCOMPARE(op.execute(10.0, 4.0), 6.0);
    QCOMPARE(op.execute(0.0, 5.0), -5.0);
}

void TestEngine::testMultiplication() {
    Multiplication op;
    QCOMPARE(op.execute(3.0, 7.0), 21.0);
    QCOMPARE(op.execute(-2.0, 3.0), -6.0);
}

void TestEngine::testDivision() {
    Division op;
    QCOMPARE(op.execute(10.0, 2.0), 5.0);
    QCOMPARE(op.execute(7.0, 2.0), 3.5);
}

void TestEngine::testDivisionByZero() {
    Division op;
    QVERIFY_THROWS_EXCEPTION(std::domain_error, op.execute(5.0, 0.0));
}

void TestEngine::testNthRoot() {
    NthRoot op;
    QCOMPARE(op.execute(8.0, 3.0), 2.0);
    QVERIFY(qAbs(op.execute(2.0, 2.0) - 1.41421356) < 1e-6);
}

void TestEngine::testPower() {
    Power op;
    QCOMPARE(op.execute(2.0, 10.0), 1024.0);
    QCOMPARE(op.execute(3.0, 2.0), 9.0);
}

// ── Testes de operações unárias ───────────────────────────────────────────────

void TestEngine::testFactorial() {
    Factorial op;
    QCOMPARE(op.apply(0.0), 1.0);
    QCOMPARE(op.apply(5.0), 120.0);
    QCOMPARE(op.apply(10.0), 3628800.0);
}

void TestEngine::testFactorialInvalid() {
    Factorial op;
    QVERIFY_THROWS_EXCEPTION(std::domain_error, op.apply(-1.0));
    QVERIFY_THROWS_EXCEPTION(std::domain_error, op.apply(2.5));
}

void TestEngine::testInverse() {
    Inverse op;
    QCOMPARE(op.apply(4.0), 0.25);
    QCOMPARE(op.apply(1.0), 1.0);
}

void TestEngine::testInverseZero() {
    Inverse op;
    QVERIFY_THROWS_EXCEPTION(std::domain_error, op.apply(0.0));
}

void TestEngine::testLogarithmNatural() {
    Logarithmic op(LogBase::NATURAL);
    QVERIFY(qAbs(op.apply(M_E) - 1.0) < 1e-10);
    QVERIFY(qAbs(op.apply(1.0) - 0.0) < 1e-10);
}

void TestEngine::testLogarithmBase10() {
    Logarithmic op(LogBase::TEN);
    QVERIFY(qAbs(op.apply(100.0) - 2.0) < 1e-10);
    QVERIFY(qAbs(op.apply(1.0)   - 0.0) < 1e-10);
}

void TestEngine::testLogarithmInvalid() {
    Logarithmic op(LogBase::NATURAL);
    QVERIFY_THROWS_EXCEPTION(std::domain_error, op.apply(0.0));
    QVERIFY_THROWS_EXCEPTION(std::domain_error, op.apply(-1.0));
}

void TestEngine::testSine() {
    AngleUnit unit = AngleUnit::RADIANS;
    Trigonometric op("sin", [](double x){ return std::sin(x); }, &unit);
    QVERIFY(qAbs(op.apply(0.0)) < 1e-10);
    QVERIFY(qAbs(op.apply(M_PI / 2.0) - 1.0) < 1e-10);
}

// ── Testes de Memory ──────────────────────────────────────────────────────────

void TestEngine::testMemoryOperators() {
    Memory mem;
    QVERIFY(!mem.hasValue());

    mem.store(10.0);
    QVERIFY(mem.hasValue());
    QCOMPARE(mem.recall(), 10.0);

    mem += 5.0;
    QCOMPARE(mem.recall(), 15.0);

    mem -= 3.0;
    QCOMPARE(mem.recall(), 12.0);

    mem.clear();
    QVERIFY(!mem.hasValue());
}

// ── Testes de CalculatorEngine ────────────────────────────────────────────────

void TestEngine::testEngineFullCalculation() {
    CalculatorEngine eng;
    eng.inputDigit('7');
    eng.setPendingOperation("÷");
    eng.inputDigit('2');
    QString result = eng.calculate();
    QCOMPARE(result, "3.5");
}

void TestEngine::testEngineUnary() {
    CalculatorEngine eng;
    eng.inputDigit('9');
    QString result = eng.applyUnary("²√x");
    QCOMPARE(result, "3");
}

void TestEngine::testEngineToggleSign() {
    CalculatorEngine eng;
    eng.inputDigit('5');
    eng.toggleSign();
    QCOMPARE(eng.currentDisplay(), "-5");
    eng.toggleSign();
    QCOMPARE(eng.currentDisplay(), "5");
}

void TestEngine::testEnginePercentage() {
    CalculatorEngine eng;
    eng.inputDigit('5');
    eng.inputDigit('0');
    eng.percentage();
    QCOMPARE(eng.currentDisplay(), "0.5");
}

void TestEngine::testEngineClear() {
    CalculatorEngine eng;
    eng.inputDigit('9');
    eng.inputDigit('9');
    eng.clear();
    QCOMPARE(eng.currentDisplay(), "0");
}

QTEST_MAIN(TestEngine)
#include "TestEngine.moc"
