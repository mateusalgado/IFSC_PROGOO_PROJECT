# Testes

> [!NOTE]
> Relato do processo de testes do sistema, especificando os casos de teste realizados e os resultados obtidos.

---

## Estratégia de Testes

O projeto adota dois níveis de testes:

1. **Testes unitários automatizados** (`code/tests/TestEngine.cpp`) — testam a engine isolada da UI usando o framework **Qt Test**.
2. **Testes funcionais manuais** — verificam o comportamento visual e de interação da interface Qt.

---

## Testes Unitários — Engine

Os testes são executados com `ctest` a partir do diretório de build. Todos os 15 casos passaram com sucesso:

```
Test project /code/build
    Start 1: EngineTests
1/1 Test #1: EngineTests ......................   Passed    0.08 sec

100% tests passed, 0 tests failed out of 1
```

### Tabela de Casos de Teste Unitários

| ID | Função Testada | Entrada | Resultado Esperado | Resultado Obtido | Status |
|---|---|---|---|---|---|
| UT01 | `Addition::execute` | 3 + 5 | 8.0 | 8.0 | ✅ Passou |
| UT02 | `Addition::execute` | -1 + 1 | 0.0 | 0.0 | ✅ Passou |
| UT03 | `Subtraction::execute` | 10 - 4 | 6.0 | 6.0 | ✅ Passou |
| UT04 | `Subtraction::execute` | 0 - 5 | -5.0 | -5.0 | ✅ Passou |
| UT05 | `Multiplication::execute` | 3 × 7 | 21.0 | 21.0 | ✅ Passou |
| UT06 | `Multiplication::execute` | -2 × 3 | -6.0 | -6.0 | ✅ Passou |
| UT07 | `Division::execute` | 10 ÷ 2 | 5.0 | 5.0 | ✅ Passou |
| UT08 | `Division::execute` | 7 ÷ 2 | 3.5 | 3.5 | ✅ Passou |
| UT09 | `Division::execute` — exceção | 5 ÷ 0 | `std::domain_error` | lançado | ✅ Passou |
| UT10 | `NthRoot::execute` | ³√8 | 2.0 | 2.0 | ✅ Passou |
| UT11 | `Power::execute` | 2¹⁰ | 1024.0 | 1024.0 | ✅ Passou |
| UT12 | `Factorial::apply` | 5! | 120.0 | 120.0 | ✅ Passou |
| UT13 | `Factorial::apply` — exceção | (-1)! | `std::domain_error` | lançado | ✅ Passou |
| UT14 | `Factorial::apply` — exceção | (2.5)! | `std::domain_error` | lançado | ✅ Passou |
| UT15 | `Inverse::apply` | 1/4 | 0.25 | 0.25 | ✅ Passou |
| UT16 | `Inverse::apply` — exceção | 1/0 | `std::domain_error` | lançado | ✅ Passou |
| UT17 | `Logarithmic::apply` (ln) | ln(e) | 1.0 | 1.0 | ✅ Passou |
| UT18 | `Logarithmic::apply` (log₁₀) | log₁₀(100) | 2.0 | 2.0 | ✅ Passou |
| UT19 | `Logarithmic::apply` — exceção | ln(0) | `std::domain_error` | lançado | ✅ Passou |
| UT20 | `Trigonometric::apply` (sin) | sin(0) | 0.0 | 0.0 | ✅ Passou |
| UT21 | `Trigonometric::apply` (sin) | sin(π/2) | 1.0 | 1.0 | ✅ Passou |
| UT22 | `Memory::operator+=` | store(10), +=5 | 15.0 | 15.0 | ✅ Passou |
| UT23 | `Memory::operator-=` | store(15), -=3 | 12.0 | 12.0 | ✅ Passou |
| UT24 | `Memory::clear` | clear() | hasValue=false | false | ✅ Passou |
| UT25 | `CalculatorEngine` — 7÷2= | inputDigit, setPending, calculate | "3.5" | "3.5" | ✅ Passou |
| UT26 | `CalculatorEngine` — ²√9 | inputDigit('9'), applyUnary("²√x") | "3" | "3" | ✅ Passou |
| UT27 | `CalculatorEngine` — +/- | inputDigit('5'), toggleSign | "-5" | "-5" | ✅ Passou |
| UT28 | `CalculatorEngine` — % | input("50"), percentage | "0.5" | "0.5" | ✅ Passou |
| UT29 | `CalculatorEngine` — clear | input("99"), clear | "0" | "0" | ✅ Passou |

---

## Testes Funcionais — Interface

| ID | Cenário | Procedimento | Resultado Esperado | Status |
|---|---|---|---|---|
| FT01 | Modo normal — cálculo básico | Pressionar 7 ÷ 2 = | Display exibe "3.5" | ✅ OK |
| FT02 | Modo normal — encadeamento | 3 + 5 × 2 = | Display exibe "16" (resultado sequencial) | ✅ OK |
| FT03 | Modo normal — limpeza (AC) | Digitar qualquer número, pressionar AC | Display volta a "0" | ✅ OK |
| FT04 | Modo normal — erro | Pressionar 5 ÷ 0 = | Display exibe "Erro" | ✅ OK |
| FT05 | Modo normal — memória | m+ com 5, mc, mr | Recupera 5 após limpar | ✅ OK |
| FT06 | Toggle modo científico | Pressionar botão ⇤ | Painel científico expande à esquerda | ✅ OK |
| FT07 | Modo científico — sin(0) | Modo Rad, digitar 0, pressionar sin | Display exibe "0" | ✅ OK |
| FT08 | Modo científico — sin(90°) | Trocar para Grau, digitar 90, pressionar sin | Display exibe "1" | ✅ OK |
| FT09 | Modo científico — x² | Digitar 5, pressionar x² | Display exibe "25" | ✅ OK |
| FT10 | Modo científico — 2nd | Pressionar 2nd, depois sin | Label muda para asin; sin(1) retorna ~90° | ✅ OK |
| FT11 | Modo científico — π | Pressionar π | Display exibe "3.141592653590" | ✅ OK |
| FT12 | Modo científico — Rand | Pressionar Rand | Display exibe número entre 0 e 1 | ✅ OK |
| FT13 | Modo científico — ln(e) | Pressionar e, pressionar ln | Display exibe "1" | ✅ OK |
| FT14 | Modo científico — 10! | Digitar 10, pressionar x! | Display exibe "3628800" | ✅ OK |
| FT15 | Visibilidade | Redimensionar janela, alternar modos | Layout correto em ambos os modos | ✅ OK |

---

## Cobertura de Conceitos POO nos Testes

| Conceito POO | Testado em | Teste(s) |
|---|---|---|
| **Polimorfismo virtual** | `execute()` de diferentes subclasses | UT01–UT11 |
| **Herança** | `BinaryOperation` e `UnaryOperation` herdam `Operation` | UT01–UT21 |
| **Sobrecarga de operadores** | `Memory::operator+=/-=` | UT22–UT23 |
| **Encapsulamento** | Engine testado só pela API pública | UT25–UT29 |
| **STL** | `std::map` de operações, `std::vector` | UT25–UT29 |
| **Tratamento de exceções** | `std::domain_error` | UT09, UT13, UT14, UT16, UT19 |

---

<div align="center">

[Retroceder](implementacao.md) | [Início](README.md)

</div>
