#include <stdio.h>
#include <stdlib.h>

#define m 5

typedef struct B_tree_node {
    struct B_tree_node *children[2 * m];
    int keys[2 * m - 1];
    int number_keys;
} B_tree_node;

typedef B_tree_node *B_tree;

B_tree create_B_tree() {
    B_tree x = malloc(sizeof(B_tree_node));
    if (x == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 2 * m; i++) {
        x->children[i] = NULL;
    }
    x->number_keys = 0;
    return x;
}

B_tree find(int val, B_tree b) {
    if (b == NULL) return NULL;
    for (int i = 0; i < b->number_keys; i++) {
        if (b->keys[i] == val) return b;
        if (val < b->keys[i]) return find(val, b->children[i]);
    }
    return find(val, b->children[b->number_keys]);
}

void splitChild(B_tree x, int i) {
    B_tree new_node = (B_tree) malloc(sizeof(B_tree_node));
    if (new_node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    B_tree child = x->children[i];
    new_node->number_keys = m - 1;
    child->number_keys = m;
    for (int j = 0; j < m - 1; j++) {
        new_node->keys[j] = child->keys[j + m];
    }
    if (child->children[0] != NULL) {
        for (int j = 0; j < m; j++) {
            new_node->children[j] = child->children[j + m];
        }
    }
    int median = child->keys[m - 1];
    for (int j = x->number_keys; j > i; j--) {
        x->keys[j] = x->keys[j - 1];
    }
    x->keys[i] = median;
    for (int j = x->number_keys + 1; j > i; j--) {
        x->children[j] = x->children[j - 1];
    }
    x->children[i + 1] = new_node;
    x->number_keys++;
}