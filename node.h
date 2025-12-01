#ifndef NODE_H
#define NODE_H

#include "log.h"

#include <stddef.h>
#include <stdlib.h>

typedef enum { RED, BLACK } NodeColor;

typedef struct Node Node;

struct Node {
    int value;
    NodeColor color;
    Node *left;
    Node *right;
    Node *parent;
};

Node *Node_init(int value, Node *parent);
void Node_destroy(Node *node);

#endif
