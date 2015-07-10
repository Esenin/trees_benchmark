//! @author http://users.cis.fiu.edu/~weiss/dsaa_c++/code/SplayTree.h

#pragma once

namespace tree
{
namespace splay2
{
// Node and forward declaration because g++ does
// not understand nested classes.
template<class Comparable>
class SplayTree;

template<class Comparable>
class BinaryNode
{
    Comparable element;
    BinaryNode* left;
    BinaryNode* right;

    BinaryNode()
            : left(NULL), right(NULL)
    { }

    BinaryNode(const Comparable &theElement, BinaryNode* lt, BinaryNode* rt)
            : element(theElement), left(lt), right(rt)
    { }

    friend class SplayTree<Comparable>;
};


template<class Comparable>
class SplayTree
{
public:
    explicit SplayTree(const Comparable &notFound);

    SplayTree(const SplayTree &rhs) = delete;

    ~SplayTree();

    const Comparable &findMin();

    const Comparable &findMax();

    bool contains(const Comparable &x) const;

    const Comparable &find(const Comparable &x);

    bool isEmpty() const;

    void makeEmpty();

    void insert(const Comparable &x);

    void remove(const Comparable &x);

    const SplayTree &operator=(const SplayTree &rhs);

private:
    mutable BinaryNode<Comparable>* root;
    mutable BinaryNode<Comparable>* nullNode;
    const Comparable ITEM_NOT_FOUND;

    void reclaimMemory(BinaryNode<Comparable>* t) const;

    BinaryNode<Comparable>* clone(BinaryNode<Comparable>* t) const;

    // Tree manipulations
    void rotateWithLeftChild(BinaryNode<Comparable>*&k2) const;

    void rotateWithRightChild(BinaryNode<Comparable>*&k1) const;

    void splay(const Comparable &x, BinaryNode<Comparable>*&t) const;
};


/**
 * Construct the tree.
 */
template<class Comparable>
SplayTree<Comparable>::SplayTree(const Comparable &notFound)
        : ITEM_NOT_FOUND(notFound)
{
    nullNode = new BinaryNode<Comparable>;
    nullNode->left = nullNode->right = nullNode;
    nullNode->element = notFound;
    root = nullNode;
}

/**
 * Destructor.
 */
template<class Comparable>
SplayTree<Comparable>::~SplayTree()
{
    makeEmpty();
    delete nullNode;
}

/**
 * Insert x into the tree.
 */
template<class Comparable>
void SplayTree<Comparable>::insert(const Comparable &x)
{
    static BinaryNode<Comparable>* newNode = NULL;

    if (newNode == NULL)
        newNode = new BinaryNode<Comparable>;
    newNode->element = x;

    if (root == nullNode)
    {
        newNode->left = newNode->right = nullNode;
        root = newNode;
    }
    else
    {
        splay(x, root);
        if (x < root->element)
        {
            newNode->left = root->left;
            newNode->right = root;
            root->left = nullNode;
            root = newNode;
        }
        else if (root->element < x)
        {
            newNode->right = root->right;
            newNode->left = root;
            root->right = nullNode;
            root = newNode;
        }
        else
            return;
    }
    newNode = NULL;   // So next insert will call new
}

/**
 * Remove x from the tree.
 */
template<class Comparable>
void SplayTree<Comparable>::remove(const Comparable &x)
{
    BinaryNode<Comparable>* newTree;

    // If x is found, it will be at the root
    splay(x, root);
    if (root->element != x)
        return;   // Item not found; do nothing

    if (root->left == nullNode)
        newTree = root->right;
    else
    {
        // Find the maximum in the left subtree
        // Splay it to the root; and then attach right child
        newTree = root->left;
        splay(x, newTree);
        newTree->right = root->right;
    }
    delete root;
    root = newTree;
}

/**
 * Find the smallest item in the tree.
 * Not the most efficient implementation (uses two passes), but has correct
 *     amortized behavior.
 * A good alternative is to first call Find with parameter
 *     smaller than any item in the tree, then call findMin.
 * Return the smallest item or ITEM_NOT_FOUND if empty.
 */
template<class Comparable>
const Comparable &SplayTree<Comparable>::findMin()
{
    if (isEmpty())
        return ITEM_NOT_FOUND;

    BinaryNode<Comparable>* ptr = root;

    while (ptr->left != nullNode)
        ptr = ptr->left;

    splay(ptr->element, root);
    return ptr->element;
}

/**
 * Find the largest item in the tree.
 * Not the most efficient implementation (uses two passes), but has correct
 *     amortized behavior.
 * A good alternative is to first call Find with parameter
 *     larger than any item in the tree, then call findMax.
 * Return the largest item or ITEM_NOT_FOUND if empty.
 */
template<class Comparable>
const Comparable &SplayTree<Comparable>::findMax()
{
    if (isEmpty())
        return ITEM_NOT_FOUND;

    BinaryNode<Comparable>* ptr = root;

    while (ptr->right != nullNode)
        ptr = ptr->right;

    splay(ptr->element, root);
    return ptr->element;
}

/**
 * Find item x in the tree.
 * Return the matching item or ITEM_NOT_FOUND if not found.
 */
template<class Comparable>
const Comparable &SplayTree<Comparable>::find(const Comparable &x)
{
    if (isEmpty())
        return ITEM_NOT_FOUND;
    splay(x, root);
    if (root->element != x)
        return ITEM_NOT_FOUND;

    return root->element;
}

template<class Comparable>
bool SplayTree<Comparable>::contains(const Comparable &x) const
{
    if (isEmpty())
        return false;
    splay(x, root);
    return root->element == x;
}

/**
 * Make the tree logically empty.
 */
template<class Comparable>
void SplayTree<Comparable>::makeEmpty()
{
    /******************************
     * Comment this out, because it is prone to excessive
     * recursion on degenerate trees. Use alternate algorithm.

        reclaimMemory( root );
        root = nullNode;
     *******************************/
    findMax();        // Splay max item to root
    while (!isEmpty())
        remove(root->element);
}

/**
 * Test if the tree is logically empty.
 * @return true if empty, false otherwise.
 */
template<class Comparable>
bool SplayTree<Comparable>::isEmpty() const
{
    return root == nullNode;
}

/**
 * Deep copy.
 */
template<class Comparable>
const SplayTree<Comparable> &
SplayTree<Comparable>::operator=(const SplayTree<Comparable> &rhs)
{
    if (this != &rhs)
    {
        makeEmpty();
        root = clone(rhs.root);
    }

    return *this;
}

/**
 * Internal method to perform a top-down splay.
 * The last accessed node becomes the new root.
 * This method may be overridden to use a different
 * splaying algorithm, however, the splay tree code
 * depends on the accessed item going to the root.
 * x is the target item to splay around.
 * t is the root of the subtree to splay.
 */
template<class Comparable>
void SplayTree<Comparable>::splay(const Comparable &x, BinaryNode<Comparable>*&t) const
{
    BinaryNode<Comparable>* leftTreeMax, * rightTreeMin;
    static BinaryNode<Comparable> header;

    header.left = header.right = nullNode;
    leftTreeMax = rightTreeMin = &header;

    nullNode->element = x;   // Guarantee a match

    for (; ;)
        if (x < t->element)
        {
            if (x < t->left->element)
                rotateWithLeftChild(t);
            if (t->left == nullNode)
                break;
            // Link Right
            rightTreeMin->left = t;
            rightTreeMin = t;
            t = t->left;
        }
        else if (t->element < x)
        {
            if (t->right->element < x)
                rotateWithRightChild(t);
            if (t->right == nullNode)
                break;
            // Link Left
            leftTreeMax->right = t;
            leftTreeMax = t;
            t = t->right;
        }
        else
            break;

    leftTreeMax->right = t->left;
    rightTreeMin->left = t->right;
    t->left = header.right;
    t->right = header.left;
}

/**
 * Rotate binary tree node with left child.
 */
template<class Comparable>
void SplayTree<Comparable>::rotateWithLeftChild(BinaryNode<Comparable>*&k2) const
{
    BinaryNode<Comparable>* k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2 = k1;
}

/**
 * Rotate binary tree node with right child.
 */
template<class Comparable>
void SplayTree<Comparable>::rotateWithRightChild(BinaryNode<Comparable>*&k1) const
{
    BinaryNode<Comparable>* k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1 = k2;
}

/**
 * Internal method to reclaim internal nodes in subtree t.
 * WARNING: This is prone to running out of stack space.
 */
template<class Comparable>
void SplayTree<Comparable>::reclaimMemory(BinaryNode<Comparable>* t) const
{
    if (t != t->left)
    {
        reclaimMemory(t->left);
        reclaimMemory(t->right);
        delete t;
    }
}

/**
 * Internal method to clone subtree.
 * WARNING: This is prone to running out of stack space.
 */
template<class Comparable>
BinaryNode<Comparable>*
SplayTree<Comparable>::clone(BinaryNode<Comparable>* t) const
{
    if (t == t->left)  // Cannot test against nullNode!!!
        return nullNode;
    else
        return new BinaryNode<Comparable>(t->element, clone(t->left), clone(t->right));
}

}
}