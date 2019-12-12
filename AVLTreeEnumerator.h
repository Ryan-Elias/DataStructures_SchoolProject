#ifndef AVL_TREE_ENUMERATOR
#define AVL_TREE_ENUMERATOR

#include <stack>
#include "Enumerator.h"
#include "AVLTreeOrder.h"

template <typename S>
class AVLTree;

template <typename T>
class AVLTreeEnumerator : public Enumerator<T>
{
private:
	AVLTreeOrder order;
	std::stack<const AVLTree<T>*> traversalStack;
	void buildTraversalStack(const AVLTree<T>* current);
public:
	AVLTreeEnumerator(const AVLTree<T>* root, AVLTreeOrder order = AVLTreeOrder::inorder);
	virtual ~AVLTreeEnumerator();
	bool hasNext() const;
	T next();                        // throws ExceptionEnumerationBeyondEnd if no next item is available
	T peek() const;                  // throws ExceptionEnumerationBeyondEnd if no next item is available
	AVLTreeOrder getOrder();         // returns the order of this enumerator (preorder, inorder, or postorder)
};

// Add your implementation below this line.


#endif // !AVL_TREE_ENUMERATOR

template<typename T>
inline void AVLTreeEnumerator<T>::buildTraversalStack(const AVLTree<T>* current)
{
	//this will create a stack (essentially a list) of all of the nodes in the order specified in the constructor.

	//inorder (Left, Root, Right)
	//Preorder (Root, Left, Right)
	//Postorder (Left, Right, Root)

	if (current == NULL) {
		return;
	}
	else if (current->empty) {
		return;
	}



	if (this->order == AVLTreeOrder::postorder) {
		if (!current->empty) {
			this->traversalStack.push(current);
		}
		if (current->right != NULL) {
			buildTraversalStack(current->right);
		}
		if (current->left != NULL) {
			buildTraversalStack(current->left);
		}
	}
	else if (this->order == AVLTreeOrder::preorder) {
		if (current->right != NULL) {
			buildTraversalStack(current->right);
		}
		if (current->left != NULL) {
			buildTraversalStack(current->left);
		}
		if (!current->empty) {
			this->traversalStack.push(current);
		}
	}
	//if not pre or post order, then will be inorder
	else {
		if (current->right != NULL) {
			buildTraversalStack(current->right);
		}
		if (!current->empty) {
			this->traversalStack.push(current);
		}
		if (current->left != NULL) {
			buildTraversalStack(current->left);
		}
	}


}

template<typename T>
inline AVLTreeEnumerator<T>::AVLTreeEnumerator(const AVLTree<T>* root, AVLTreeOrder order)
{
	if (root->empty) {
		throw new ExceptionAVLTreeAccess;
	}
	this->order = order;
	buildTraversalStack(root);
}

template<typename T>
inline AVLTreeEnumerator<T>::~AVLTreeEnumerator()
{
	//traversal stack is an object and not a pointer so will be deleted anyway
}

template<typename T>
inline bool AVLTreeEnumerator<T>::hasNext() const
{
	if (traversalStack.empty()) {
		return false;
	}
	else {
		return true;
	}
}

template<typename T>
inline T AVLTreeEnumerator<T>::next()
{
	if (traversalStack.empty()) {
		throw new ExceptionEnumerationBeyondEnd;
	}
	T temp = traversalStack.top()->data;
	traversalStack.pop();
	return temp;
}

template<typename T>
inline T AVLTreeEnumerator<T>::peek() const
{
	if (traversalStack.empty()) {
		throw new ExceptionEnumerationBeyondEnd;
	}
	return traversalStack.top()->data;
}

template<typename T>
inline AVLTreeOrder AVLTreeEnumerator<T>::getOrder()
{
	return this->order;
}
