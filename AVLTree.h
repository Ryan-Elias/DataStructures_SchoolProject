#ifndef AVL_TREE
#define AVL_TREE

#include "Comparator.h"
#include "AVLTreeEnumerator.h"

template <typename T>
class AVLTree {
	template <typename U>
	friend class AVLTreeEnumerator;
private:
	Comparator<T>* comparator = NULL;               // used to determine tree order and item equality
	unsigned long size = 0;                         // actual number of items currently in tree
	T data{};                                       // data item of any type
	bool empty = true;                              // flag to indicate whether node contains data
	int diff = 0;                                   // height of right minus height of left
	AVLTree<T>* left = NULL;                        // pointer to left subtree
	AVLTree<T>* right = NULL;                       // pointer to right subtree
	void zig();                                     // right rotation
	void zag();                                     // left rotation
	void zigzag();                                  // left rotation on left subtree, followed by right rotation
	void zagzig();                                  // right rotation on right subtree, followed by left rotation
	void rebalance();                               // check for and rebalance this node, if needed
public:
	AVLTree(Comparator<T>* comparator);             // creates empty linked tree with comparator
	virtual ~AVLTree();                             // deletes subtrees

	// if an equivalent item is not already present, insert item in order and return true
	// if an equivalent item is already present, leave tree unchanged and return false
	bool insert(const T item);

	// if an equivalent item is already present, replace item and return true
	// if an equivalent item is not already present, leave tree unchanged and return false
	bool replace(const T item);

	// if an equivalent item is already present, remove item and return true
	// if an equivalent item is not already present, leave tree unchanged and return false
	bool remove(const T item);

	// if an equivalent item is present, return true
	// if an equivalent item is not present, false
	bool contains(T item) const;

	// if an equivalent item is present, return a copy of the item
	// if an equivalent item is not present, throw a new ExceptionLinkedtreeAccess
	T find(const T item) const;

	unsigned long getSize() const;                  // returns the current number of items in the tree

	AVLTreeEnumerator<T> enumerator(AVLTreeOrder order = AVLTreeOrder::inorder) const;    // create an enumerator for this AVL tree
	
																						  /*
	//test methods, will delete later
	AVLTree<T>* getLeft();
	AVLTree<T>* getRight();
	T getData();
	int getDiff();
	void zigTest();
	void zagTest();
	void zigzagTest();
	void zagzigTest();
	void rebalanceTest();
	*/
};

// Add your implementation below this line.

#endif // !AVL_TREE

template<typename T>
inline AVLTree<T>::AVLTree(Comparator<T>* comparator)
{
	this->comparator = comparator;


}

template<typename T>
inline AVLTree<T>::~AVLTree()
{
	delete left;
	delete right;
}

template<typename T>
inline bool AVLTree<T>::insert(const T item)
{
//recursive insert
	// if node is empty, adds item to data, updates information, returns true.
	if (this->empty) {
		this->data = item;
		size++;
		this->empty = false;
		// should not need to call rebalance as this will only be the case if the root is empty (as all other nodes are filled when they are created)
		return true;
	}
	// if item is equal to the item at that node, then returns false
	else if (comparator->compare(item, data) == 0) {
		return false;
	}
	else {
		//recursively adds to left or right node accordingly. 
		int rootDiff;
		//if item is less than the item at that data, return insert->left node
		if (comparator->compare(item, data) < 0) {
			if (this->left == NULL) {
				this->left = new AVLTree<T>(comparator);
				this->left->data = item;
				this->left->empty = false;
				diff--;
				//return true;
			}
			else {
				rootDiff = this->left->diff;
				if (this->left->insert(item)) {
					if ((rootDiff != this->left->diff) && (this->left->diff != 0)) {
						this->diff--;
					}
				}
				else {
					return false;
				}
			}
		}
		else if (comparator->compare(item, data) > 0) {
			if (this->right == NULL) {
				this->right = new AVLTree<T>(comparator);
				this->right->data = item;
				this->right->empty = false;
				diff++;
			}
			else {
				rootDiff = this->right->diff;
				if (this->right->insert(item)) {
					if ((rootDiff != this->right->diff) && (this->right->diff != 0)) {
						this->diff++;
					}
				}
				else {
					return false;
				}
			}
		}
	}
	this->rebalance();
	this->size++;
	return true;

}
template<typename T>
inline bool AVLTree<T>::replace(const T item)
{
	//in order to replace an item, the item must be found.
	//will recursively call replace until item is found or until the nodes are NULL.

	if (this->empty) {
		return false;
	}
	else if (comparator->compare(this->data, item) == 0) {
		this->data = item;
		return true;
	}
	else if (comparator->compare(item, this->data) < 0) {
		if (this->left == NULL) {
			return false;
		}
		return this->left->replace(item);
	}
	else if (comparator->compare(item, this->data) > 0) {
		if (this->right == NULL) {
			return false;
		}
		return this->right->replace(item);
	}
	else {
		//this point should not be reached
		std::cout << "something is wrong with the replace function" << std::endl;
		return false;
	}


}

//REMOVE METHOD WRITTEN FROM CLASS SLIDES

template<typename T>
inline bool AVLTree<T>::remove(const T item)
{
	//if item found, remove. 
	//if item was not found, then return false and leave tree unchanged.

	//when removing a node, that node's children must be moved to be the children of the parent node to accomodate the now missing node.

	//cases for this would be: 1. Tree is empty, including Root (or is NULL)
	// 2. Left and Right nodes are empty (but has root)
	// 3. Left node is empty and right node is not
	// 4. Right node is empty and left node is not

	int oldDiff;
	
	if (this->empty) {
		return false;
	}
	//if item to remove is less than the root nodes' data
	else if (comparator->compare(item, data) < 0) {
		if (this->left == NULL) {
			return false;
		}
		else {
			oldDiff = this->left->diff;
			//might need to make sure this evaluates to true before continuing.
			if (this->left->remove(item)) {
				if ((this->left == NULL || this->left->empty) || ((this->left->diff != oldDiff) && (this->left->diff == 0))) {
					this->diff++;
				}
			}
			else {
				return false;
			}
		}
	}
	//if item to remove is greater than the root nodes' data
	else if (comparator->compare(item, data) > 0) {
		if (this->right == NULL) {
			return false;
		}
		else {
			oldDiff = this->right->diff;
			if (this->right->remove(item)) {
				if ((this->right == NULL || this->right->empty) || ((this->right->diff != oldDiff) && (this->right->diff == 0))) {
					this->diff--;
				}
			}
			else {
				return false;
			}
		}
	}
	//if the item to remove is equal to the item at that node (item to be removed has been found)
	else {
		//match found

		if (this->right == NULL) {
			delete this->right;
			AVLTree<T>* tempTree = this->left;
			if (this->left != NULL) {
				this->data = left->data;
				if (this->left->left != NULL) {
					this->left = left->left;
				}
				if (this->left->right != NULL) {
					this->right = left->right;
				}
			}
			this->diff = 0;
			if (tempTree != NULL) {
				tempTree->left = NULL;
				tempTree->right = NULL;
			}


			delete tempTree;
		}
		else if (this->left == NULL) {
			delete this->left;
			AVLTree<T>* tempTree = this->right;
			this->data = this->right->data;
			this->left = tempTree->left;
			this->right = tempTree->right;

			tempTree->left = NULL;
			tempTree->right = NULL;
			delete tempTree;
			this->diff = 0;
		}
		else {
			//find successor
			AVLTree<T>* nextRoot = this->right;
			while (nextRoot->left != NULL) {
				nextRoot = nextRoot->left;
			}
			this->data = nextRoot->data;
			int rDiff = this->right->diff;
			this->right->remove(nextRoot->data);

			if ((this->right == NULL || this->right->empty) || ((this->right->diff != rDiff) && (this->right->diff == 0))) {
				this->diff--;
			}

			nextRoot->left = NULL;
			nextRoot->right = NULL;


		}
	}
	this->rebalance();
	this->size--;
	return true;

}
template<typename T>
inline bool AVLTree<T>::contains(T item) const
{
	//will recursively call contains

	if (this->empty) {
		return false;
	}
	else if (comparator->compare(this->data, item) == 0) {
		return true;
	}
	else {
		if (comparator->compare(item, this->data) < 0) {
			if (this->left == NULL) {
				return false;
				}
			return this->left->contains(item);
		}
		else if (comparator->compare(item, this->data) > 0) {
			if (this->right == NULL) {
				return false;
			}
			return this->right->contains(item);
		}
		else {
			std::cout << "something is wrong in contain method" << std::endl;
		}
	}

	return false;
}
template<typename T>
inline T AVLTree<T>::find(const T item) const
{
	//will work the same as contains function but will instead return the item.

	if (this->empty) {
		throw new ExceptionAVLTreeAccess;
	}
	else if (comparator->compare(this->data, item) == 0) {
		return this->data;
	}
	else {
		if (comparator->compare(item, this->data) < 0) {
			if (this->left == NULL) {
				throw new ExceptionAVLTreeAccess;
			}
			return this->left->find(item);
		}
		else if (comparator->compare(item, this->data) > 0) {
			if (this->right == NULL) {
				throw new ExceptionAVLTreeAccess;
			}
			return this->right->find(item);
		}
		else {
			std::cout << "something is wrong in find method" << std::endl;
		}
	}

	return T(); // ???

}
template<typename T>
inline unsigned long AVLTree<T>::getSize() const
{
	return this->size;
}

template<typename T>
inline AVLTreeEnumerator<T> AVLTree<T>::enumerator(AVLTreeOrder order) const
{
	return AVLTreeEnumerator<T>(this, order);
}
/*
template<typename T>
inline AVLTree<T>* AVLTree<T>::getLeft()
{
	return this->left;
}

template<typename T>
inline AVLTree<T>* AVLTree<T>::getRight()
{
	return this->right;
}

template<typename T>
inline T AVLTree<T>::getData()
{
	return data;
}

template<typename T>
inline int AVLTree<T>::getDiff()
{
	return this->diff;
}

template<typename T>
inline void AVLTree<T>::zigTest()
{
	//this will change with each test
	this->zig();
}

template<typename T>
inline void AVLTree<T>::zagTest()
{
	this->zag();
}

template<typename T>
inline void AVLTree<T>::zigzagTest()
{
	this->zigzag();
}

template<typename T>
inline void AVLTree<T>::zagzigTest()
{
	this->zagzig();
}

template<typename T>
inline void AVLTree<T>::rebalanceTest()
{
	this->rebalance();
}
*/
//ZIG AND ZAG HEAVILY INFLUENCED FROM SLIDES FORM CLASS AS WELL AS FILES ON CANVAS

template<typename T>
inline void AVLTree<T>::zig()
{
	//right rotation

	//in a right rotation, the root's left becomes the new root.
	//left of root's left remains the left pointer of that node
	//the root becomes the right node of the  new root
	//whatever was the right pointer of the new root becomes the left pointer of the old root (current right)

	//because there is no parent pointer, that means that the pointer to this node must remain the same, meaning the data of this node will have to change.

	if (this->left == NULL) {
		return;
	}

	AVLTree<T>* leftRightNode = this->left->right;
	AVLTree<T>* tempNode = this->left;
	T tempData = data;
	this->data = tempNode->data;
	tempNode->data = tempData;
	//also need to change the diff values for each node
	int parent = this->left->diff;
	int grandparent = this->diff;

	
	//now that there is a pointer to the left node, we can replace the left node with tempNode's left.

	this->left = tempNode->left;

	//because the data of the nodes will have to be changed, the tempNode will be used as the right node for the updated tree.
	tempNode->left = leftRightNode;

	tempNode->right = this->right;

	this->right = tempNode;

	//if the balance of the child leaned towards the left
	if (parent < 0) {
		this->diff = grandparent + 2;
		this->right->diff = grandparent - parent + 1;
	}
	//if the balance of the child leaned to the right
	else {
		this->diff = parent + 1;
		this->right->diff = grandparent + 1;

	}



}

template<typename T>
inline void AVLTree<T>::zag()
{
	//left rotate
	if (this->right == NULL) {
		return;
	}

	AVLTree<T>* tempTree = this->right;
	AVLTree<T>* rightLeftTree = this->right->left;
	T tempData = this->data;

	this->data = tempTree->data;
	tempTree->data = tempData;
	int parent = this->right->diff;
	int grandparent = this->diff;




	this->right = tempTree->right;
	tempTree->left = rightLeftTree;

	tempTree->left = this->left;
	this->left = tempTree;
	tempTree->right = rightLeftTree;

	if (parent > 0) {
		this->diff = grandparent - 2;
		this->left->diff = grandparent - parent - 1;
	}
	else {
		this->diff = parent - 1;
		this->left->diff = grandparent - 1;
	}

	

}

template<typename T>
inline void AVLTree<T>::zigzag()
{
	// right rotate then left rotate
	// done when there is a right - left case

	//so a right rotate would be performed on this->right and then a left rotate would be performed on the root
	this->right->zig();
	this->zag();




}

template<typename T>
inline void AVLTree<T>::zagzig()
{
	//left rotate then right rotate
	//when there is a left -right case
	// a left rotate is done on this->left and then a right rotate is done on the root.

	this->left->zag();
	this->zig();
}

template<typename T>
inline void AVLTree<T>::rebalance()
{
	// There are 4 cases to consider:

	// 1. Left - Left
	// 2. Left - Right
	// 3. Right - Right
	// 4. Right - Left

	if (this->diff == -1 || this->diff == 1) {
		return;
	}
	else if (this->diff == -2) {
		//Left - Right
		if (this->left->diff == 1) {
			this->zagzig();
		}
		//Left - Left
		else {
			this->zig();
		}
	}
	else if (this->diff == 2) {
		//Right - Left
		if (this->right->diff == -1) {
			this->zigzag();
		}
		else {
			this->zag();
		}
	}


}
