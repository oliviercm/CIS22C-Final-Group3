/********************************************************************************
************************************ BST ****************************************
*********************************************************************************
* This class is a BST (binary search tree) ADT.
*****************
***** USAGE *****
*****************
* 
*********************************************************************************/

#pragma once

#include "BST_Node.h"

template <typename K, typename V>
class BST
{
protected:
	int count;
	BST_Node<K, V>* head;

	BST_Node<K, V>* insert(K, V&, BST_Node<K, V>*);
	BST_Node<K, V>* remove(const K&, const V&, BST_Node<K, V>*);
	BST_Node<K, V>* findByKey(const K&, BST_Node<K, V>*);
	BST_Node<K, V>* findInorderSuccessor(BST_Node<K, V>*); //Pass the right child of the node to find the successor of.
	void deleteAll(BST_Node<K, V>*);
public:
	BST();
	virtual ~BST();
	/**
	* getHead
	*
	* @brief Returns head.
	*
	* @return head
	*/
	BST_Node<K, V>* getHead();
	/**
	* insert
	*
	* @brief Recursively inserts the data into a new node in the sorted tree.
	*
	* @param The data to insert, as a reference.
	*/
	void insert(K, V&);
	/**
	* delete
	*
	* @brief Recursively deletes the node with matching key from the tree.
	*
	* @param The key of the node to delete.
	*/
	void remove(const K&, const V&);
	/**
	* find
	*
	* @brief Recursively finds the node with the matching key from the tree.
	*
	* @param K The key to search for.
	*
	* @param bool Whether to search for matching key or matching data. True searches for matching key.
	*
	* @return The found BST_Node.
	*/
	V& findByKey(const K&);
	/**
	* deleteAll
	*
	* @brief Deletes all managed nodes using a queue.
	*/
	virtual void deleteAll();
};

template <typename K, typename V>
BST<K, V>::BST() : count(0), head(nullptr)
{
	
}

template <typename K, typename V>
BST<K, V>::~BST()
{
	deleteAll();
}

template <typename K, typename V>
BST_Node<K, V>* BST<K, V>::getHead()
{
	return head;
}
template <typename K, typename V>
void BST<K, V>::insert(K k, V& v)
{
	insert(k, v, head);
}

template <typename K, typename V>
BST_Node<K, V>* BST<K, V>::insert(K k, V& v, BST_Node<K, V>* root)
{
	//The BST is completely empty
	if (head == nullptr)
	{
		head = new BST_Node<K, V>(k, v);
		return head;
	}
	
	//If the root is non-existent, create a new node and return it - the new node will be saved in the previous call
	if (root == nullptr)
	{
		return new BST_Node<K, V>(k, v);
	}

	//If the new data is less than root, recur for left root
	if (k < root->key)
	{
		root->left = insert(k, v, root->left);
	}
	//If the new data is greater or equal than root, recur for right root
	else
	{
		root->right = insert(k, v, root->right);
	}

	//Return the root so previous recursion calls keep their current values for left and right
	return root;
}

template <typename K, typename V>
V& BST<K, V>::findByKey(const K& searchKey)
{
	return findByKey(searchKey, head)->value;
}

template <typename K, typename V>
BST_Node<K, V>* BST<K, V>::findByKey(const K& searchKey, BST_Node<K, V>* root)
{
	if (root == nullptr) //If the tree is empty, or we have reached a leaf node and haven't found the key, return null
	{
		return nullptr;
	}
	else
	{
		if (searchKey == root->key) //The node was found
		{
			return root;
		}
		else if (searchKey < root->key) //if key is less than root, search from the left.
		{
			return find(searchKey, root->left, searchKey);
		}
		else if (searchKey > root->key) //if key is greater than root, search from the right.
		{
			return find(searchKey, root->right, searchKey);
		}
	}

	return nullptr;
}

template <typename K, typename V>
void BST<K, V>::remove(const K& key, const V& value)
{
	remove(key, value, head);
}

template <typename K, typename V>
BST_Node<K, V>* BST<K, V>::remove(const K& key, const V& value, BST_Node<K, V>* root)
{
	if (root == nullptr) //if tree is empty return null.
	{
		return root;
	}
	else if (key < root->key)//if data is less than root, remove the root from the left.
	{
		root->left = remove(key, value, root->left);
	}
	else if (key > root->key)//if data is greater than root, remove the root from the right.
	{
		root->right = remove(key, value, root->right);
	}
	else
	{
		//if root has no child
		if (root->left == nullptr && root->right == nullptr)
		{
			delete root;
			root = nullptr;
		}
		else if (root->left == nullptr) //if root has only one right child
		{
			//Copy the values from the right child into root and then delete the right child
			BST_Node<K, V>* temp = root->right;
			root->key = temp->key;
			root->value = temp->value;
			root->left = temp->left;
			root->right = temp->right;
			delete temp;
			temp = nullptr;
		}
		else if (root->right == nullptr) //if root has only one left child
		{
			//Copy the values from the left child into root and then delete the left child
			BST_Node<K, V>* temp = root->left;
			root->key = temp->key;
			root->value = temp->value;
			root->left = temp->left;
			root->right = temp->right;
			delete temp;
			temp = nullptr;
		}
		else //if root has a left and right child.
		{
			//Copy the values from the inorder successor to the root and then delete the inorder successor
			BST_Node<K, V>* temp = findInorderSuccessor(root->right);
			root->key = temp->key;
			root->value = temp->value;
			root->right = remove(temp->key, temp->value, root->right);
		}
	}
	return root;
}

template <typename K, typename V>
BST_Node<K, V>* BST<K, V>::findInorderSuccessor(BST_Node<K, V>* root)
{
	BST_Node<K, V>* current = root;
	while (current->left != nullptr)
	{
		current = current->left;
	}
	return current;
}

template <typename K, typename V>
void BST<K, V>::deleteAll(BST_Node<K, V>* root)
{
	if (root == nullptr)
	{
		return;
	}

	deleteAll(root->left);
	deleteAll(root->right);
	delete root;
	root = nullptr;
}

template <typename K, typename V>
void BST<K, V>::deleteAll()
{
	deleteAll(head);
}