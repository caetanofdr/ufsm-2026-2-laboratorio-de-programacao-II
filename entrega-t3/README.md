# Trabalho 3 - Calculadora

## Comandos do make

| Comando | O que faz |
|---|---|
| `make` | Compila o programa (otimizado, `-O2`) e gera o executável `programa`. Igual a `make programa`. |
| `make debug` | Compila com `-g` e com os sanitizers (`-fsanitize=address,undefined`), que detectam erros de memória. Também gera o executável `programa`. |
| `make clean` | Apaga o executável `programa`. |
| `make CC=gcc` | Compila usando o `gcc`. |
| `make CC=clang` | Compila usando o `clang`. |

Por padrão o compilador é o `cc` (o do sistema). O `make CC=...` só é necessário se ele não funcionar.

Antes de executar, deixe na mesma pasta do programa um arquivo `input.txt`, com uma expressão por linha. O resultado é gravado em `output.txt`, uma resposta por linha.
