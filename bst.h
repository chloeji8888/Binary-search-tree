#ifndef HW2_BST_H
#define HW2_BST_H

#include<iostream>
#include <utility>
#include <functional>
#include <stdexcept>

namespace pic10c {

    template<typename T, typename compare_type = std::less<T>>

    class bst {

    private:

        class node;

        node *root;

        compare_type pred;

        size_t sz;

        /**
         * Function to recursively delete the tree
         * @param Node to delete
         */
        void deleteTree(node *Node);

        /**
         * Function to help with copying
         * @param Node to copy
         */
        void traverseInsert(node *Node);


    public:
        /**
         * Single Default constructor
         * @param _compare
         */
        bst(const compare_type &_compare = compare_type()) : pred(_compare), root(nullptr), sz(0) {}

        class iterator;

        /**
         * De constructor
         */
        ~bst();

        /**
         * copy constructor
         * @param rhs
         */
        bst(const bst &rhs) : root(nullptr), sz(rhs.sz), pred(rhs.pred) {
            traverseInsert(rhs.root);
        }

        /**
         * move constructor
         * @param rhs
         */
        bst(bst &&rhs) noexcept;

        /**
         * copy assignment
         * @param rhs
         * @return
         */
        bst &operator=(const bst &rhs);

        /**
         * move assigment
         * @param rhs
         * @return
         */
        bst &operator=(bst &&rhs);

        /**
         * iterator points to the beginning of the tree
         * @return
         */
        iterator begin() const;

        /**
         * iterator points to the end of hte tree
         * @return
         */
        iterator end() const;

        /**
         * member function swap
         * @param rhs
         */
        void swap(const bst &rhs);

        /**
         * free function swap
         * @param left
         * @param right
         */
        friend void swap(const bst &left, const bst &right);

        /**
         * insertNode function that insert the node in the right position
         * @param n
         */
        bool insertNode(node *parent, node *n);

        /**
         * Insert member function with O(log n) time cost, suitably implemented to
         * take in a T and attempt to add the value to the tree, dealing with rvalues and
         * lvalues appropriately;
         * @param value
         */
        void insert(const T &value);

        /**
         * emplace function that accepts a variadic list of arguments to construct a
         * T and attempt to place it within the tree with O(log n) time cost;
         * @tparam Type
         * @param _value
         */
        template<typename ...Type>
        void emplace(Type &&..._value);

        /**
         * erase member function with O(log n) time cost accepting an iterator and
         * removing the no de managed by the iterator from the tree
         * @param eraseNode
         */
        void erase(iterator eraseNode);

        /**
         * size member function returning the numb er of elements in the bst
         * @return
         */
        const size_t size();

        /**
         * Find member function running in O(log n) time cost returning the iterator
         * to the no de with a given value if found and otherwise returning the past-the-end
         * iterator.
         * @param value
         * @return
         */
        iterator &find(T &value);

    };

    template<typename T, typename compare_type>
    class bst<T, compare_type>::node {
        friend bst;
        friend iterator;
    private:
        T value;
        node *left;
        node *right;
        node *parent;

        /**
         * Function to get the left most node of the tree
         * @return the leftmost node
         */
        node *leftMost();

        /**
         * Function to get the right most of the tree
         * @return the rightmost node
         */
        node *rightMost();

        /**
         * function to replace deleted node with a new node
         * @return
         */
        node *replace();

        node(T _value) : value(_value), right(nullptr), left(nullptr), parent(nullptr) {}

    };


    template<typename T, typename compare_type>
    class bst<T, compare_type>::iterator {
        friend bst;
    private:
        node *curr;//current position
        const bst *container;//pointer to the tree
        iterator(node *n, const bst *_container) : curr(n), container(_container) {}

    public:
        /**
         * prefix++
         * @return
         */
        iterator &operator++();

        /**
         * prefix --
         * @return
         */
        iterator &operator--();

        /**
         * postfix++
         * @return
         */
        iterator operator++(int);

        /**
         * postfix--
         * @return
         */
        iterator operator--(int);

        /**
         * member function == operator
         * @param rhs
         * @return
         */
        friend bool operator==(const iterator &first, const iterator &second){
            return ((first.curr == second.curr) && (first.container == second.container));
        }

        /**
         * member function != operator
         * @param rhs
         * @return
         */
        friend bool operator!=(const iterator &first, const iterator &second){
            return !((first.curr == second.curr) && (first.container == second.container));
        }

        /**
         * De referencing operator
         * @return
         */
        const T &operator*() const;

        /**
         * Arrow operator
         * @return
         */
        const T *operator->() const;
    };

    //BTS---------------------------------------------------------------------------------------------------------------

    //Delete each node
    template<typename T, typename compare_type>
    void bst<T, compare_type>::deleteTree(bst::node *Node) {
        if (Node->right != nullptr) {//if right child exist
            deleteTree(Node->right);//recurse in right
        }else if (Node->left != nullptr) {//if left child exist
            deleteTree(Node->left);//recurse in left
        }
        delete Node;//delete node
    }

    //Deconstruct
    template<typename T, typename compare_type>
    bst<T, compare_type>::~bst() {
        if (root != nullptr) {
            deleteTree(root);
        }
    }

    //move constructor
    template<typename T, typename compare_type>
    bst<T, compare_type>::bst(bst &&rhs) noexcept:bst() {
        std::swap(rhs.root, root);
        std::swap(rhs.pred, pred);
        std::swap(rhs.sz, sz);
    }

    //copy assignment
    template<typename T, typename compare_type>
    pic10c::bst<T, compare_type> &bst<T, compare_type>::operator=(const bst &rhs) {
        bst &copy(rhs);
        std::swap(copy.root, root);
        std::swap(copy.pred, pred);
        std::swap(copy.sz, sz);
        return *this;
    }


    template<typename T, typename compare_type>
    pic10c::bst<T, compare_type> &bst<T, compare_type>::operator=(bst &&rhs) {
        std::swap(this->root, rhs.root);
        std::swap(this->pred, rhs.pred);
        std::swap(this->sz, rhs.sz);
        return *this;
    }

    template<typename T, typename compare_type>
    typename bst<T, compare_type>::iterator bst<T, compare_type>::begin() const {
        if (root == nullptr) {
            return iterator(nullptr, this);
        }
        node *n = root;
        if (n->left != nullptr) {
            n = n->left;
        }
        return iterator(n, this);
    }

    template<typename T, typename compare_type>
    typename bst<T, compare_type>::iterator bst<T, compare_type>::end() const {
        return iterator(nullptr, this);
    }

    template<typename T, typename compare_type>
    void bst<T, compare_type>::swap(const bst &rhs) {
        std::swap(rhs.sz, sz);
        std::swap(rhs.pred, pred);
        std::swap(rhs.root, root);
    }

    template<typename T, typename compare_type>
    void swap(const bst<T, compare_type> &left, const bst<T, compare_type> &right) {
        left.swap(right);
    }

    template<typename T, typename compare_type>
    bool bst<T, compare_type>::insertNode(bst::node *parent, bst::node *n) {
        if (pred(parent->value, n->value)) {//if n is greater than its parent
            //goes to the right
            if (parent->right != nullptr) {//if the parent right is not null
                insertNode(parent->right, n);//go to the right and recurs
            }else {//if the right is null
                //make the n the right node
                parent->right = n;
                n->parent = parent;
            }
            return true;
        }else if (pred(n->value, parent->value)) {//if n is smaller that its parent
            //goes to the left
            if (parent->left != nullptr) {
                insertNode(parent->left, n);
                //go to the left and recurse
            }else {
                //make the n the left node
                parent->left = n;
                n->parent = parent;
            }
            return true;
        }else {//if the node is equal to the
            delete n;
            return false;
        }
    }

    template<typename T, typename compare_type>
    void bst<T, compare_type>::insert(const T &value) {
        if (!root) {//if there is no root
            root = new node(value);//create a new node
            ++sz;//increment sz;
        }else {//if there is root
            node *newNode = new node(value);//create a new node
            if (insertNode(root, newNode)) {
                ++sz;//increment size;
            }//check where to put that node
        }
    }

    template<typename T, typename compare_type>
    void bst<T, compare_type>::traverseInsert(bst::node *Node) {
        insert(Node->value);
        if (Node->left != nullptr) {
            traverseInsert(Node->left);
        }
        if (Node->right != nullptr) {
            traverseInsert(Node->right);
        }
    }

    template<typename T, typename compare_type>
    template<typename... Type>
    void bst<T, compare_type>::emplace(Type &&... _value) {
        if (!root) {//if there is no root
            root = new node(T(std::forward<Type>(_value)...));;//make the root the new Node
            ++sz;//increment sz;
        }else {//if there is root
            node *newNode = new node(T(std::forward<Type>(_value)...));//create a new node
            insertNode(root, newNode);//check where to put that node
            ++sz;//increment size;
        }
    }

    template<typename T, typename compare_type>
    void bst<T, compare_type>::erase(bst::iterator eraseNode) {
        node *toDelete = eraseNode.curr;
        if (toDelete->left == nullptr || toDelete->right == nullptr) {
            //if only have one side
            node *newNode = toDelete->replace();
            if (toDelete->parent == nullptr) {//toDelete is a root
                root = newNode;
            }
            delete toDelete;
            return;
        }else {
            node *smallest = toDelete->right->leftMost();//find the substitute node
            toDelete->value = smallest->value;//make the substitute node to replace the position of the deleted node
            node *newNode = smallest->replace();//get the node to replace the smallest node
            if (smallest->parent == nullptr) {//smallest is the root
                root = newNode;//make the newNode the root
            }
            delete smallest;//delete the smallest node
        }
    }

    template<typename T, typename compare_type>
    const size_t bst<T, compare_type>::size() {
        return sz;
    }

    template<typename T, typename compare_type>
    typename bst<T, compare_type>::iterator &bst<T, compare_type>::find(T &value) {
        for (auto i = begin(); i != end(); ++i) {
            if (*i == value) {
                return i;
            }
        }
        return end();
    }

    //Node definitions----------------------------------------------------------------------------------------------

    template<typename T, typename compare_type>
    typename bst<T, compare_type>::node *bst<T, compare_type>::node::leftMost() {
        if (left == nullptr) {//if left child is null
            return this;//return this
        }
        return left->leftMost();//go to the left
    }

    template<typename T, typename compare_type>
    typename bst<T, compare_type>::node *bst<T, compare_type>::node::rightMost() {
        if (right == nullptr) {//if right child is null
            return this;//return this
        }
        return right->rightMost();//go to the right
    }

    template<typename T, typename compare_type>
    typename bst<T, compare_type>::node *bst<T, compare_type>::node::replace() {
        node *newNode;//create a new node
        if (this->left == nullptr) {//if the node only have right child
            newNode = this->right;//make the replacement to the right child of it
        }
        if (this->right == nullptr) {//if the node only have left child
            newNode = this->left;//make the replacement to the left child of it
        }
        if (parent == nullptr) {// if the parent is null
            if (newNode != nullptr) {//but the newNode is not Null
                newNode->parent = nullptr;//make the newNode the root
            }
        }else {
            if (parent->left == this) {//if the node that we try to delete is on the left of its parent
                parent->left = newNode;//make the newNode on the left
                if (newNode != nullptr) {//if the newNode is not null
                    newNode->parent = parent;//point back to its parent
                }
            }else if (parent->right == this) {//if the node that we try to delete is on the left of its parent
                parent->right = newNode;//make the newNode on the right
                if (newNode != nullptr) {//if the newNode is not null
                    newNode->parent = parent;//point back to its parent
                }
            }
        }
        return newNode;//return the newNode
    }


    //Iterator definitions ---------------------------------------------------------------------------------------------

    template<typename T, typename compare_type>
    typename bst<T, compare_type>::iterator &bst<T, compare_type>::iterator::operator++() {
        if (this->curr == nullptr) {//reach the end, can not go further
            throw std::logic_error("Out of scoop");
        }else if (this->curr == container->root->rightMost()) {//reach the end
            curr = nullptr;
        }else if (curr->right != nullptr) {//if the node have a right node
            curr = curr->right->leftMost();//go to the leftMost of the right tree
        }else {
            node *next = curr;
            while (next->parent != nullptr && next == next->parent->right) {//if the curr is now at the right
                next = next->parent;//go back to its parent
            }
            if (nullptr == next->parent) {
                curr = nullptr;
            }else {//if the node is now on the left
                curr = next->parent;
            }
        }
        return *this;
    }

    template<typename T, typename compare_type>
    typename bst<T, compare_type>::iterator &bst<T, compare_type>::iterator::operator--() {
        if (this->curr == container->begin()) {//reach the end can not go further
            throw std::logic_error("Out of scoop");
        }else if (this->curr == nullptr) {//at the end of the tree
            curr = container->root->rightMost();//go to the end of the tree
        }else if (this->curr->left != nullptr) {//if there is left children
            curr = curr->left->rightMost();//go to the biggest one on the left tree
        }else {
            if (curr == curr->parent->right) {//if the node is on the right
                curr = curr->parent;//go to its parent
            }else {//not a right child
                while (curr->parent != nullptr && curr == curr->parent->left) {
                    curr = curr->parent;
                }
                if (curr->parent == nullptr) {
                    curr = nullptr;
                }else {//if it is a right child
                    curr = curr->parent;
                }
            }
        }
        return *this;
    }

    template<typename T, typename compare_type>
    typename bst<T, compare_type>::iterator bst<T, compare_type>::iterator::operator++(int) {
        iterator &copy(*this);
        ++(*this);
        return copy;
    }

    template<typename T, typename compare_type>
    typename bst<T, compare_type>::iterator bst<T, compare_type>::iterator::operator--(int) {
        iterator &copy(*this);
        --(*this);
        return copy;
    }

    template<typename T, typename compare_type>
    const T &bst<T, compare_type>::iterator::operator*() const {
        return curr->value;
    }

    template<typename T, typename compare_type>
    const T *bst<T, compare_type>::iterator::operator->() const {
        return &(curr->value);
    }


}
#endif //HW2_BST_H
