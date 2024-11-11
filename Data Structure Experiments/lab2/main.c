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
