# Parte 3

## (a) Estruturas de dados

### 1. Vetor fixo de indivíduos em cada espécie

- **Problema:** a `struct espécie` guarda os indivíduos em `indivíduos[MAX_IND_ESP]`, com 500 posições. O vetor de espécies da geração (`espécies[MAX_ESPÉCIES]`) também tem 500.
- **Por que é um problema:** todas as espécies juntas têm 500 indivíduos, mas cada uma reserva 500 lugares. O número de espécies varia de dezenas a quase 500 (no início do treino, quase cada indivíduo forma uma espécie), então quase todo esse espaço fica vazio: com 490 espécies, são 245 000 posições reservadas para 500 indivíduos. Se o limite for ultrapassado, o `assert` aborta o programa.
- **Alternativa:** lista com implementação contígua e alocação dinâmica, que aumenta a capacidade quando enche.
- **Por que melhora:** a memória acompanha o número real de indivíduos, não existe mais limite fixo, e o acesso por posição (usado no sorteio dos pais) continua direto.

### 2. Tabela de inovações com busca linear

- **Problema:** os ids dos neurônios criados por mutação ficam no vetor `inovações[10000]`, e `id_neurônio_entre` procura um par (`id_origem`, `id_destino`) percorrendo o vetor do início.
- **Por que é um problema:** a busca fica mais lenta à medida que o treino avança. Quando o vetor enche, ids novos deixam de ser registrados, e a mesma inovação passa a receber ids diferentes em redes diferentes.
- **Alternativa:** árvore AA (ABB auto-equilibrada) com chave (`id_origem`, `id_destino`).
- **Por que melhora:** a busca percorre no máximo a altura da árvore, próxima de log₂ n (cerca de 14 comparações para 10000 inovações), e a árvore cresce sem limite.

### 3. Busca de neurônio por id percorrendo a rede

- **Problema:** `indivíduo_neurônio_com_id` e `indivíduo_sinapse_com_ids` percorrem todos os neurônios para achar um id. Elas são chamadas para cada sinapse em `compatibilidade` e no cruzamento.
- **Por que é um problema:** comparar duas redes faz uma busca completa para cada sinapse, e essa comparação acontece entre cada filhote e o campeão de cada espécie, milhares de vezes por geração.
- **Alternativa:** em cada indivíduo, uma árvore AA de id do neurônio para posição no grafo (ou uma lista contígua ordenada por id, com busca binária).
- **Por que melhora:** cada busca percorre só a altura da árvore em vez de todos os neurônios.

### 4. Ordenações repetidas

- **Problema:** os indivíduos e as espécies são reordenados com `qsort` várias vezes por geração (`geração_separa_espécies`, `geração_joga`, `espécie_totaliza`, `geração_calcula_descendentes`, `geração_reproduz`), e o código depende disso para que o campeão seja `indivíduos[0]`.
- **Por que é um problema:** a mesma ordem é refeita várias vezes, e se uma ordenação faltar, o "campeão" deixa de ser o melhor sem nenhum erro aparente.
- **Alternativa:** lista ordenada por pontuação, inserindo cada indivíduo na posição certa, ou fila de prioridade (heap) quando só o melhor interessa.
- **Por que melhora:** a própria estrutura mantém a ordem, sem precisar reordenar.

## (b) Mudanças no `grafo.h`

1. **Percorrer todas as arestas de uma vez.** Funções como `g_arestas(g)` e `g_próxima_aresta_do_grafo(g, &origem, &destino)`. Hoje o treinador precisa de um laço pelos nós e outro pelas arestas de cada nó (o próprio código comenta "'Rede' não tem uma função que percorra todas as sinapses").
2. **Copiar um grafo.** `g_copia(g)`. Hoje copiar uma rede é inserir nó por nó e aresta por aresta, testando ciclo a cada aresta.
3. **Identificador estável dos nós.** Um identificador que não mude quando um nó é removido, ou `g_busca_nó(g, dado, compara)`. Assim o treinador não precisaria guardar um `id` próprio e procurá-lo percorrendo a rede.