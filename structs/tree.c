#include "tree.h"

Tree create_tree() {
    Tree tree;
    tree.head = NULL;
    tree.curr_ptr = NULL;

    return tree;
}

Node *create_node(Token value) {
    Node *node = malloc(sizeof(Node));
    node->right = NULL;
    node->left = NULL;
    node->value = value;

    return node;
}

void tree_insert(Tree *tree, Token value) {
    Node *node = create_node(value);
    if(tree->head == NULL) {  
        tree->head = node;
        tree->curr_ptr = node;
        return;
    }

    tree->curr_ptr->left = node;
    tree->curr_ptr = tree->curr_ptr->left;
}

void print_tree(Tree *tree) {
    print_node(tree->head);
}

void print_node(Node *node) {
    printf("%s: [", node->value.tk_text.data);
    if(node->left != NULL) {
        printf("left -> {");
        print_node(node->left);
        printf("}, ");
    }
    if(node->right != NULL) {
        printf("right -> {");
        print_node(node->right);
        printf("}, ");
    }
    printf("]; ");
}
