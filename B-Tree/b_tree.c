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

void print_B_tree(B_tree x, int level) {
    if (x != NULL) {
        int i;
        for (i = 0; i < x->number_keys; i++) {
            if (x->children[i] != NULL) {
                print_B_tree(x->children[i], level + 1);
            }
            for (int j = 0; j < level; j++) printf("  ");
            printf("%d\n", x->keys[i]);
        }
        if (x->children[i] != NULL) {
            print_B_tree(x->children[i], level + 1);
        }
    }
}

B_tree find(int val, B_tree b) {
    if (b == NULL) return NULL;
    for (int i = 0; i < b->number_keys; i++) {
        if (b->keys[i] == val) return b; // Value found
        if (val < b->keys[i]) return find(val, b->children[i]); // Go to left child
    }
    return find(val, b->children[b->number_keys]); // Go to right child
}

void splitChild(B_tree x, int i) {
    B_tree child = x->children[i];
    B_tree new_node = (B_tree) malloc(sizeof(B_tree_node));
    if (new_node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    for (int j = 0; j < m - 1; j++) {
        new_node->keys[j] = child->keys[j + m];
    }
    if (child->children[0] != NULL) {
        for (int j = 0; j < m; j++) {
            new_node->children[j] = child->children[j + m];
        }
    }
    new_node->number_keys = m - 1;
    child->number_keys = m - 1;
    for (int j = x->number_keys; j > i; j--) {
        x->keys[j] = x->keys[j - 1];
    }
    x->keys[i] = child->keys[m - 1];
    for (int j = x->number_keys + 1; j > i; j--) {
        x->children[j] = x->children[j - 1];
    }
    x->children[i + 1] = new_node;
    x->number_keys++;
}

void insertIncompleteNode(B_tree x, int k) {
    int i = x->number_keys - 1;
    if (x->children[0] == NULL) { // Leaf node
        while (k < x->keys[i] && i >= 0) {
            x->keys[i + 1] = x->keys[i];
            i--;
        }
        x->keys[i + 1] = k;
        x->number_keys++;
    } else { // Internal node
        while (k < x->keys[i] && i >= 0) {
            i--;
        }
        if (x->children[i + 1]->number_keys == 2 * m + 1) {
            splitChild(x, i + 1);
            if (k > x->keys[i + 1]) {
                i++;
            }
        }
        insertIncompleteNode(x->children[i + 1], k);
    }
}

void insert(B_tree x, int k) {
    if (x->number_keys == 2 * m + 1) {
        B_tree newRoot = create_B_tree();
        newRoot->children[0] = x;
        splitChild(newRoot, 0);
        insertIncompleteNode(newRoot, k);
    } else
        insertIncompleteNode(x, k);
}

void borrowLeft(B_tree x, int indice) {
    B_tree y = x->children[indice];
    B_tree brother = x->children[indice - 1];
    for (int i = y->number_keys; i > 0; --i) {
        y->keys[i] = y->keys[i - 1];
    }
    for (int i = y->number_keys + 1; i > 0; --i) {
        y->children[i] = y->children[i - 1];
    }
    y->keys[0] = x->keys[indice - 1];
    y->children[0] = brother->children[brother->number_keys];
    y->number_keys++;
    x->keys[indice - 1] = brother->keys[brother->number_keys - 1];
    brother->number_keys--;
}

void fusionLeft(B_tree x, int indice) {
    B_tree frere = x->children[indice - 1];
    B_tree y = x->children[indice];
    frere->keys[frere->number_keys] = x->keys[indice - 1];
    for (int i = 0; i < y->number_keys; i++) {
        frere->keys[i + frere->number_keys + 1] = y->keys[i];
        frere->children[i + frere->number_keys + 1] = y->children[i];
    }
    frere->children[2 * m - 1] = y->children[y->number_keys];
    frere->number_keys = 2 * m - 1;
    free(y);
    for (int i = indice - 1; i < x->number_keys - 1; i++) {
        x->keys[i] = x->keys[i + 1];
    }
    for (int i = indice; i < x->number_keys; i++) {
        x->children[i] = x->children[i + 1];
    }
    x->number_keys--;
}


int sup_pred(B_tree y) {
    if (y->children[0] == NULL) {
        y->number_keys--;
        return y->keys[y->number_keys];
    }
    if (y->children[y->number_keys]->number_keys >= m) {
        return sup_pred(y->children[y->number_keys]);
    }
    if (y->children[y->number_keys - 1]->number_keys >= m) {
        borrowLeft(y, y->number_keys);
    } else {
        fusionLeft(y, y->number_keys);
    }
    return sup_pred(y->children[y->number_keys]);
}


B_tree supprimerInterne(B_tree x, int k) {
    if (x->children[0] == nullptr) {  // x is a leaf
        int i;
        for (i = 0; i < x->number_keys; i++) {
            if (x->keys[i] == k) {
                break;
            }
        }
        for (int j = i; j < x->number_keys - 1; j++) {
            x->keys[j] = x->keys[j + 1];
        }
        x->number_keys--;
        if (x->number_keys == 0) {
            return nullptr;
        }
        return x;
    }
    //x isn't a leaf
    int i;
    bool appartient = false;
    for (i = 0; i < x->number_keys; i++) {
        if (x->keys[i] == k) {
            appartient = true;
            break;
        }
        if (x->keys[i] > k) {
            break;
        }
    }
    if (appartient) {
        if (x->children[i]->number_keys >= m) {
            x->keys[i] = sup_pred(x->children[i]);
            return x;
        }
        if (x->children[i + 1]->number_keys >= m) {
            x->keys[i] = sup_succ(x->keys[i + 1]); // Like supp_pred (not implemented)
            return x;
        }
        fusionLeft(x, i + 1);
        return supprimerInterne(x->children[i], k);
    }
    if (x->children[i]->number_keys >= m) {
        return supprimerInterne(x->children[i], k);
    }
    if (i > 0) {        //child i has a left brother
        if (x->children[i - 1]->number_keys >= m) {
            borrowLeft(x, i);
            return supprimerInterne(x->children[i], k);
        }
    }
    if (i < x->number_keys) {
        if (x->children[i + 1]->number_keys >= m) {
            borrowRight(x, i); //Like borrowLeft
            return supprimerInterne(x->children[i], k);
        }
    }
    if (i > 0) {
        fusionLeft(x, i);
        return supprimerInterne(x->children[i - 1], k);
    }
    if (i < m) {
        fusionRight(x, i); //Like fusionLeft
        return supprimerInterne(x->children[i], k);
    }
}

B_tree delete(B_tree a, int k) {
    if (a->number_keys == 1) {
        if (a->children[0] != nullptr) {
            if (a->children[0]->number_keys == m - 1 && a->children[1]->number_keys == m - 1) {
                fusionLeft(a, 1);
                B_tree x = a->children[0];
                free(a);
                return supprimerInterne(x, k);
            }
        }
    }
    return supprimerInterne(a, k);
}

int main() {
    // Create the B-tree
    B_tree tree = create_B_tree();

    // Insert values into the B-tree
    int values_to_insert[] = {10, 20, 5, 6, 12, 30, 7, 17};
    int num_values = sizeof(values_to_insert) / sizeof(values_to_insert[0]);

    for (int i = 0; i < num_values; i++) {
        insert(tree, values_to_insert[i]);
    }

    // Print the B-tree structure
    printf("B-tree structure after insertions:\n");
    print_B_tree(tree, 0);

    // Search for a value in the B-tree
    int search_value = 12;
    B_tree result = find(search_value, tree);
    if (result != NULL) {
        printf("\nValue %d found in the B-tree.\n", search_value);
    } else {
        printf("\nValue %d not found in the B-tree.\n", search_value);
    }
    return 0;
}