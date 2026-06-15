# Implementação

> [!NOTE]
> Relato do processo de implementação do sistema, incluindo as ferramentas, bibliotecas utilizadas, decisões técnicas e trechos de código relevantes.

---

## Ferramentas e Ambiente

| Ferramenta | Versão | Uso |
|---|---|---|
| **C++** | 17 | Linguagem principal |
| **Qt** | 6.4.2 | Framework de interface gráfica (Widgets) |
| **CMake** | 3.28.3 | Sistema de build |
| **GCC** | 13.3.0 | Compilador |
| **Qt Test** | 6.4.2 | Framework de testes unitários |
| **PlantUML** | 1.2024.6 | Geração dos diagramas UML |
| **Java** | 17 | Execução do PlantUML |

---

## Estrutura de Arquivos Implementada

```
code/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── engine/
│   │   ├── Operation.h / .cpp          ← classe abstrata base
│   │   ├── BinaryOperation.h / .cpp    ← +, -, ×, ÷, xʸ, ʸ√x
│   │   ├── UnaryOperation.h / .cpp     ← sin, cos, x², ln, √, x!, 1/x, ...
│   │   ├── Memory.h / .cpp             ← mc, m+, m-, mr
│   │   └── CalculatorEngine.h / .cpp   ← orquestrador central
│   └── ui/
│       ├── CalculatorButton.h / .cpp   ← QPushButton estilizado
│       ├── CalcDisplay.h / .cpp        ← widget de display
│       ├── StandardPanel.h / .cpp      ← grade de botões normais
│       ├── ScientificPanel.h / .cpp    ← grade científica (herda StandardPanel)
│       └── MainWindow.h / .cpp         ← janela principal
├── resources/
│   ├── style.qss                       ← tema escuro iPhone
│   └── resources.qrc
└── tests/
    ├── CMakeLists.txt
    └── TestEngine.cpp                  ← 15 testes com Qt Test
```

---

## Decisões de Implementação

### 1. Hierarquia de Operações com Polimorfismo Virtual

A classe abstrata `Operation` define o contrato polimórfico via método `execute()` puro virtual:

```cpp
class Operation {
public:
    virtual double execute(double a, double b = 0.0) const = 0;
    QString getName() const;
protected:
    QString name_;
};
```

Cada operação concreta implementa sua própria lógica sem que o chamador precise conhecer o tipo específico. O `CalculatorEngine` despacha qualquer operação pelo nome:

```cpp
std::map<QString, Operation*> ops_;
// ...
double result = ops_.at("÷")->execute(pendingValue_, currentValue_);
```

### 2. Despacho Dinâmico via `std::map`

Em vez de um `switch/case` para cada operação, o engine registra todas no construtor usando `registerOps()`:

```cpp
void CalculatorEngine::registerOps()
{
    auto addOp = [&](std::unique_ptr<Operation> op) {
        Operation* raw = op.get();
        ownedOps_.push_back(std::move(op));
        ops_[raw->getName()] = raw;
    };
    addOp(std::make_unique<Addition>());
    addOp(std::make_unique<Division>());
    // ...
}
```

Isso permite adicionar novas operações sem modificar nenhuma estrutura de controle existente (**Open/Closed Principle**).

### 3. Funções Trigonométricas com `std::function`

A classe `Trigonometric` encapsula qualquer função do tipo `double(double)` via `std::function`, evitando uma subclasse por função:

```cpp
class Trigonometric : public UnaryOperation {
    std::function<double(double)> func_;
    const AngleUnit* angleUnit_;  // ponteiro para o estado do engine
    bool isInverse_;
public:
    double apply(double x) const override {
        if (!isInverse_ && angleUnit_ && *angleUnit_ == AngleUnit::DEGREES)
            x = x * M_PI / 180.0;
        double result = func_(x);
        if (isInverse_ && angleUnit_ && *angleUnit_ == AngleUnit::DEGREES)
            result = result * 180.0 / M_PI;
        return result;
    }
};
```

O ponteiro `const AngleUnit*` mantém referência ao estado vivo do engine — quando o usuário troca de Rad para Grau, todos os objetos `Trigonometric` se adaptam automaticamente.

### 4. Sobrecarga de Operadores

**`Memory::operator+=` e `operator-=`** tornam as operações m+ e m- idiomáticas:

```cpp
Memory& Memory::operator+=(double v) {
    value_    += v;
    hasValue_  = true;
    return *this;
}
// Uso no engine:
void CalculatorEngine::memoryAdd() { memory_ += currentValue_; }
```

**`CalcDisplay::operator<<`** permite uma interface fluente para atualização do display:

```cpp
CalcDisplay& CalcDisplay::operator<<(const QString& value) {
    setValue(value);
    return *this;
}
// Uso no MainWindow:
display_ << engine_->calculate();
```

### 5. Herança na Interface: `ScientificPanel` herda `StandardPanel`

`ScientificPanel` reusa toda a infraestrutura de `StandardPanel` (vetor de botões, layout, sinal `buttonClicked`, método `connectButtons()`) e sobrepõe apenas `buildButtons()`:

```cpp
class ScientificPanel : public StandardPanel {
protected:
    void buildButtons() override;  // adiciona 5 linhas de botões científicos
public:
    void toggle2nd();
    void setAngleUnit(AngleUnit unit);
};
```

### 6. Tratamento de Erros com Exceções

Operações inválidas lançam `std::domain_error`. O engine captura e converte para a string `"Erro"`:

```cpp
QString CalculatorEngine::calculate() {
    try {
        double result = pendingOp_->execute(pendingValue_, currentValue_);
        // ...
    } catch (const std::exception&) {
        pendingOp_  = nullptr;
        setDisplay("Erro");
    }
    return displayStr_;
}
```

### 7. Formatação Inteligente do Resultado

O método `formatResult()` evita representações como `3.00000000` ou `3.5e+00`:

```cpp
QString CalculatorEngine::formatResult(double v) const {
    if (v == std::floor(v) && std::abs(v) < 1e15)
        return QString::number(static_cast<long long>(v));
    if (std::abs(v) >= 1e10 || (std::abs(v) < 1e-6 && v != 0.0))
        return QString::number(v, 'e', 6);
    // Remove zeros à direita
    QString s = QString::number(v, 'f', 10);
    while (s.endsWith('0')) s.chop(1);
    if (s.endsWith('.')) s.chop(1);
    return s;
}
```

### 8. Alternância do Painel Científico

O painel científico expande/recolhe com simples controle de visibilidade e redimensionamento da janela:

```cpp
void MainWindow::toggleScientificMode() {
    isScientific_ = !isScientific_;
    sciPanel_->setVisible(isScientific_);
    int w = isScientific_ ? SCI_WIDTH : STD_WIDTH;
    setFixedWidth(w);
    resize(w, WIN_HEIGHT);
}
```

---

## Como Compilar e Executar

```bash
cd code
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel 4
./CalculadoraIphone
```

### Executar os Testes

```bash
cd code/build
ctest --output-on-failure
```

---

<div align="center">

[Retroceder](projeto.md) | [Avançar](testes.md)

</div>
