# Trabalho 4 - Cobrinha

## Comandos do make

| Comando | O que faz |
|---|---|
| `make` | Compila o jogo e o teste do grafo (otimizado, `-O2`). Igual a `make all`. |
| `make cobra` | Compila só o jogo, gerando o executável `cobra`. |
| `make testa_grafo` | Compila só o teste do grafo, gerando o executável `testa_grafo`. |
| `make debug` | Apaga e recompila tudo com `-g -O0` e com os sanitizers (`-fsanitize=address,undefined`), que detectam erros de memória. |
| `make clean` | Apaga os executáveis `cobra` e `testa_grafo` (e as pastas `.dSYM`, criadas no macOS). |
| `make CC=gcc` | Compila usando o `gcc`. |
| `make CC=clang` | Compila usando o `clang`. |

Por padrão o compilador é o `cc` (o do sistema). O `make CC=...` só é necessário se ele não funcionar.

## Jeitos de executar o jogo

| Comando | O que faz |
|---|---|
| `./cobra` | Jogo normal, pelo teclado (`k` ou `e` vira para a direita, `j` ou `n` para a esquerda). |
| `./cobra rede/rede1` | Jogo controlado pela rede lida do arquivo `rede/rede1` (ou `rede/rede2`). |
| `./cobra treina` | Treina as redes; a melhor de cada geração é gravada na pasta `G/`, criada automaticamente. |
| `./cobra G/0010-000` | Jogo controlado por uma rede gerada no treino (troque pelo nome de um arquivo da pasta `G/`). |

Um segundo argumento define a semente dos números aleatórios, para repetir uma execução. Por exemplo, `./cobra treina 42`. Sem ele, a semente é a hora atual.
