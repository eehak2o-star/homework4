#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
    int height;
} Node;

int array[100];
int size = 0;

Node* new_node(int data)
{
    Node* p;

    p = (Node*)malloc(sizeof(Node));

    p->data = data;
    p->left = NULL;
    p->right = NULL;
    p->height = 1;

    return p;
}

int get_height(Node* p)
{
    if (p == NULL)
        return 0;

    return p->height;
}

int bigger(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}

int array_insert(int data, int* count)
{
    int i;

    for (i = 0; i < size; i++) {
        *count = *count + 1;

        if (array[i] == data)
            return 0;
    }

    array[size] = data;
    size = size + 1;

    return 1;
}

Node* bst_insert(Node* root, int data, int* count)
{
    if (root == NULL)
        return new_node(data);

    *count = *count + 1;

    if (data < root->data)
        root->left = bst_insert(root->left, data, count);
    else if (data > root->data)
        root->right = bst_insert(root->right, data, count);

    return root;
}

Node* right_rotate(Node* root)
{
    Node* p;
    Node* q;

    p = root->left;
    q = p->right;

    p->right = root;
    root->left = q;

    root->height = bigger(get_height(root->left),
        get_height(root->right)) + 1;

    p->height = bigger(get_height(p->left),
        get_height(p->right)) + 1;

    return p;
}

Node* left_rotate(Node* root)
{
    Node* p;
    Node* q;

    p = root->right;
    q = p->left;

    p->left = root;
    root->right = q;

    root->height = bigger(get_height(root->left),
        get_height(root->right)) + 1;

    p->height = bigger(get_height(p->left),
        get_height(p->right)) + 1;

    return p;
}

Node* avl_insert(Node* root, int data, int* count)
{
    int balance;

    if (root == NULL)
        return new_node(data);

    *count = *count + 1;

    if (data < root->data)
        root->left = avl_insert(root->left, data, count);
    else if (data > root->data)
        root->right = avl_insert(root->right, data, count);
    else
        return root;

    root->height = bigger(get_height(root->left),
        get_height(root->right)) + 1;

    balance = get_height(root->left) - get_height(root->right);

    if (balance > 1) {
        if (data < root->left->data)
            return right_rotate(root);

        if (data > root->left->data) {
            root->left = left_rotate(root->left);
            return right_rotate(root);
        }
    }

    if (balance < -1) {
        if (data > root->right->data)
            return left_rotate(root);

        if (data < root->right->data) {
            root->right = right_rotate(root->right);
            return left_rotate(root);
        }
    }

    return root;
}

int array_search(int data, int* count)
{
    int i;

    for (i = 0; i < size; i++) {
        *count = *count + 1;

        if (array[i] == data)
            return 1;
    }

    return 0;
}

int tree_search(Node* root, int data, int* count)
{
    while (root != NULL) {
        *count = *count + 1;

        if (data == root->data)
            return 1;

        if (data < root->data)
            root = root->left;
        else
            root = root->right;
    }

    return 0;
}

void free_tree(Node* root)
{
    if (root == NULL)
        return;

    free_tree(root->left);
    free_tree(root->right);

    free(root);
}

int main(void)
{
    Node* bst;
    Node* avl;

    int data[100];

    int array_count;
    int bst_count;
    int avl_count;
    int duplicate;

    int search_total_array;
    int search_total_bst;
    int search_total_avl;

    int i;
    int key;

    int array_search_count;
    int bst_search_count;
    int avl_search_count;

    int array_result;
    int bst_result;
    int avl_result;

    bst = NULL;
    avl = NULL;

    array_count = 0;
    bst_count = 0;
    avl_count = 0;
    duplicate = 0;

    search_total_array = 0;
    search_total_bst = 0;
    search_total_avl = 0;

    srand((unsigned int)time(NULL));

    printf("Generated values\n");

    for (i = 0; i < 100; i++) {
        data[i] = rand() % 1001;

        printf("%d ", data[i]);

        if (array_insert(data[i], &array_count) == 0)
            duplicate = duplicate + 1;

        bst = bst_insert(bst, data[i], &bst_count);
        avl = avl_insert(avl, data[i], &avl_count);
    }

    printf("\n\nStored values : %d\n", size);
    printf("Duplicate values : %d\n", duplicate);

    printf("\nConstruction\n");
    printf("Array comparisons : %d\n", array_count);
    printf("BST comparisons   : %d\n", bst_count);
    printf("AVL comparisons   : %d\n", avl_count);

    printf("\nStructure\n");
    printf("Array length : %d\n", size);
    printf("BST height   : %d\n", get_height(bst));
    printf("AVL height   : %d\n", get_height(avl));

    printf("\nSearches : 50\n");

    for (i = 0; i < 50; i++) {
        key = rand() % 1001;

        array_search_count = 0;
        bst_search_count = 0;
        avl_search_count = 0;

        array_result = array_search(key, &array_search_count);
        bst_result = tree_search(bst, key, &bst_search_count);
        avl_result = tree_search(avl, key, &avl_search_count);

        search_total_array =
            search_total_array + array_search_count;

        search_total_bst =
            search_total_bst + bst_search_count;

        search_total_avl =
            search_total_avl + avl_search_count;

        printf("\nSearch Key : %d\n", key);

        printf("\nSequential Search\n");

        if (array_result == 1)
            printf("Result      : Found\n");
        else
            printf("Result      : Not Found\n");

        printf("Comparisons : %d\n", array_search_count);

        printf("\nBST Search\n");

        if (bst_result == 1)
            printf("Result      : Found\n");
        else
            printf("Result      : Not Found\n");

        printf("Comparisons : %d\n", bst_search_count);

        printf("\nAVL Search\n");

        if (avl_result == 1)
            printf("Result      : Found\n");
        else
            printf("Result      : Not Found\n");

        printf("Comparisons : %d\n", avl_search_count);
    }

    printf("\nSearch Summary\n");

    printf("\nSequential Search\n");
    printf("Total comparisons   : %d\n", search_total_array);
    printf("Average comparisons : %.2f\n",
        (double)search_total_array / 50);

    printf("\nBST Search\n");
    printf("Total comparisons   : %d\n", search_total_bst);
    printf("Average comparisons : %.2f\n",
        (double)search_total_bst / 50);

    printf("\nAVL Search\n");
    printf("Total comparisons   : %d\n", search_total_avl);
    printf("Average comparisons : %.2f\n",
        (double)search_total_avl / 50);

    free_tree(bst);
    free_tree(avl);

    return 0;
}
