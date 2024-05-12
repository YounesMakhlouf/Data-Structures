#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b) (((a)>(b))?(a):(b))
#define MIN(a, b) (((a)<(b))?(a):(b))

typedef struct noeud {
    int val;
    struct noeud *left;
    struct noeud *right;
    int balance;
} node;

typedef node *AVL;

//AVL AVLInit() {
//    return NULL;
//}

AVL AVLInit(int v) {
    AVL x = malloc(sizeof(node));
    x->val = v;
    x->balance = 0;
    x->right = NULL;
    x->left = NULL;
    return x;
}

AVL find(int x, AVL A) {
    if (A == NULL) return NULL;
    if (A->val == x) return A;
    if (A->val > x)
        return find(x, A->left);
    else
        return find(x, A->right);
}

AVL RightRotation(AVL x) {
    AVL y = x->left;
    x->balance = x->balance - 1 - MAX(0, y->balance);
    y->balance = y->balance - 1 + MIN(0, x->balance);
    x->left = y->right;
    y->right = x;
    return y;
}

AVL LeftRotation(AVL x) {
    AVL y = x->right;
    x->balance = x->balance + 1 - MIN(0, y->balance);
    y->balance = y->balance + 1 + MAX(0, x->balance);
    x->right = y->left;
    y->left = x;
    return y;
}

AVL Reequilibrer(AVL a) {
    if (a->balance > 0) {
        if ((a->left)->balance > 0)
            return LeftRotation(a);
        else {
            a->left = LeftRotation(a->left);
            return RightRotation(a);
        }
    } else {
        if ((a->right)->balance < 0)
            return RightRotation(a);
        else {
            a->right = RightRotation(a->right);
            return LeftRotation(a);
        }
    }
}

AVL insertion(AVL a, int val, int *modif) {
    if (a == NULL) {
        AVL x = AVLInit(val);
        *modif = 1;
        return x;
    }
    if (val > a->val) {
        a->right = insertion(a->right, val, modif);
        if (modif)
            a->balance -= 1;
    } else {
        a->left = insertion(a->left, val, modif);
        if (modif)
            a->balance += 1;
    }
    if (abs(a->balance) > 1)
        a = Reequilibrer(a);
    if (a->balance == 0)
        *modif = 0;
    return a;
}

AVL deleteChild(AVL a, int *heritier, int *modif) {
    if (a->right != NULL) {
        a->right = deleteChild(a->right, heritier, modif);
        if (*modif)
            a->balance++;
    } else {
        *heritier = a->val;
        *modif = 1;
        return a->left;
    }
    if (abs(a->balance) > 1) a = Reequilibrer(a);
    if (a->balance == 0) modif = 0;
}

AVL delete(AVL a, int val, int *modif) {
    if (a == NULL) return NULL;
    if (a->val > val) {
        a->left = delete(a->left, val, modif);
        if (*modif)
            a->balance--;
    }
    if (a->val < val) {
        a->right = delete(a->right, val, modif);
        if (*modif)
            a->balance++;
    } else {
        if (a->left == NULL) {
            AVL r = a->right;
            free(a);
            *modif = 1;
            return r;
        } else {
            int h;
            a->left = deleteChild(a->left, &h, modif);
            a->val = h;
        }
    }
    if (abs(a->balance) > 1) a = Reequilibrer(a);
    if (a->balance == 0) modif = 0;
}