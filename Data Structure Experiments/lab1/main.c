#include <stdio.h>
#include <stdlib.h>

#define ERROR 0
#define OK 1
#define OVERFLOW 2
#define UNDERFLOW 3
#define NOTPRESENT 4
#define DUPLICATE 5

typedef int ElemType;

typedef struct node {
    ElemType element;
    struct node *link;
} Node;

typedef struct singleList {
    Node *first;
    int n;
} SingleList;

ElemType init(SingleList *L) {
    L->first = NULL;
    L->n = 0;
    return OK;
}

ElemType find(SingleList *L, int i, ElemType *x) {
    Node *p;
    int j;
    if (i < 0 || i > L->n - 1) {
        return ERROR;
    }
    p = L->first;
    for (j = 0; j < i; j++) {
        p = p->link;
    }
    *x = p->element;
    return OK;
}

ElemType insert(SingleList *L, int i, ElemType x) {
    Node *p, *q;
    int j;
    if (i < -1 || i > L->n - 1) {
        return ERROR;
    }
    p = L->first;
    for (j = 0; j < i; j++) {
        p = p->link;
    }
    q = (Node *) malloc(sizeof(Node));
    if (q == NULL) {
        return OVERFLOW;
    }
    q->element = x;
    if (i > -1) {
        q->link = p->link;
        p->link = q;
    } else {
        q->link = L->first;
        L->first = q;
    }
    L->n++;
    return OK;
}

ElemType delete(SingleList *L, int i, ElemType *x) {
    Node *p, *q;
    int j;
    if (i < 0 || i > L->n - 1) {
        return ERROR;
    }
    p = L->first;
    for (j = 0; j < i - 1; j++) {
        p = p->link;
    }
    if (i > 0) {
        q = p->link;
        p->link = q->link;
    } else {
        q = L->first;
        L->first = q->link;
    }
    *x = q->element;
    free(q);
    L->n--;
    return OK;
}

ElemType output(SingleList *L) {
    Node *p;
    p = L->first;
    while (p != NULL) {
        printf("%d ", p->element);
        p = p->link;
    }
    printf("\n");
    return OK;
}

void destory(SingleList *L) {
    Node *p;
    while (L->first) {
        p = L->first->link;
        free(L->first);
        L->first = p;
    }
}

void reverse(SingleList *L) {
    Node *p, *q;
    p = L->first;
    L->first = NULL;
    while (p != NULL) {
        q = p;
        p = p->link;
        q->link = L->first;
        L->first = q;
    }
}

void sort(SingleList *L) {
    if (L->first == NULL || L->first->link == NULL) {
        return;
    }
    int swapped;
    Node *p;
    Node *q = NULL;
    do {
        swapped = 0;
        p = L->first;
        while (p->link != q) {
            if (p->element > p->link->element) {
                int temp = p->element;
                p->element = p->link->element;
                p->link->element = temp;
                swapped = 1;
            }
            p = p->link;
        }
        q = p;
    } while (swapped);
}

/**
 * This is a commit
 * @return
 */
int main() {
    int i;
    int x;
    SingleList list;
    init(&list);
    for (i = 1; i < 20; i += 2) {
        insert(&list, -1, i);
    }
    for (i = 2; i <= 20; i += 2) {
        insert(&list, -1, i);
    }

    output(&list);
    reverse(&list);
    output(&list);

    sort(&list);
    output(&list);

    destory(&list);

    return 0;
}
