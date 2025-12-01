#include "node.h"

Node *Node_init(int value, Node *parent) {
    Node *node = calloc(1, sizeof(Node));
    if (!node) {
        log_error("Allocation fail for Node instance");
        return NULL;
    }

    node->value = value;
    node->parent = parent;

    return node;
}

void Node_destroy(Node *node) {
    if (!node) return;
    Node_destroy(node->left);
    Node_destroy(node->right);
    free(node);
}
