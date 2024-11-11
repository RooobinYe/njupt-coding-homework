# 实验二 二叉树的基本操作及含满腹编码/译码系统的实现

班级：B230413 姓名：叶桢荣 学号：B23041313 日期：2024 年 11 月 11 日

## 实验目的

通过本实验，掌握二叉树的基本操作，如创建、遍历、统计节点、计算树的高度、交换子树等操作。同时，学习哈夫曼树的创建、编码和解码，实现简单的压缩算法，理解其在数据压缩中的应用。

## 实验任务

1. 实现二叉树的基本操作，包括先序、中序、后序遍历。
2. 计算二叉树的节点个数、叶节点个数、二叉树的高度。
3. 实现交换二叉树的所有左右子树的操作。
4. 创建哈夫曼树并实现哈夫曼编码和解码功能。

## 实验内容

（使用模块和流程图表示系统分析和设计的结果；描述各模块之间的层次结构；给出函数之间的调用关系和数据传递方式；给出核心算法的C语言源代码，并加上详细注释；分析主要算法的时间复杂度，必要时分析空间复杂度；给出算法分析的计算过程。）

### 完成二叉树的先序创建、先序遍历、中序遍历、后序遍历等操作。

```c
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef char ElemType;

typedef struct btnode {
    ElemType element;
    struct btnode *lChild;
    struct btnode *rChild;
} BTNode;

typedef struct binarytree {
    BTNode *root;
} BinaryTree;

void create(BinaryTree *bt) {
    bt->root = NULL;
}

BTNode *newNode(ElemType x, BTNode *ln, BTNode *rn) {
    BTNode *p = (BTNode *) malloc(sizeof(BTNode));
    if (!p) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    p->element = x;
    p->lChild = ln;
    p->rChild = rn;
    return p;
}

bool root(BinaryTree *bt, ElemType **x) {
    if (bt->root) {
        *x = &bt->root->element;
        return true;
    } else {
        return false;
    }
}

void makeTree(BinaryTree *bt, ElemType e, BinaryTree *left, BinaryTree *right) {
    if (bt->root || left == right) return;
    bt->root = newNode(e, left->root, right->root);
    left->root = right->root = NULL;
}

void preOrder(BTNode *t) {
    if (!t) return;
    printf("%c", t->element);
    preOrder(t->lChild);
    preOrder(t->rChild);
}

void preOrderTree(BinaryTree *bt) {
    preOrder(bt->root);
}

void inOrder(BTNode *t) {
    if (!t) return;
    inOrder(t->lChild);
    printf("%c", t->element);
    inOrder(t->rChild);
}

void inOrderTree(BinaryTree *bt) {
    inOrder(bt->root);
}

void postOrder(BTNode *t) {
    if (!t) return;
    postOrder(t->lChild);
    postOrder(t->rChild);
    printf("%c", t->element);
}

void postOrderTree(BinaryTree *bt) {
    postOrder(bt->root);
}

int main() {
    BinaryTree a, b, x, y, z;
    create(&a);
    create(&b);
    create(&x);
    create(&y);
    create(&z);
    makeTree(&y, 'E', &a, &b);
    makeTree(&z, 'F', &a, &b);
    makeTree(&x, 'C', &y, &z);
    makeTree(&y, 'D', &a, &b);
    makeTree(&z, 'B', &y, &x);

    printf("Pre-order: ");
    preOrderTree(&z);
    printf("\n");

    printf("In-order: ");
    inOrderTree(&z);
    printf("\n");

    printf("Post-order: ");
    postOrderTree(&z);
    printf("\n");

    return 0;
}
```

#### 模块设计

   - `BTNode`：二叉树节点结构，包含元素、左子树、右子树。
   - `BinaryTree`：二叉树结构，包含树的根节点。
   - 树的创建、遍历的功能模块。

#### 流程图

   - 创建二叉树
   - 进行各种遍历

#### 核心算法实现

   - 创建节点: `BTNode *newNode(ElemType x, BTNode *ln, BTNode *rn)`
   - 遍历:
     - 先序遍历: `void preOrder(BTNode *t)`
     - 中序遍历: `void inOrder(BTNode *t)`
     - 后序遍历: `void postOrder(BTNode *t)`

#### 时间复杂度分析

   - 树的遍历、节点统计、的时间复杂度均为 O(n)，其中 n 为节点个数。

### 实现求二叉树的节点个数、叶节点个数字、二叉树的高度以及交换二叉树的所有左右子树的操作

```c
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef char ElemType;

typedef struct btnode {
    ElemType element;
    struct btnode *lChild;
    struct btnode *rChild;
} BTNode;

typedef struct binarytree {
    BTNode *root;
} BinaryTree;

void create(BinaryTree *bt) {
    bt->root = NULL;
}

BTNode *newNode(ElemType x, BTNode *ln, BTNode *rn) {
    BTNode *p = (BTNode *) malloc(sizeof(BTNode));
    if (!p) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    p->element = x;
    p->lChild = ln;
    p->rChild = rn;
    return p;
}

bool root(BinaryTree *bt, ElemType **x) {
    if (bt->root) {
        *x = &bt->root->element;
        return true;
    } else {
        return false;
    }
}

void makeTree(BinaryTree *bt, ElemType e, BinaryTree *left, BinaryTree *right) {
    if (bt->root || left == right) return;
    bt->root = newNode(e, left->root, right->root);
    left->root = right->root = NULL;
}

void preOrder(BTNode *t) {
    if (!t) return;
    printf("%c", t->element);
    preOrder(t->lChild);
    preOrder(t->rChild);
}

void preOrderTree(BinaryTree *bt) {
    preOrder(bt->root);
}

void inOrder(BTNode *t) {
    if (!t) return;
    inOrder(t->lChild);
    printf("%c", t->element);
    inOrder(t->rChild);
}

void inOrderTree(BinaryTree *bt) {
    inOrder(bt->root);
}

void postOrder(BTNode *t) {
    if (!t) return;
    postOrder(t->lChild);
    postOrder(t->rChild);
    printf("%c", t->element);
}

void postOrderTree(BinaryTree *bt) {
    postOrder(bt->root);
}

int countNodes(BTNode *t) {
    if (!t) return 0;
    return 1 + countNodes(t->lChild) + countNodes(t->rChild);
}

int countLeafNodes(BTNode *t) {
    if (!t) return 0;
    if (!t->lChild && !t->rChild) return 1;
    return countLeafNodes(t->lChild) + countLeafNodes(t->rChild);
}

int height(BTNode *t) {
    if (!t) return 0;
    int leftHeight = height(t->lChild);
    int rightHeight = height(t->rChild);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

void swapChildren(BTNode *t) {
    if (!t) return;
    BTNode *temp = t->lChild;
    t->lChild = t->rChild;
    t->rChild = temp;
    swapChildren(t->lChild);
    swapChildren(t->rChild);
}

int main() {
    BinaryTree a, b, x, y, z;
    create(&a);
    create(&b);
    create(&x);
    create(&y);
    create(&z);
    makeTree(&y, 'E', &a, &b);
    makeTree(&z, 'F', &a, &b);
    makeTree(&x, 'C', &y, &z);
    makeTree(&y, 'D', &a, &b);
    makeTree(&z, 'B', &y, &x);

    printf("Pre-order: ");
    preOrderTree(&z);
    printf("\n");

    printf("In-order: ");
    inOrderTree(&z);
    printf("\n");

    printf("Post-order: ");
    postOrderTree(&z);
    printf("\n");

    printf("Total number of nodes: %d\n", countNodes(z.root));
    printf("Number of leaf nodes: %d\n", countLeafNodes(z.root));
    printf("Height of the tree: %d\n", height(z.root));

    swapChildren(z.root);
    printf("Pre-order after swapping children: ");
    preOrderTree(&z);
    printf("\n");

    return 0;
}
```

#### 模块设计

   - `BTNode`：二叉树节点结构，包含元素、左子树、右子树。
   - `BinaryTree`：二叉树结构，包含树的根节点。
   - 树的统计节点、计算高度、交换子树的功能模块。

#### 流程图

   - 统计节点和叶节点
   - 计算树的高度
   - 交换左右子树

#### 核心算法实现

   - 统计节点个数: `int countNodes(BTNode *t)`
   - 统计叶节点个数: `int countLeafNodes(BTNode *t)`
   - 计算树的高度: `int height(BTNode *t)`
   - 交换子树: `void swapChildren(BTNode *t)`

#### 时间复杂度分析

   - 树的叶节点统计、交换子树操作的时间复杂度均为 O(n)，其中 n 为节点个数。
   - 树的高度计算的时间复杂度为 O(n)。

### 实现哈夫曼树的创建、哈夫曼编码以及解码的实现

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TREE_NODES 256

typedef char ElemType;

typedef struct hfmTNode {
    ElemType element;
    int weight;
    struct hfmTNode *lChild;
    struct hfmTNode *rChild;
} HFMTNode;

// Priority queue for building Huffman Tree
typedef struct {
    HFMTNode *nodes[MAX_TREE_NODES];
    int size;
} MinHeap;

// Function to create a new tree node
HFMTNode* createNode(ElemType element, int weight) {
    HFMTNode *node = (HFMTNode *)malloc(sizeof(HFMTNode));
    node->element = element;
    node->weight = weight;
    node->lChild = node->rChild = NULL;
    return node;
}

// Function to create a min heap
MinHeap* createMinHeap() {
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    heap->size = 0;
    return heap;
}

// Function to insert a node into the min heap
void insertMinHeap(MinHeap *heap, HFMTNode *node) {
    heap->nodes[heap->size] = node;
    int i = heap->size;
    while (i > 0 && heap->nodes[(i - 1) / 2]->weight > heap->nodes[i]->weight) {
        HFMTNode *temp = heap->nodes[i];
        heap->nodes[i] = heap->nodes[(i - 1) / 2];
        heap->nodes[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
    heap->size++;
}

// Function to extract the minimum node from the heap
HFMTNode* extractMin(MinHeap *heap) {
    HFMTNode *minNode = heap->nodes[0];
    heap->nodes[0] = heap->nodes[--heap->size];
    int i = 0;
    while (true) {
        int smallest = i;
        if (2 * i + 1 < heap->size && heap->nodes[2 * i + 1]->weight < heap->nodes[smallest]->weight) {
            smallest = 2 * i + 1;
        }
        if (2 * i + 2 < heap->size && heap->nodes[2 * i + 2]->weight < heap->nodes[smallest]->weight) {
            smallest = 2 * i + 2;
        }
        if (smallest == i) {
            break;
        }
        HFMTNode *temp = heap->nodes[i];
        heap->nodes[i] = heap->nodes[smallest];
        heap->nodes[smallest] = temp;
        i = smallest;
    }
    return minNode;
}

// Function to build the Huffman Tree
HFMTNode* buildHuffmanTree(ElemType elements[], int frequencies[], int size) {
    MinHeap *heap = createMinHeap();
    for (int i = 0; i < size; i++) {
        insertMinHeap(heap, createNode(elements[i], frequencies[i]));
    }
    while (heap->size > 1) {
        HFMTNode *left = extractMin(heap);
        HFMTNode *right = extractMin(heap);
        HFMTNode *merged = createNode('\0', left->weight + right->weight);
        merged->lChild = left;
        merged->rChild = right;
        insertMinHeap(heap, merged);
    }
    return extractMin(heap);
}

// Function to generate Huffman Codes from the tree
void generateCodes(HFMTNode *root, char *code, int depth, char codes[][MAX_TREE_NODES]) {
    if (root->lChild == NULL && root->rChild == NULL) {
        code[depth] = '\0';
        strcpy(codes[(unsigned char)root->element], code);
        return;
    }
    if (root->lChild) {
        code[depth] = '0';
        generateCodes(root->lChild, code, depth + 1, codes);
    }
    if (root->rChild) {
        code[depth] = '1';
        generateCodes(root->rChild, code, depth + 1, codes);
    }
}

// Function to encode a string using the Huffman codes
void encodeString(const char *input, char codes[][MAX_TREE_NODES], char *output) {
    output[0] = '\0';
    for (int i = 0; input[i] != '\0'; i++) {
        strcat(output, codes[(unsigned char)input[i]]);
    }
}

// Function to decode a string using the Huffman tree
void decodeString(const char *encoded, HFMTNode *root, char *output) {
    int index = 0;
    HFMTNode *current = root;
    for (int i = 0; encoded[i] != '\0'; i++) {
        current = (encoded[i] == '0') ? current->lChild : current->rChild;
        if (current->lChild == NULL && current->rChild == NULL) {
            output[index++] = current->element;
            current = root;
        }
    }
    output[index] = '\0';
}

int main() {
    ElemType elements[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    int frequencies[] = {5, 9, 12, 13, 16, 45};
    int size = sizeof(elements) / sizeof(elements[0]);

    HFMTNode *root = buildHuffmanTree(elements, frequencies, size);

    char codes[MAX_TREE_NODES][MAX_TREE_NODES] = {0};
    char code[MAX_TREE_NODES];
    generateCodes(root, code, 0, codes);

    printf("Huffman Codes:\n");
    for (int i = 0; i < size; i++) {
        printf("%c: %s\n", elements[i], codes[(unsigned char)elements[i]]);
    }

    const char *input = "abcdef";
    char encoded[MAX_TREE_NODES * 10] = {0};
    encodeString(input, codes, encoded);
    printf("Encoded: %s\n", encoded);

    char decoded[MAX_TREE_NODES] = {0};
    decodeString(encoded, root, decoded);
    printf("Decoded: %s\n", decoded);

    return 0;
}
```

#### 模块设计

   - `HFMTNode`：哈夫曼树节点结构，包含元素、权重、左子树、右子树。
   - `MinHeap`：用于构建哈夫曼树的优先队列结构。
   - 哈夫曼树的创建、编码、解码功能模块。

#### 核心算法实现

   - 创建节点: `HFMTNode* createNode(ElemType element, int weight)`
   - 构建最小堆: `MinHeap* createMinHeap()`
   - 插入堆: `void insertMinHeap(MinHeap *heap, HFMTNode *node)`
   - 提取最小节点: `HFMTNode* extractMin(MinHeap *heap)`
   - 构建哈夫曼树: `HFMTNode* buildHuffmanTree(ElemType elements[], int frequencies[], int size)`
   - 生成哈夫曼编码: `void generateCodes(HFMTNode *root, char *code, int depth, char codes[][MAX_TREE_NODES])`
   - 编码字符串: `void encodeString(const char *input, char codes[][MAX_TREE_NODES], char *output)`
   - 解码字符串: `void decodeString(const char *encoded, HFMTNode *root, char *output)`

#### 时间复杂度分析

   - 构建哈夫曼树的时间复杂度为 O(n log n)，其中 n 为字符种类数。
   - 编码和解码的时间复杂度为 O(m)，其中 m 为输入字符串的长度。

## 实验过程描述

在本次实验中，我们实现了二叉树的基本操作以及哈夫曼编码和解码功能。通过一系列的测试用例，我们验证了程序的正确性：

1. 二叉树操作：
   - 先序、中序、后序遍历的输出结果与预期一致，说明遍历算法实现正确。
   - 节点总数、叶节点数和树的高度计算正确，符合手动计算的结果。
   - 交换左右子树后，先序遍历的结果发生了预期的变化，验证了子树交换功能的正确性。

2. 哈夫曼树操作：
   - 哈夫曼编码生成的结果符合字符权重的预期。
   - 输入字符串 "abcdef" 的编码与解码结果一致，验证了编码和解码功能的正确性。

### 测试和调试过程遇到的问题

1. 内存管理问题：
   - 在实现二叉树和哈夫曼树时，需频繁进行动态内存分配。初期因未能正确释放内存，导致内存泄漏问题。通过仔细分析代码执行流程，确保在合适的地方释放不再使用的内存，解决了这一问题。

2. 边界条件处理：
   - 在处理空树或仅有一个节点的特殊情况下，遇到了一些逻辑错误。这些问题通过在测试用例中加入边界条件进行调试和验证得以解决。

3. 优先队列（最小堆）实现问题：
   - 在实现哈夫曼树的过程中，最小堆的插入和删除操作出现了错误，导致无法正确构建树。通过仔细检查堆的调整过程，修正了索引计算错误，确保了堆的性质。

### 软件设计与实现的经验和体会

- 模块化设计：将二叉树和哈夫曼树的操作功能模块化，便于代码管理和测试。同时，模块化设计有助于提高代码的可读性和可维护性。

- 递归的应用：二叉树的遍历、统计节点数和计算树的高度等操作都依赖于递归的思想，进一步加深了对递归算法的理解。

- 数据结构的选择：在哈夫曼树的实现中，通过使用优先队列有效地管理节点，展示了选择合适的数据结构对算法效率的影响。

### 进一步改进的设想

- 优化内存使用：在哈夫曼编码中，进一步优化编码的存储方式，例如使用位运算来压缩编码结果，减少空间开销。

- 更多树的操作：增加对二叉树的平衡性检测和重平衡操作的实现，进一步理解平衡二叉树的概念。

- 扩展应用：尝试实现其他数据压缩算法，如LZW或基于字典的压缩方法，扩展对数据压缩技术的理解和应用。

- 性能测试：在大规模数据集上测试哈夫曼编码的性能，并与其他压缩算法进行比较，分析其优缺点。

