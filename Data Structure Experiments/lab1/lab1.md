# 实验一 线性表的基本运算及多项式的算术运算

班级：B230413 姓名：叶桢荣 学号：B23041313 日期：2024 年 10 月 31 日

## 实验目的

1. 掌握线性表的顺序存储和链式存储这两种基本存储结构及应用场景
2. 掌握顺序表和链表的各种基本操作算法
3. 理解线性表应用于多项式的实现算法

## 实验任务

1. 实现线性表的基本操作：
   - 设计并实现顺序表和链表的初始化、插入、删除、查找、输出、销毁等基本操作。
   - 比较顺序存储和链式存储在不同应用场景下的优势和不足。

2. 实现多项式的算术运算：
   - 使用线性表（顺序表或链表）来表示多项式。
   - 实现多项式的创建、输出、相加、相乘等运算。
   - 理解多项式的表示方法和算法实现细节。

3. 实现链表的逆置和排序：
   - 实现单链表的逆置功能，即将链表中的元素顺序颠倒。
   - 实现链表的排序功能，使链表中的元素按指定顺序排列。

## 实验内容

### 完成顺序表的初始化、查找、插入、删除、输出、撤销等操作。

```c
#include <stdlib.h>
#include <stdio.h>

#define ERROR 0
#define OK 1
#define OVERFLOW 2
#define UNDERFLOW 3
#define NOTPRESENT 4
#define DUPLICATE 5

typedef int ElementType;

typedef struct seqlist {
    int n;
    int maxLength;
    ElementType *element;
} SeqList;

ElementType init(SeqList *L, int m) {
    L->maxLength = m;
    L->n = 0;
    L->element = (ElementType *)malloc(m * sizeof(ElementType));
    if (!L->element) return ERROR;
    return OK;
}

ElementType find(SeqList L, int i, ElementType *x) {
    if (i < 1 || i > L.n) return ERROR;
    *x = L.element[i - 1];
    return OK;
}

ElementType insert(SeqList *L, int i, ElementType x) {
    if (i < 1 || i > L->n + 1) return ERROR;
    if (L->n >= L->maxLength) return OVERFLOW;
    for (int j = L->n - 1; j >= i - 1; j--) {
        L->element[j + 1] = L->element[j];
    }
    L->element[i - 1] = x;
    L->n++;
    return OK;
}

ElementType removeElement(SeqList *L, int i, ElementType *x) {
    if (i < 1 || i > L->n) return ERROR;
    *x = L->element[i - 1];
    for (int j = i; j < L->n; j++) {
        L->element[j - 1] = L->element[j];
    }
    L->n--;
    return OK;
}

ElementType output(SeqList L) {
    for (int i = 0; i < L.n; i++) {
        printf("%d\n", L.element[i]);
    }
    return OK;
}

void destroy(SeqList *L) {
    free(L->element);
    L->element = NULL; // 避免悬空指针
}

int main() {
    int i;
    SeqList list;
    init(&list, 10);
    for (i = 0; i < 10; i++) {
        insert(&list, i + 1, i);
    }
    output(list);
    ElementType removed;
    removeElement(&list, 1, &removed); // 修改调用
    output(list);
    destroy(&list);
    return 0;
}
```

#### 模块之间的层次结构

该程序采用了一种简单的模块化设计，每个功能由一个独立的函数实现。这些函数包括：

1. 初始化模块 (`init`)：负责分配内存并初始化顺序表。
2. 查找模块 (`find`)：用于查找并返回顺序表中的指定元素。
3. 插入模块 (`insert`)：负责在顺序表的指定位置插入元素。
4. 删除模块 (`removeElement`)：用于删除顺序表中的指定元素。
5. 输出模块 (`output`)：用于输出顺序表中的所有元素。
6. 销毁模块 (`destroy`)：负责释放顺序表占用的内存。

#### 函数之间的调用关系和数据传递方式

- `main`函数：
  - 调用`init`初始化顺序表。
  - 使用循环调用`insert`在顺序表中插入元素。
  - 调用`output`输出顺序表的内容。
  - 调用`removeElement`删除顺序表中的一个元素。
  - 再次调用`output`输出顺序表的内容。
  - 最后调用`destroy`释放内存。

- 数据传递：
  - 通过指针传递`SeqList`结构体指针，以便在函数内修改顺序表。
  - 对于具体的元素操作（如插入、删除），通过值传递元素类型`ElementType`。

#### 核心算法的C语言源代码及注释

以下是代码中几个核心函数的详细注释：

```c
ElementType init(SeqList *L, int m) {
    // 设置顺序表的最大长度
    L->maxLength = m;
    // 初始化当前元素个数为0
    L->n = 0;
    // 分配内存给元素数组
    L->element = (ElementType *)malloc(m * sizeof(ElementType));
    // 检查内存分配是否成功
    if (!L->element) return ERROR;
    return OK;
}

ElementType insert(SeqList *L, int i, ElementType x) {
    // 检查插入位置是否合法
    if (i < 1 || i > L->n + 1) return ERROR;
    // 检查顺序表是否已满
    if (L->n >= L->maxLength) return OVERFLOW;
    // 从插入位置开始，所有元素后移一位
    for (int j = L->n - 1; j >= i - 1; j--) {
        L->element[j + 1] = L->element[j];
    }
    // 在指定位置插入新元素
    L->element[i - 1] = x;
    // 更新元素个数
    L->n++;
    return OK;
}

ElementType removeElement(SeqList *L, int i, ElementType *x) {
    // 检查删除位置是否合法
    if (i < 1 || i > L->n) return ERROR;
    // 取出要删除的元素
    *x = L->element[i - 1];
    // 从删除位置开始，所有元素前移一位
    for (int j = i; j < L->n; j++) {
        L->element[j - 1] = L->element[j];
    }
    // 更新元素个数
    L->n--;
    return OK;
}
```

#### 时间复杂度和空间复杂度分析

- 时间复杂度：
  - `init`函数：O(1)，只涉及内存分配和简单赋值操作。
  - `insert`函数：O(n)，在最坏情况下，需要移动所有n个元素。
  - `removeElement`函数：O(n)，在最坏情况下，需要移动n-1个元素。
  - `output`函数：O(n)，需要遍历所有元素进行输出。

- 空间复杂度：
  - 整个顺序表使用了动态分配的数组，其空间复杂度为O(m)，其中m是顺序表的最大长度。

#### 计算过程

对于每个涉及元素移动的操作（插入和删除），最坏情况下需要移动所有元素，因此其时间复杂度是线性的O(n)。初始化和销毁操作由于只涉及简单的内存分配和释放，其时间复杂度为O(1)。程序的空间复杂度主要取决于顺序表的最大长度，因而为O(m)。

### 完成带表头结点单链表的初始化、查找、插入、删除、输出、撤销等操作。

```c
#include <stdlib.h>
#include <stdio.h>

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

int main() {
    int i;
    int x;
    SingleList list;
    init(&list);
    for (i = 0; i < 10; i++) {
        insert(&list, i - 1, i);
    }
    printf("the linklist is: ");
    output(&list);
    delete(&list, 0, &x);
    printf("\nthe linklist is: ");
    output(&list);
    find(&list, 0, &x);
    printf("\nthe value is: ");
    printf("%d", x);
    destory(&list);
}
```

#### 模块之间的层次结构

1. 初始化模块 (`init`)：创建一个空的单链表。
2. 查找模块 (`find`)：根据索引查找单链表中的元素。
3. 插入模块 (`insert`)：在指定位置插入新元素。
4. 删除模块 (`delete`)：删除指定位置的元素。
5. 输出模块 (`output`)：遍历并输出单链表中的所有元素。
6. 销毁模块 (`destroy`)：释放单链表占用的内存。

#### 函数之间的调用关系和数据传递方式

- `main`函数：
  - 调用`init`初始化链表。
  - 使用循环调用`insert`在链表中插入元素。
  - 调用`output`输出链表的内容。
  - 调用`delete`删除链表中的一个元素。
  - 再次调用`output`输出链表的内容。
  - 调用`find`查找链表中的元素。
  - 最后调用`destroy`释放内存。

- 数据传递：
  - 使用指针传递`SingleList`结构体以修改链表。
  - 通过指针获取或修改链表中的元素。

#### 核心算法的C语言源代码及注释

以下是代码中几个核心函数的详细注释：

```c
ElemType init(SingleList *L) {
    // 初始化一个空的链表，设置头指针为空，长度为0
    L->first = NULL;
    L->n = 0;
    return OK;
}

ElemType find(SingleList *L, int i, ElemType *x) {
    // 查找第i个元素的值
    Node *p;
    int j;
    // 检查索引是否有效
    if (i < 0 || i > L->n - 1) {
        return ERROR;
    }
    // 从头开始遍历到第i个元素
    p = L->first;
    for (j = 0; j < i; j++) {
        p = p->link;
    }
    // 返回第i个元素的值
    *x = p->element;
    return OK;
}

ElemType insert(SingleList *L, int i, ElemType x) {
    // 在第i个位置插入新元素
    Node *p, *q;
    int j;
    // 检查插入位置是否有效
    if (i < -1 || i > L->n - 1) {
        return ERROR;
    }
    // 遍历到第i个元素
    p = L->first;
    for (j = 0; j < i; j++) {
        p = p->link;
    }
    // 分配新节点
    q = (Node *) malloc(sizeof(Node));
    if (q == NULL) {
        return OVERFLOW;
    }
    // 设置新节点的数据
    q->element = x;
    // 插入新节点
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
    // 删除第i个元素
    Node *p, *q;
    int j;
    // 检查删除位置是否有效
    if (i < 0 || i > L->n - 1) {
        return ERROR;
    }
    // 遍历到第i-1个节点
    p = L->first;
    for (j = 0; j < i - 1; j++) {
        p = p->link;
    }
    // 删除第i个节点
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
    // 输出链表中的所有元素
    Node *p = L->first;
    while (p != NULL) {
        printf("%d ", p->element);
        p = p->link;
    }
    printf("\n");
    return OK;
}

void destroy(SingleList *L) {
    // 销毁链表，释放所有节点
    Node *p;
    while (L->first) {
        p = L->first->link;
        free(L->first);
        L->first = p;
    }
}
```

#### 时间复杂度和空间复杂度分析

- 时间复杂度：
  - `init`函数：O(1)，只涉及简单赋值操作。
  - `find`函数：O(n)，需要遍历链表到第i个元素。
  - `insert`函数：O(n)，需要遍历到插入位置。
  - `delete`函数：O(n)，需要遍历到删除位置。
  - `output`函数：O(n)，需要遍历链表输出所有元素。
  - `destroy`函数：O(n)，需要遍历链表释放所有节点。

- 空间复杂度：
  - 每个节点需要固定的内存空间，空间复杂度为O(n)，其中n是链表中的节点数。

#### 计算过程

在操作链表时，所有涉及遍历链表的操作（如查找、插入、删除）都需要线性时间，即O(n)，因为在最坏情况下需要访问链表中的每个节点。初始化和销毁操作分别涉及常量时间和线性时间的内存分配和释放。链表本身是动态分配的，空间复杂度为O(n)。

### 实现单链表的逆置操作

```c
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

int main() {
    int i;
    int x;
    SingleList list;
    init(&list);
    for (i = 0; i < 10; i++) {
        insert(&list, i - 1, i);
    }
    output(&list);
    reverse(&list);
    output(&list);
    return 0;
}
```

#### 逆置操作的实现

逆置操作的核心在于遍历链表，并逐一将节点插入到新链表的头部，这样遍历完成后，链表就被逆置了。

#### 核心代码和注释

```c
void reverse(SingleList *L) {
    Node *p, *q;
    // 初始化指针p为链表的第一个节点
    p = L->first;
    // 将链表头指针置为空，准备构建新链表
    L->first = NULL;
    // 遍历原链表
    while (p != NULL) {
        // 保存当前节点
        q = p;
        // 前进到下一个节点
        p = p->link;
        // 当前节点指向新链表的头部
        q->link = L->first;
        // 更新新链表的头部为当前节点
        L->first = q;
    }
}
```

#### 操作步骤

1. 初始化：
   - `p`指向链表的第一个节点。
   - `L->first`置为`NULL`，用于构建逆置后的链表。

2. 遍历和逆置：
   - 通过`while`循环遍历每一个节点。
   - `q`保存当前节点`p`。
   - 移动`p`到下一个节点。
   - 将`q`（当前节点）链接到新链表的头部。
   - 更新新链表的头部为`q`。

3. 完成逆置：
   - 循环结束后，`L->first`指向逆置后的链表。

#### 时间复杂度和空间复杂度分析

- 时间复杂度：O(n)
  - 该操作需要遍历整个链表一次，因此时间复杂度为O(n)，其中n是链表中的节点数。

- 空间复杂度：O(1)
  - 逆置操作在链表节点内部修改链接，不需要额外的存储空间（除了几个临时指针），因此空间复杂度为O(1)。

### 实现将单链表排序成有序单链表的操作

```c
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
```

#### 冒泡排序算法实现

冒泡排序是一种稳定的排序算法，虽然时间复杂度为 \(O(n^2)\)，但对于链表的实现较为简单。以下是排序函数的实现：

#### 核心代码和注释

```c
void sort(SingleList *L) {
    // 检查链表是否为空或只有一个元素，若是则无需排序
    if (L->first == NULL || L->first->link == NULL) {
        return;
    }
    int swapped;
    Node *p;
    Node *q = NULL; // 用于标记每次冒泡的终止位置
    do {
        swapped = 0; // 标志位，用于检测是否发生了交换
        p = L->first;
        while (p->link != q) { // 遍历链表直到q
            if (p->element > p->link->element) {
                // 如果当前元素大于下一个元素，则交换
                int temp = p->element;
                p->element = p->link->element;
                p->link->element = temp;
                swapped = 1; // 发生了交换，设置标志位
            }
            p = p->link; // 移动到下一个节点
        }
        q = p; // 更新q到最后一个已排序位置
    } while (swapped); // 如果未发生交换，说明已排序完成
}
```

#### 操作步骤

1. 初始检查：
   - 如果链表为空或只有一个节点，则直接返回。

2. 冒泡排序：
   - 使用一个`do-while`循环来重复执行排序操作，直到没有交换发生。
   - 在每次遍历中，比较相邻节点的值并交换不符合顺序的节点。
   - `swapped`标志用于指示在一轮遍历中是否发生了交换操作。
   - `q`指针用来标记每轮排序的结束位置，以减少不必要的比较。

3. 终止条件：
   - 当一整轮遍历没有发生任何交换时，排序结束。

#### 时间复杂度和空间复杂度分析

- 时间复杂度：O(n^2)
  - 冒泡排序在最坏情况下需要进行 \(n^2/2\) 次比较和交换。

- 空间复杂度：O(1)
  - 排序在原地进行，不需要额外的存储空间，除了几个临时变量。

#### 代码示例

在`main`函数中演示了如何使用`sort`函数对链表进行排序：

```c
int main() {
    SingleList list;
    init(&list);
    
    // 插入奇数
    for (int i = 1; i < 20; i += 2) {
        insert(&list, -1, i);
    }
    // 插入偶数
    for (int i = 2; i <= 20; i += 2) {
        insert(&list, -1, i);
    }

    printf("Original List:\n");
    output(&list);
    
    reverse(&list);
    printf("Reversed List:\n");
    output(&list);
    
    sort(&list);
    printf("Sorted List:\n");
    output(&list);
    
    destroy(&list);
    return 0;
}
```

### 实现一元多项式的创建、输出、撤销以及两个一元多项式相加和相乘

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct pNode {
    int coef;           // 系数
    int exp;            // 指数
    struct pNode *link; // 指向下一个节点
} PNode;

typedef struct polynominal {
    PNode *head;        // 指向多项式链表的头节点
} Polynominal;

PNode* createNode(int coef, int exp) {
    PNode *node = (PNode *)malloc(sizeof(PNode));
    node->coef = coef;
    node->exp = exp;
    node->link = NULL;
    return node;
}

Polynominal* createPolynominal() {
    Polynominal *poly = (Polynominal *)malloc(sizeof(Polynominal));
    poly->head = NULL;
    return poly;
}

void insertTerm(Polynominal *poly, int coef, int exp) {
    PNode *newNode = createNode(coef, exp);
    if (!poly->head || poly->head->exp < exp) {
        newNode->link = poly->head;
        poly->head = newNode;
    } else {
        PNode *current = poly->head;
        PNode *prev = NULL;
        while (current && current->exp > exp) {
            prev = current;
            current = current->link;
        }
        if (current && current->exp == exp) {
            current->coef += coef;
            free(newNode);
        } else {
            newNode->link = current;
            if (prev) {
                prev->link = newNode;
            }
        }
    }
}

void printPolynominal(Polynominal *poly) {
    PNode *current = poly->head;
    while (current) {
        printf("%dx^%d", current->coef, current->exp);
        if (current->link) {
            printf(" + ");
        }
        current = current->link;
    }
    printf("\n");
}

void deletePolynominal(Polynominal *poly) {
    PNode *current = poly->head;
    while (current) {
        PNode *temp = current;
        current = current->link;
        free(temp);
    }
    free(poly);
}

Polynominal* addPolynominals(Polynominal *poly1, Polynominal *poly2) {
    Polynominal *result = createPolynominal();
    PNode *p1 = poly1->head;
    PNode *p2 = poly2->head;
    while (p1 && p2) {
        if (p1->exp > p2->exp) {
            insertTerm(result, p1->coef, p1->exp);
            p1 = p1->link;
        } else if (p1->exp < p2->exp) {
            insertTerm(result, p2->coef, p2->exp);
            p2 = p2->link;
        } else {
            int sumCoef = p1->coef + p2->coef;
            if (sumCoef != 0) {
                insertTerm(result, sumCoef, p1->exp);
            }
            p1 = p1->link;
            p2 = p2->link;
        }
    }
    while (p1) {
        insertTerm(result, p1->coef, p1->exp);
        p1 = p1->link;
    }
    while (p2) {
        insertTerm(result, p2->coef, p2->exp);
        p2 = p2->link;
    }
    return result;
}

Polynominal* multiplyPolynominals(Polynominal *poly1, Polynominal *poly2) {
    Polynominal *result = createPolynominal();
    for (PNode *p1 = poly1->head; p1; p1 = p1->link) {
        for (PNode *p2 = poly2->head; p2; p2 = p2->link) {
            int coef = p1->coef * p2->coef;
            int exp = p1->exp + p2->exp;
            insertTerm(result, coef, exp);
        }
    }
    return result;
}

int main() {
    Polynominal *poly1 = createPolynominal();
    insertTerm(poly1, 3, 4);
    insertTerm(poly1, 5, 2);
    insertTerm(poly1, 6, 0);

    Polynominal *poly2 = createPolynominal();
    insertTerm(poly2, 2, 3);
    insertTerm(poly2, 4, 2);
    insertTerm(poly2, 1, 0);

    printf("Poly1: ");
    printPolynominal(poly1);

    printf("Poly2: ");
    printPolynominal(poly2);

    Polynominal *sum = addPolynominals(poly1, poly2);
    printf("Sum: ");
    printPolynominal(sum);

    Polynominal *product = multiplyPolynominals(poly1, poly2);
    printf("Product: ");
    printPolynominal(product);

    deletePolynominal(poly1);
    deletePolynominal(poly2);
    deletePolynominal(sum);
    deletePolynominal(product);

    return 0;
}
```

#### 主要结构和函数

1. 数据结构：
   - `PNode`：表示多项式中的一个项，包含系数、指数和指向下一项的指针。
   - `Polynominal`：表示整个多项式，包含指向头节点的指针。

2. 函数实现：

   - 创建节点和多项式：
     - `createNode(int coef, int exp)`：创建一个新的节点，包含指定的系数和指数。
     - `createPolynominal()`：初始化一个空的多项式。

   - 插入项：
     - `insertTerm(Polynominal *poly, int coef, int exp)`：在多项式中插入一个新项。如果该指数的项已经存在，则更新其系数。

   - 输出多项式：
     - `printPolynominal(Polynominal *poly)`：以可读的格式输出多项式。

   - 删除多项式：
     - `deletePolynominal(Polynominal *poly)`：释放多项式占用的内存。

   - 多项式相加：
     - `addPolynominals(Polynominal *poly1, Polynominal *poly2)`：返回两个多项式相加后的新多项式。

   - 多项式相乘：
     - `multiplyPolynominals(Polynominal *poly1, Polynominal *poly2)`：返回两个多项式相乘后的新多项式。

#### 详细代码和注释

插入项

```c
void insertTerm(Polynominal *poly, int coef, int exp) {
    PNode *newNode = createNode(coef, exp);
    if (!poly->head || poly->head->exp < exp) {
        newNode->link = poly->head;
        poly->head = newNode;
    } else {
        PNode *current = poly->head;
        PNode *prev = NULL;
        while (current && current->exp > exp) {
            prev = current;
            current = current->link;
        }
        if (current && current->exp == exp) {
            current->coef += coef;
            free(newNode);
        } else {
            newNode->link = current;
            if (prev) {
                prev->link = newNode;
            }
        }
    }
}
```

- 逻辑：该函数确保多项式按指数降序排列。如果同指数的项已存在，则更新其系数；否则，在正确位置插入新项。

多项式相加

```c
Polynominal* addPolynominals(Polynominal *poly1, Polynominal *poly2) {
    Polynominal *result = createPolynominal();
    PNode *p1 = poly1->head;
    PNode *p2 = poly2->head;
    while (p1 && p2) {
        if (p1->exp > p2->exp) {
            insertTerm(result, p1->coef, p1->exp);
            p1 = p1->link;
        } else if (p1->exp < p2->exp) {
            insertTerm(result, p2->coef, p2->exp);
            p2 = p2->link;
        } else {
            int sumCoef = p1->coef + p2->coef;
            if (sumCoef != 0) {
                insertTerm(result, sumCoef, p1->exp);
            }
            p1 = p1->link;
            p2 = p2->link;
        }
    }
    while (p1) {
        insertTerm(result, p1->coef, p1->exp);
        p1 = p1->link;
    }
    while (p2) {
        insertTerm(result, p2->coef, p2->exp);
        p2 = p2->link;
    }
    return result;
}
```

- 逻辑：同时遍历两个多项式，根据指数大小插入到结果多项式中，处理相同指数时累加系数。

多项式相乘

```c
Polynominal* multiplyPolynominals(Polynominal *poly1, Polynominal *poly2) {
    Polynominal *result = createPolynominal();
    for (PNode *p1 = poly1->head; p1; p1 = p1->link) {
        for (PNode *p2 = poly2->head; p2; p2 = p2->link) {
            int coef = p1->coef * p2->coef;
            int exp = p1->exp + p2->exp;
            insertTerm(result, coef, exp);
        }
    }
    return result;
}
```

- 逻辑：通过嵌套循环，将每一项与另一多项式的每一项相乘，并将结果插入到结果多项式中。

## 实验过程描述

（列出实验所用的测试用例和相应的程序运行结果，程序运行结果从屏幕截图表示，总结本次实验，包括对测试结果的分析、测试和调试过程遇到问题的回顾和分析、软件设计与实现的经验和体会、进一步改进的设想。）

### 测试结果分析

在本次实验中，我们实现了线性表的基本操作、多项式的基本运算以及链表的逆置和排序功能。经过测试，所有功能均正常运行，符合预期结果。

- 顺序表操作：初始化、插入、删除、查找和输出功能均正常。尤其是删除和插入操作，能够正确地调整顺序表的元素顺序。
- 链表操作：单链表的基本操作，包括插入、删除、查找、输出等，均能正常运行。链表逆置和排序功能也表现良好。
- 多项式运算：多项式的创建、相加和相乘功能实现了预期的多项式代数运算，结果正确。

### 测试和调试过程中遇到的问题

1. 内存管理问题：
   - 在链表操作和多项式操作中，内存管理是一个常见问题。最初实现中曾出现内存泄漏问题，尤其是在删除操作和多项式销毁过程中。通过仔细检查每一个 `malloc` 和 `free` 的对应关系，确保在释放节点后不再访问其内容，解决了该问题。

2. 边界条件处理：
   - 插入和删除操作的边界条件（如头部和尾部的处理）需要特别注意。在链表实现中，尤其是对空链表的操作，易出现错误。通过增加边界条件的检查和测试（如插入到空链表、删除唯一元素等），确保程序的稳健性。

3. 指数排序问题：
   - 在多项式的插入过程中，确保多项式按照指数降序排列是关键。初始实现中未考虑完全，导致输出不符合预期。通过在插入时对指数进行检查和调整，解决了这一问题。

### 软件设计与实现的经验和体会

1. 模块化设计：
   - 将每个功能模块化有助于代码的可读性和可维护性。每个操作都有独立的函数实现，便于调试和后续扩展。

2. 内存管理：
   - 在C语言中进行动态内存分配时，严格管理内存的分配和释放至关重要。通过适当的内存管理，可以避免内存泄漏和悬空指针问题。

3. 注释和文档：
   - 详细的代码注释和文档对理解和维护代码非常有帮助。在实现过程中，通过注释明确每一步的目的和逻辑，能够更快速地发现和解决问题。

4. 边界测试：
   - 对程序进行全面的边界测试，尤其是极端情况（如空列表、单节点列表等），可以确保程序的鲁棒性。

### 进一步改进的设想

1. 性能优化：
   - 对于链表排序等复杂度较高的操作，可以考虑使用更高效的算法（如归并排序），以提高性能。

2. 通用性增强：
   - 当前实现假定多项式的系数和指数均为整数，可以扩展以支持浮点数或其他数据类型，以提高通用性。

3. 用户交互改进：
   - 添加用户交互界面，允许用户动态输入多项式和操作命令，以提高程序的实用性。

4. 错误处理：
   - 增强错误处理机制，提供更详细的错误信息和恢复措施，以提高程序的健壮性。

通过本次实验，我们对线性表和多项式的基本运算有了更深入的理解，并积累了处理链表和内存管理的实践经验。未来可以在此基础上进一步优化和扩展功能。

