#include <stdio.h>
#include <stdlib.h>

struct StNode
{
	int data;
	int freq;
	struct StNode *parent;
	struct StNode *left;
	struct StNode *right;
};
typedef struct StNode StNode;
typedef StNode *StNodePtr;

StNodePtr Insert(StNodePtr, int, StNodePtr *, int *);
void preorder(StNodePtr, FILE *);
void Splay(StNodePtr *, StNodePtr *, int *);
void leftRotation(StNodePtr *, StNodePtr *);
void rightRotation(StNodePtr *, StNodePtr *);

int main(int argc, char *argv[])
{
	int inNumber;
	int TimeForModSplay = 0;
	int TimeForSplay = 0;

	StNodePtr root = NULL;
	StNodePtr modRoot = NULL;

	StNodePtr lastAdded = NULL;
	StNodePtr modLastAdded = NULL;

	FILE *file = fopen("input.txt", "r");
	FILE *output = fopen("output.txt", "w");
	int key;
	while (fscanf(file, "%d,", &inNumber) == 1)
	{

		root = Insert(root, inNumber, &lastAdded, &TimeForSplay);
		modRoot = Insert(modRoot, inNumber, &modLastAdded, &TimeForModSplay);

		Splay(&root, &lastAdded, &TimeForSplay);
		if (modRoot->freq < modLastAdded->freq)
			Splay(&modRoot, &modLastAdded, &TimeForModSplay);

		key = 0;
	}
	fprintf(output, "\n Pre-order demonstration of splay tree: ");
	preorder(root, output);
	fprintf(output, "\n Comparison time of splay tree: %d \n", TimeForSplay);
	fprintf(output, "\n Pre-order demonstration of mod-splay tree: ");
	preorder(modRoot, output);
	fprintf(output, "\n Comparison time of mod-splay tree: %d", TimeForModSplay);
}
void leftRotation(StNodePtr *lastAdded, StNodePtr *root)
{
	// to perform left rotation lastAdded must be right child of its parent.
	(*lastAdded)->parent->right = (*lastAdded)->left;	 // the left child of lastAdded is in the right of the parent of the lastAdded. After rotation its right child must be on the right of the former parent of lastAdded.
	(*lastAdded)->left = (*lastAdded)->parent;			 // lastAdded will become the root of the subtree so its left child must be its former parent.
	(*lastAdded)->parent = (*lastAdded)->parent->parent; // parent of the lastAdded must be its former grandparent at the end of the rotation
	if ((*lastAdded)->left->right != NULL)
		(*lastAdded)->left->right->parent = (*lastAdded)->left;

	(*lastAdded)->left->parent = (*lastAdded); // we denote parent of the former parent of lastAdded as lastAdded since the former parent is left child of lastAdded now.

	if ((*lastAdded)->parent == NULL) // if lastAdded don't have a parent no longer. It means that it is root now
		*root = *lastAdded;
	else // In this statement we confirm the subtree in where we perform the rotation is on the left of the grandparent or on the right
	{
		if (((*lastAdded)->parent->left != NULL) && (*lastAdded)->parent->left->data == (*lastAdded)->left->data)
			(*lastAdded)->parent->left = (*lastAdded);
		else if (((*lastAdded)->parent->right != NULL) && (*lastAdded)->parent->right->data == (*lastAdded)->left->data)
			(*lastAdded)->parent->right = (*lastAdded);
	}
}
void rightRotation(StNodePtr *lastAdded, StNodePtr *root)
{
	// clockwise rotation.
	(*lastAdded)->parent->left = (*lastAdded)->right;
	(*lastAdded)->right = (*lastAdded)->parent;
	(*lastAdded)->parent = (*lastAdded)->parent->parent;

	if ((*lastAdded)->right->left != NULL)
		(*lastAdded)->right->left->parent = (*lastAdded)->right;

	(*lastAdded)->right->parent = *lastAdded;

	if ((*lastAdded)->parent == NULL)
		*root = *lastAdded;
	else
	{
		if ((*lastAdded)->parent->left != NULL && (*lastAdded)->parent->left->data == (*lastAdded)->right->data)
			(*lastAdded)->parent->left = (*lastAdded);
		else if ((*lastAdded)->parent->right != NULL && (*lastAdded)->parent->right->data == (*lastAdded)->right->data)
			(*lastAdded)->parent->right = (*lastAdded);
	}
}
void Splay(StNodePtr *root, StNodePtr *lastAdded, int *cTime)
{
	if ((*lastAdded)->parent != NULL)
	{
		if ((*lastAdded)->parent->parent == NULL) // For zig rotation
		{
			if ((*lastAdded)->parent->left != NULL && (*lastAdded)->parent->left->data == (*lastAdded)->data)
			{
				rightRotation(lastAdded, root);
				(*cTime)++;
			}
			else if ((*lastAdded)->parent->right != NULL && (*lastAdded)->parent->right->data == (*lastAdded)->data)
			{
				leftRotation(lastAdded, root);
				(*cTime)++;
			}
		}
		else if ((*lastAdded)->parent->parent->left != NULL && (*lastAdded)->parent->parent->left->left != NULL && (*lastAdded)->parent->parent->left->left->data == (*lastAdded)->data)
		{ // If lastAdded, parent and grandparent construct successive right directed line perform zigzig
			rightRotation(&((*lastAdded)->parent), root);
			rightRotation(lastAdded, root);
			*cTime += 2;
			Splay(root, lastAdded, cTime);
		}
		else if ((*lastAdded)->parent->parent->right != NULL && (*lastAdded)->parent->parent->right->right != NULL && (*lastAdded)->parent->parent->right->right->data == (*lastAdded)->data)
		{ // If lastAdded, parent and grandparent construct successive left directed line perform zigzig
			leftRotation(&((*lastAdded)->parent), root);
			leftRotation(lastAdded, root);
			*cTime += 2;
			Splay(root, lastAdded, cTime);
		}
		// check if it provides the condition for zigzag case
		else if ((*lastAdded)->parent->parent->left != NULL && (*lastAdded)->parent->parent->left->right != NULL && (*lastAdded)->parent->parent->left->right->data == (*lastAdded)->data)
		{
			leftRotation(lastAdded, root);
			rightRotation(lastAdded, root);
			*cTime += 2;
			Splay(root, lastAdded, cTime);
		}
		else if ((*lastAdded)->parent->parent->right != NULL && (*lastAdded)->parent->parent->right->left != NULL && (*lastAdded)->parent->parent->right->left->data == (*lastAdded)->data)
		{
			rightRotation(lastAdded, root);
			leftRotation(lastAdded, root);
			*cTime += 2;
			Splay(root, lastAdded, cTime);
		}
	}
}
StNodePtr Insert(StNodePtr root, int data, StNodePtr *lastAdded, int *cTime)
{
	if (root == NULL)
	{
		StNodePtr newNode = malloc(sizeof(StNode));
		newNode->data = data;
		newNode->freq = 0;
		newNode->left = newNode->right = newNode->parent = NULL;
		*lastAdded = newNode;
		return newNode;
	}
	else if (data < root->data)
	{
		(*cTime)++;
		root->left = Insert(root->left, data, lastAdded, cTime);
		root->left->parent = root;
	}
	else if (data > root->data)
	{
		(*cTime)++;
		root->right = Insert(root->right, data, lastAdded, cTime);
		root->right->parent = root;
	}
	else if (data == root->data)
	{
		(*cTime)++;
		(root->freq)++;
		*lastAdded = root;
	}
	return root;
}
void preorder(StNodePtr root, FILE *file)
{
	if (root != NULL)
	{
		fprintf(file, "(%d,%d) ", root->data, root->freq);
		preorder(root->left, file);
		preorder(root->right, file);
	}
}