# Trabalho 1 - Invasores do Espaço

## Comandos do make

| Comando | O que faz |
|---|---|
| `make` | Compila o programa e gera o executável `main`. Igual a `make main`. |
| `make run` | Compila, se for preciso, e executa o programa. |
| `make debug` | Compila com `-g` e com os sanitizers (`-fsanitize=address,undefined`), que detectam erros de memória. Também gera o executável `main`. |
| `make clean` | Apaga o executável `main` (e a pasta `main.dSYM`, criada no macOS). |
| `make CC=gcc` | Compila usando o `gcc`. |
| `make CC=clang` | Compila usando o `clang`. |

Por padrão o compilador é o `cc` (o do sistema). O `make CC=...` só é necessário se ele não funcionar.
