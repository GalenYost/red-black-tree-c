#include "tree.h"
#include "log.h"

RedBlackTree *RedBlackTree_init() {
    RedBlackTree *tree = calloc(1, sizeof(RedBlackTree));
    if (!tree) {
        log_error("Allocation fail for RedBlackTree instance");
        return NULL;
    }

#ifndef NO_TRACE
    log_trace("Tree initialized");
#endif

    return tree;
}

void RedBlackTree_destroy(RedBlackTree *tree) {
    if (!tree) {
        log_warn("No tree provided, dropping the function call");
        return;
    }

    Node_destroy(tree->head);
    free(tree);

#ifndef NO_TRACE
    log_trace("Tree destroyed");
#endif
}

static void left_rotate(RedBlackTree *tree, Node *node) {
    Node *y = node->right;
    node->right = y->left;

    if (y->left != NULL) { y->left->parent = node; }

    y->parent = node->parent;

    if (node->parent == NULL) {
        tree->head = y;
    } else if (node == node->parent->left) {
        node->parent->left = y;
    } else {
        node->parent->right = y;
    }

    y->left = node;
    node->parent = y;
}

static void right_rotate(RedBlackTree *tree, Node *node) {
    Node *x = node->left;
    node->left = x->right;

    if (x->right != NULL) { x->right->parent = node; }

    x->parent = node->parent;

    if (node->parent == NULL) {
        tree->head = x;
    } else if (node == node->parent->right) {
        node->parent->right = x;
    } else {
        node->parent->left = x;
    }

    x->right = node;
    node->parent = x;
}

static void RedBlackTree_insertFixup(RedBlackTree *tree, Node *z) {
    while (z->parent && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node *uncle = z->parent->parent->right;

            if (uncle && uncle->color == RED) {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(tree, z->parent->parent);
            }
        } else {
            Node *uncle = z->parent->parent->left;

            if (uncle && uncle->color == RED) {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(tree, z->parent->parent);
            }
        }
    }

    tree->head->color = BLACK;
}

Node *RedBlackTree_insert(RedBlackTree *tree, int value) {
    if (!tree) {
        log_error("No tree provided");
        return NULL;
    }

    if (!tree->head) {
        Node *head = Node_init(value, NULL);
        head->color = BLACK;
        tree->head = head;
        return tree->head;
    }

    Node *y = NULL;
    Node *x = tree->head;
    Node *z = Node_init(value, y);

    while (x != NULL) {
        y = x;
        if (z->value < x->value)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;

    if (y == NULL)
        tree->head = z;
    else if (z->value < y->value)
        y->left = z;
    else
        y->right = z;

    RedBlackTree_insertFixup(tree, z);
    return z;
}

static void transplant(RedBlackTree *tree, Node *u, Node *v) {
    if (u->parent == NULL) {
        tree->head = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }

    if (v != NULL) { v->parent = u->parent; }
}

static void RedBlackTree_deleteFixup(RedBlackTree *tree, Node *x,
                                     Node *x_parent) {
    Node *w;

    while (x != tree->head && (x == NULL || x->color == BLACK)) {
        if (x == x_parent->left) {
            w = x_parent->right;

            if (w->color == RED) {
                w->color = BLACK;
                x_parent->color = RED;
                left_rotate(tree, x_parent);
                w = x_parent->right;
            }

            if ((w->left == NULL || w->left->color == BLACK) &&
                (w->right == NULL || w->right->color == BLACK)) {
                w->color = RED;
                x = x_parent;
                x_parent = x->parent;
            } else {
                if (w->right == NULL || w->right->color == BLACK) {
                    if (w->left) w->left->color = BLACK;
                    w->color = RED;
                    right_rotate(tree, w);
                    w = x_parent->right;
                }

                w->color = x_parent->color;
                x_parent->color = BLACK;
                if (w->right) w->right->color = BLACK;
                left_rotate(tree, x_parent);

                x = tree->head;
            }
        } else {
            w = x_parent->left;

            if (w->color == RED) {
                w->color = BLACK;
                x_parent->color = RED;
                right_rotate(tree, x_parent);
                w = x_parent->left;
            }

            if ((w->right == NULL || w->right->color == BLACK) &&
                (w->left == NULL || w->left->color == BLACK)) {
                w->color = RED;
                x = x_parent;
                x_parent = x->parent;
            } else {
                if (w->left == NULL || w->left->color == BLACK) {
                    if (w->right) w->right->color = BLACK;
                    w->color = RED;
                    left_rotate(tree, w);
                    w = x_parent->left;
                }

                w->color = x_parent->color;
                x_parent->color = BLACK;
                if (w->left) w->left->color = BLACK;
                right_rotate(tree, x_parent);
                x = tree->head;
            }
        }
    }

    if (x) x->color = BLACK;
}

static Node *minimum(Node *node) {
    while (node->left != NULL) node = node->left;
    return node;
}

void RedBlackTree_remove(RedBlackTree *tree, int value) {
    if (!tree) {
        log_error("No tree provided");
        return;
    }

    Node *z = tree->head;
    while (z != NULL) {
        if (value == z->value) break;
        if (value < z->value)
            z = z->left;
        else
            z = z->right;
    }
    if (z == NULL) return;

    Node *y = z;
    Node *x;
    NodeColor y_original_color = y->color;
    Node *x_parent = NULL;

    if (z->left == NULL) {
        x = z->right;
        x_parent = z->parent;
        transplant(tree, z, z->right);
    } else if (z->right == NULL) {
        x = z->left;
        x_parent = z->parent;
        transplant(tree, z, z->left);
    } else {
        y = minimum(z->right);
        y_original_color = y->color;
        x = y->right;

        if (y->parent == z) {
            x_parent = y;
        } else {
            x_parent = y->parent;
            transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (y_original_color == BLACK) {
        RedBlackTree_deleteFixup(tree, x, x_parent);
    }

    free(z);
}

static void print_tree_recursive(Node *node, int indent) {
    if (node == NULL) { return; }

    int spacing = 6;

    print_tree_recursive(node->right, indent + spacing);

    printf("\n");
    for (int i = 0; i < indent; i++) { printf(" "); }

    if (node->color == RED) {
        printf(ANSI_COLOR_RED "%d" ANSI_COLOR_RESET, node->value);
    } else {
        printf(ANSI_BOLD "%d" ANSI_COLOR_RESET, node->value);
    }

    print_tree_recursive(node->left, indent + spacing);
}

void RedBlackTree_display(RedBlackTree *tree) {
    if (!tree) {
        log_error("No tree provided");
        return;
    }
    printf("----------------------------------------\n");
    print_tree_recursive(tree->head, 0);
    printf("\n");
    printf("----------------------------------------\n");
}
