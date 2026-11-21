# Trabalho 2 - TAD string

## Comandos do make

| Comando | O que faz |
|---|---|
| `make` | Compila o programa de testes, com `-g`, e gera o executável `teste_str`. Igual a `make teste_str`. |
| `make run` | Compila, se for preciso, e executa os testes. |
| `make clean` | Apaga o executável `teste_str` (e a pasta `teste_str.dSYM`, criada no macOS). |
| `make CC=gcc` | Compila usando o `gcc`. |
| `make CC=clang` | Compila usando o `clang`. |

Por padrão o compilador é o `cc` (o do sistema). O `make CC=...` só é necessário se ele não funcionar.
