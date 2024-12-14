#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

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

int main(int argc, char *argv[])
{
	int TimeForModSplay = 0;
	int TimeForSplay = 0;

	StNodePtr root = NULL;
	StNodePtr modRoot = NULL;

	StNodePtr lastAdded = NULL;
	StNodePtr modLastAdded = NULL;

	FILE *file = fopen("input.txt", "r");
	FILE *output = fopen("output.txt", "w");
	int key = 0;
	char x = fgetc(file);

	while (!feof(file))
	{
		if (x - '0' < 10 && x - '0' >= 0)
			key += key * 10 + (x - '0');
		else
		{
			root = Insert(root, key, &lastAdded, &TimeForSplay);
			modRoot = Insert(modRoot, key, &modLastAdded, &TimeForModSplay);
			if (lastAdded->freq == 0)
				Splay(&root, &lastAdded, &TimeForSplay);

			if (modRoot->freq < modLastAdded->freq)
				Splay(&modRoot, &modLastAdded, &TimeForModSplay);

			key = 0;
		}
		x = fgetc(file);
	}
	fprintf(output, "\n Pre-order demonstration of splay tree: ");
	preorder(root, output);
	fprintf(output, "\n Comparison time of splay tree: %d \n", TimeForSplay);
	fprintf(output, "\n Pre-order demonstration of mod-splay tree: ");
	preorder(modRoot, output);
	fprintf(output, "\n Comparison time of mod-splay tree: %d", TimeForModSplay);

	/*
		root = Insert(root, 10, &lastAdded);
		// Splay(&root, &lastAdded);

		root = Insert(root, 11, &lastAdded);
		//	Splay(&root, &lastAdded);

		root = Insert(root, 2, &lastAdded);
		//	Splay(&root, &lastAdded);

		root = Insert(root, 1, &lastAdded);
		//	Splay(&root, &lastAdded);

		root = Insert(root, 4, &lastAdded);
		//	Splay(&root, &lastAdded);
		root = Insert(root, 3, &lastAdded);
		root = Insert(root, 9, &lastAdded);
		root = Insert(root, 5, &lastAdded);
		root = Insert(root, 8, &lastAdded);
		root = Insert(root, 7, &lastAdded);
		root = Insert(root, 6, &lastAdded);
		root = Insert(root, 6, &lastAdded);
		Splay(&root, &lastAdded);
		preorder(root);
		return 0;*/
}
void zigZig(StNodePtr *lastAdded, StNodePtr *root, int leftOrRight)
{
	// 0 for left, 1 for right.
	// left zigzig
	if (leftOrRight == 0)
	{

		(*lastAdded)->parent->parent->left = (*lastAdded)->parent->right;

		if ((*lastAdded)->parent->right != NULL)
			(*lastAdded)->parent->right->parent = (*lastAdded)->parent->parent;

		(*lastAdded)->parent->right = (*lastAdded)->parent->parent;
		(*lastAdded)->parent->parent = (*lastAdded)->parent->parent->parent;
		(*lastAdded)->parent->right->parent = (*lastAdded)->parent;
		//---------------------------
		(*lastAdded)->parent->left = (*lastAdded)->right;

		if ((*lastAdded)->right != NULL)
			(*lastAdded)->right->parent = (*lastAdded)->parent;

		(*lastAdded)->right = (*lastAdded)->parent;
		(*lastAdded)->parent = (*lastAdded)->parent->parent;

		(*lastAdded)->right->parent = *lastAdded;

		if ((*lastAdded)->parent == NULL)
			*root = *lastAdded;
		else
		{
			if (((*lastAdded)->parent->right != NULL) && (*lastAdded)->parent->right->data == (*lastAdded)->right->right->data)
				(*lastAdded)->parent->right = *lastAdded;
			else if (((*lastAdded)->parent->left != NULL) && (*lastAdded)->parent->left->data == (*lastAdded)->right->right->data)
				(*lastAdded)->parent->left = *lastAdded;
		}
	}
	// right zizig
	else
	{

		(*lastAdded)->parent->parent->right = (*lastAdded)->parent->left;

		if ((*lastAdded)->parent->left != NULL)
			(*lastAdded)->parent->left->parent = (*lastAdded)->parent->parent;

		(*lastAdded)->parent->left = (*lastAdded)->parent->parent;
		(*lastAdded)->parent->parent = (*lastAdded)->parent->parent->parent;
		(*lastAdded)->parent->left->parent = (*lastAdded)->parent;
		//---------------------------
		(*lastAdded)->parent->right = (*lastAdded)->left;

		if ((*lastAdded)->left != NULL)
			(*lastAdded)->left->parent = (*lastAdded)->parent;

		(*lastAdded)->left = (*lastAdded)->parent;
		(*lastAdded)->parent = (*lastAdded)->parent->parent;

		(*lastAdded)->left->parent = *lastAdded;

		if ((*lastAdded)->parent == NULL)
			*root = *lastAdded;

		else
		{
			if (((*lastAdded)->parent->left != NULL) && (*lastAdded)->parent->left->data == (*lastAdded)->left->left->data)
				(*lastAdded)->parent->left = *lastAdded;
			else if (((*lastAdded)->parent->right != NULL) && (*lastAdded)->parent->right->data == (*lastAdded)->left->left->data)
				(*lastAdded)->parent->right = *lastAdded;
		}
	}
}

void leftRotation(StNodePtr *lastAdded, StNodePtr *root)
{

	// counter clockwise rotation.
	(*lastAdded)->parent->right = (*lastAdded)->left;
	(*lastAdded)->left = (*lastAdded)->parent;
	(*lastAdded)->parent = (*lastAdded)->parent->parent;

	if ((*lastAdded)->left->right != NULL)
		(*lastAdded)->left->right->parent = (*lastAdded)->left;

	(*lastAdded)->left->parent = (*lastAdded);

	if ((*lastAdded)->parent == NULL)
		*root = *lastAdded;
	else
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

		if ((*lastAdded)->parent->parent == NULL)
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
		{
			zigZig(lastAdded, root, 0);
			*cTime += 2;
			Splay(root, lastAdded, cTime);
		}
		else if ((*lastAdded)->parent->parent->right != NULL && (*lastAdded)->parent->parent->right->right != NULL && (*lastAdded)->parent->parent->right->right->data == (*lastAdded)->data)
		{
			zigZig(lastAdded, root, 1);
			*cTime += 2;
			Splay(root, lastAdded, cTime);
		}
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
		fprintf(file, "%d ", root->data);
		preorder(root->left, file);
		preorder(root->right, file);
	}
}