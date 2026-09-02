#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef int dado_t;

typedef struct lista *Lista;
struct lista {
    int cap;
    int pri;
    int num;
    dado_t *dad;
};

static void l_realoca(Lista l)
{
    int novacap = l->cap == 0 ? 1 : l->cap * 2;
    dado_t *novo = malloc(novacap * sizeof(dado_t));
    for (int i = 0; i < l->num; i++)
        novo[i] = l->dad[(l->pri + i) % l->cap];
    free(l->dad);
    l->dad = novo;
    l->cap = novacap;
    l->pri = 0;
}

bool l_ins_ini(Lista l, dado_t d)
{
    if (l->num >= l->cap) l_realoca(l);
    int ind = l->pri == 0 ? l->cap - 1 : l->pri - 1;
    l->dad[ind] = d;
    l->pri = ind;
    l->num++;
    return true;
}

bool l_ins_fim(Lista l, dado_t d)
{
    if (l->num >= l->cap) l_realoca(l);
    int ind = (l->pri + l->num) % l->cap;
    l->dad[ind] = d;
    l->num++;
    return true;
}

bool l_ins_pos(Lista l, dado_t d, int pos)
{
    if (pos < 0 || pos > l->num) return false;
    if (l->num >= l->cap) l_realoca(l);
    for (int i = l->num; i > pos; i--)
        l->dad[(l->pri + i) % l->cap] = l->dad[(l->pri + i - 1) % l->cap];
    l->dad[(l->pri + pos) % l->cap] = d;
    l->num++;
    return true;
}

bool l_rem_ini(Lista l, dado_t *d)
{
    if (l->num == 0) return false;
    *d = l->dad[l->pri];
    l->pri = (l->pri + 1) % l->cap;
    l->num--;
    return true;
}

bool l_rem_fim(Lista l, dado_t *d)
{
    if (l->num == 0) return false;
    int ind = (l->pri + l->num - 1) % l->cap;
    *d = l->dad[ind];
    l->num--;
    return true;
}

bool l_rem_pos(Lista l, int pos, dado_t *d)
{
    if (pos < 0 || pos >= l->num) return false;
    *d = l->dad[(l->pri + pos) % l->cap];
    for (int i = pos; i < l->num - 1; i++)
        l->dad[(l->pri + i) % l->cap] = l->dad[(l->pri + i + 1) % l->cap];
    l->num--;
    return true;
}