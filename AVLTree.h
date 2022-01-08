

#ifndef WET1_AVLTREE_H
#define WET1_AVLTREE_H


#include "Node.h"
#include "AVLException.h"

namespace binaryTree {

    //using std::shared_ptr;

    // * left < this < right
    template<class T>
    class AVLTree {

        int tree_size;
        Node<T>* root;

    public:
        AVLTree();
        ~AVLTree();
        AVLTree(Node<T> **arr, int size);

        void add(int, T&);
        T remove(int key);
        T& get(int key) const;
        bool isExist(int key) const;
        bool isEmpty() const;
        static AVLTree <T>* merge(AVLTree <T> &, AVLTree <T> &);
        int size() const;
        int height() const;
        Node<T>* getRoot() const { return root;} //! DEBUG
        T& getValueForMaxKey();//TODO no ptr
        T& getValueForMinKey();//TODO no ptr
        //GGG:
        T* generateArrayFromTree(int* size = nullptr);

        void getKSmallestValues(T*, int* k, int *index);
        void getKSmallestValues_recursive(T * arr, int *k, int *index, Node<T>*);

        //!DEBUG
        void checkBalanced() const;








    private:
        void add(Node<T>*);
        Node<T>* find(int key) const;///TODO adjust , const?
        void updateHeight(Node<T>*);
        int balancingFactor(const Node<T>*) const;
        int rightHeight(const Node<T>*) const;
        int leftHeight(const Node<T>*) const;
        typedef enum rotation_type{LL, LR, RL, RR, NO_NEED} RotationType;
        void Rotation(Node<T>*, RotationType);
        RotationType calculateRotationType(Node<T>*) const;
        void destroyRecursive(Node<T>* node);
        Node<T>* getMaxNodePtrForSubTree (Node<T>* subtree_root);
        Node<T>* getMinNodePtrForSubTree (Node<T>* subtree_root);
        void replaceRemovedNode(Node<T>* node);

        //GGGG:
        void generateArrayFromTree_recursive(Node<T> **arr, Node<T> *node, int *index);
        Node<T>* generateTreeFromArray_recursive(Node<T> **arr,  int min, int max);
        Node<T>** generateArrayFromTree_private();
        static Node<T>** mergeArrays(Node<T>** arr1, Node<T>** arr2, int size1, int size2);
        void checkBalanced_rec(Node<T>*) const;
    };




    template<class T>
    AVLTree<T>::AVLTree(): tree_size(0), root(nullptr){}

    template<class T>
    AVLTree<T>::~AVLTree() { //TODO recursive Destroy
        destroyRecursive(root);
    }

    template<class T>
    bool AVLTree<T>::isEmpty() const {
        return root == nullptr;
    }

    template<class T>
    void AVLTree<T>::add(int key, T& value) {
        auto node = new Node<T>(key, value);
        add(node);
    }

    template<class T>
    void AVLTree<T>::add(Node<T>* node) {
        // *Valid Input:
        if(isEmpty()){
            root = node;
            tree_size++;
            return;
        }

        Node<T> *father = find(node->getKey());
        if (father->getKey() == node->getKey())
            throw KeyAlreadyExist();

        father->setSon(node, (father->getKey() < node->getKey()) ? Node<T>::RIGHT : Node<T>::LEFT);
        node->setFather(father);
        tree_size++;

        RotationType rotationType;
        for(Node<T>* ptr = father; ptr != nullptr; ptr = ptr->getFather()){ // * O(h) = O(log n)
            updateHeight(ptr);
            rotationType = calculateRotationType(ptr);
            if(rotationType != NO_NEED) {
                Rotation(ptr, rotationType);
                return;
            }
        }
    }

    template<class T> //TODO
    T AVLTree<T>::remove(int key) {
        if(isEmpty()){
            throw EmptyTree();
        }
        Node<T> *node = find(key);
        if(node->getKey() != key){
            throw KeyNotExist();
        }
        T value = node->getValue();
        Node<T> *father = node->getFather();
        replaceRemovedNode(node);
        tree_size--;
        for(Node<T>* ptr = father; ptr != nullptr ; ptr = ptr->getFather()) {
            updateHeight(ptr);
            rotation_type rotationType = calculateRotationType(ptr);
            Rotation(ptr, rotationType);
        }
        return value;
    }

    template<class T>
    int AVLTree<T>::size() const {
        return tree_size;
    }

    template<class T>
    int AVLTree<T>::height() const{
        return root->height;
    }

    template<class T>
    Node<T>* AVLTree<T>::find(int key) const
    {///TODO adjust
        if (isEmpty()){
            throw EmptyTree();
        }
        ///Binary Search
        Node<T> *ptr = root, *father = root;
        while(ptr != nullptr){
            father = ptr;
            if (ptr->getKey() == key){
                return ptr;
            }
            if (ptr->getKey() < key){
                ptr = ptr->getRight();
                continue;
            }
            if(ptr->getKey() > key){
                ptr = ptr->getLeft();
                continue;
            }
        }
        //If we here that means key is not found (&& ptr == nullptr).
        return father;
    }

    template<class T>
    bool AVLTree<T>::isExist(int key) const
    {
        if(isEmpty()){
            return false;
        }
        return find(key)->getKey() == key;
    }

    template<class T>
    T& AVLTree<T>::get(int key) const
    {
        Node<T>* node = find(key);
        if(node->getKey() == key){
            return node->getValue();
        }
        throw KeyNotExist();
    }

    template<class T>
    void AVLTree<T>::updateHeight(Node<T>* node) {
        int right_height = rightHeight(node);
        int left_height  = leftHeight(node);
        node->setHeight(1+ (right_height >= left_height ? right_height : left_height));
    }

    template<class T>
    void AVLTree<T>::Rotation(Node<T>* node, RotationType rotation) {

        bool is_node_root = root == node;

        Node<T>* node_father = node->getFather();
        Node<T>* node_son = (rotation == LL || rotation == LR) ? node->getLeft() : node->getRight();
        bool is_node_right_to_father = (!is_node_root) && node_father->getRight() == node; //Node<T>::R/L
        switch (rotation) {
            case RR:
                if(!is_node_root)
                    node_father->setSon(node_son, is_node_right_to_father ? Node<T>::RIGHT : Node<T>::LEFT);
                else
                    root = node_son;
                node_son->setFather(is_node_root ? nullptr : node_father);
                node->setFather(node_son);
                node->setRight(node_son->getLeft());
                if(node_son->hasLeft())
                    node_son->getLeft()->setFather(node);
                node_son->setLeft(node);
                updateHeight(node);
                updateHeight(node_son);
                break;
            case LL:
                if(!is_node_root)
                    node_father->setSon(node_son, is_node_right_to_father ? Node<T>::RIGHT : Node<T>::LEFT);
                else
                    root = node_son;
                node_son->setFather(is_node_root ? nullptr : node_father);
                node->setFather(node_son);
                node->setLeft(node_son->getRight());
                if(node_son->hasRight())
                    node_son->getRight()->setFather(node);
                node_son->setRight(node);
                updateHeight(node);
                updateHeight(node_son);
                break;
            case LR:
                Rotation(node_son, RR);
                Rotation(node, LL);
                return;
            case RL:
                Rotation(node_son, LL);
                Rotation(node, RR);
                return;
        }
    }

    template<class T>
    int AVLTree<T>::rightHeight(const Node<T>* node) const {
        return node->hasRight() ? node->getRight()->getHeight() : -1;
    }

    template<class T>
    int AVLTree<T>::leftHeight(const Node<T>* node) const {
        return node->hasLeft() ? node->getLeft()->getHeight()  : -1;
    }

    template<class T>
    int AVLTree<T>::balancingFactor(const Node<T>* node) const{
        return leftHeight(node) - rightHeight(node);
    }

    template<class T>
    typename AVLTree<T>::RotationType AVLTree<T>::calculateRotationType(Node<T>* node) const //? why typename?
    {
        switch (balancingFactor(node)) {
            case 2:
                //Left Heavy
                if (balancingFactor(node->getLeft()) > 0)
                    return LL;
                return LR;

            case -2:
                //Right Heavy
                if (balancingFactor(node->getRight()) > 0)
                    return RL;
                return RR;

            case 1:
            case -1:
            case 0:
                return NO_NEED;
            default:
                throw std::exception(); // !Exception: Big Fuck
        }
    }

    template<class T>
    void AVLTree<T>::destroyRecursive(Node<T> *node) {
        if(node == nullptr) return;
        destroyRecursive(node->getLeft());
        destroyRecursive(node->getRight());
        delete node;
    }

    template<class T>
    Node<T> *AVLTree<T>::getMaxNodePtrForSubTree(Node<T> *subtree_root) {
        if(subtree_root == nullptr){
            throw EmptySubTree();
        }
        Node<T> *ptr = subtree_root;
        while (ptr->hasRight()) ptr = ptr->getRight();
        return ptr;
    }

    template<class T>
    Node<T> *AVLTree<T>::getMinNodePtrForSubTree(Node<T> *subtree_root) {
        if(subtree_root == nullptr){
            throw EmptySubTree();
        }
        Node<T> *ptr = subtree_root;
        while (ptr->hasLeft()) ptr = ptr->getLeft();
        return ptr;
    }

    template<class T>
    T& AVLTree<T>::getValueForMaxKey() {
        return getMaxNodePtrForSubTree(root)->getValue();
    }

    template<class T>
    T& AVLTree<T>::getValueForMinKey() {
        return getMinNodePtrForSubTree(root)->getValue();
    }

    template<class T>
    void AVLTree<T>::replaceRemovedNode(Node<T> *node) {
        if(node->hasLeft() && node->hasRight()){ //TODO if root
            Node<T> *ptr = getMaxNodePtrForSubTree(node->getLeft());
            node->setValue(ptr->getValue());
            node->setKey(ptr->getKey());
            Node<T>* father = ptr->getFather();
            father->setSon(ptr->getLeft(), ptr == father->getRight() ? Node<T>::RIGHT : Node<T>::LEFT);
            if(ptr->hasLeft())
                ptr->getLeft()->setFather(father);
            ptr->setFather(nullptr);
            ptr->setLeft(nullptr);
            delete ptr;
        }

        else if(node->hasLeft() && !node->hasRight()){
            Node<T>* father = node->getFather();
            Node<T>* left_subtree = node->getLeft();
            if(node->hasFather())
                father->setSon(left_subtree, father->getRight() == node ? Node<T>::RIGHT : Node<T>::LEFT);
            else
                root = left_subtree;
            left_subtree->setFather(father);
            delete node;
        }

        else if(!node->hasLeft() && node->hasRight()){
            Node<T>* father = node->getFather();
            Node<T>* right_subtree = node->getRight();
            if(node->hasFather())
                father->setSon(right_subtree, father->getRight() == node ? Node<T>::RIGHT : Node<T>::LEFT);
            else
                root = right_subtree;
            right_subtree->setFather(father);
            delete node;
        }
        /*
         *      O
         *        \
         *         O
         *        / \
         *        ...
         *
         *
         */
        else{ //leaf
            Node<T>* father = node->getFather();
            if(node->hasFather())
                father->setSon(nullptr, father->getRight() == node ? Node<T>::RIGHT : Node<T>::LEFT);
            else
                root = nullptr;
            delete node;
        }
    }

    template<class T>
    Node<T>** AVLTree<T>::generateArrayFromTree_private() {
        auto arr = new Node<T>*[tree_size];
        int index = 0;
        generateArrayFromTree_recursive(arr, root, &index);
        return arr;
    }

    template<class T>
    void AVLTree<T>::generateArrayFromTree_recursive(Node<T> **arr, Node<T> *node, int *index) {
        if (node->hasLeft()) {
            generateArrayFromTree_recursive(arr, node->getLeft(), index);
        }

        arr[*index] = node;//? check if valgrind fails
        (*index)++;
        if (node->hasRight()) {
            generateArrayFromTree_recursive(arr, node->getRight(), index);
        }
    }

    template<class T>
    AVLTree<T>::AVLTree(Node<T> **arr, int size){
        root = generateTreeFromArray_recursive(arr, 0, size - 1);
        tree_size = size;
    }

    template<class T>
    Node<T>* AVLTree<T>::generateTreeFromArray_recursive(Node<T> **arr,  int min, int max) {
        int mid = (max + min) / 2;
        if(min == max){
            int key(arr[mid]->getKey());
            T data(arr[mid]->getValue());
            return new Node<T>(key, data);
        }
        int key(arr[mid]->getKey());
        T data(arr[mid]->getValue());
        auto node = new Node<T>(key, data);
        if(mid == min){
            node->setRight(generateTreeFromArray_recursive(arr, mid + 1, max));
            node->getRight()->setFather(node);
            updateHeight(node);
        }
        else {
            node->setLeft(generateTreeFromArray_recursive(arr, min, mid - 1));
            node->setRight(generateTreeFromArray_recursive(arr, mid + 1, max));
            node->getLeft()->setFather(node);
            node->getRight()->setFather(node);
            updateHeight(node);
        }
        return node;

    }

    template<class T>
    Node<T>** AVLTree<T>::mergeArrays(Node<T>** arr1, Node<T>** arr2, int size1, int size2){
        int i = 0, j = 0, k = 0;
        auto merged_arr = new Node<T>*[size1 + size2];
        while (i<size1 && j <size2){
            int key1 = arr1[i]->getKey(), key2 = arr2[j]->getKey();
            if (key1 < key2)
                merged_arr[k++] = arr1[i++];
            else if(key1 > key2)
                merged_arr[k++] = arr2[j++];
            else {
                throw KeyAlreadyExist();
            }
        }

        while (i < size1)
            merged_arr[k++] = arr1[i++];

        while (j < size2)
            merged_arr[k++] = arr2[j++];
        return merged_arr;

    }

    template<class T>
    AVLTree <T>* AVLTree<T>::merge(AVLTree <T> &tree1, AVLTree <T> &tree2) {
        Node<T> **arr1 = tree1.generateArrayFromTree_private(),
                **arr2 = tree2.generateArrayFromTree_private();
        int size1 = tree1.size(), size2 = tree2.size();
        Node<T> **arr_merged = mergeArrays(arr1, arr2, size1, size2);
        /*delete[] arr1; //? delete node?
        delete[] arr2; //? delete node
        delete[] arr_merged;*/

        return new AVLTree<T>(arr_merged, size1 + size2);
    }

    template<class T>
    void AVLTree<T>::getKSmallestValues(T *arr, int *k, int *index) {
        getKSmallestValues_recursive(arr, k, index, root);
    }

    template<class T>
    void AVLTree<T>::getKSmallestValues_recursive(T *arr, int *k, int *index, Node<T> *node_ptr) {
        if(node_ptr->hasLeft()){
            getKSmallestValues_recursive(arr, k, index, node_ptr->getLeft());
        }
        if(*k > *index){
            arr[(*index)] = node_ptr->getValue();
            (*index) ++;
        }
        if(*k == *index){
            return;
        }

        if(node_ptr->hasRight()){
            getKSmallestValues_recursive(arr, k, index, node_ptr->getRight());
        }

    }

    template<class T>
    T* AVLTree<T>::generateArrayFromTree(int* size) {

        Node<T>** node_arr = generateArrayFromTree_private();
        T* val_arr = new T[tree_size];
        if(size)
            *size = tree_size;
        for(int i = 0; i < tree_size; i++){
            val_arr[i] = node_arr[i]->getValue(); //copies all values to array
        }
        // * should be safe to delete, copied all values  && keys from tree && copied all values to array
        delete[] node_arr;
        return val_arr;
    }

    template<class T>
    void AVLTree<T>::checkBalanced() const{
        checkBalanced_rec(root);
    }

    template<class T>
    void AVLTree<T>::checkBalanced_rec(Node<T>* node) const{
        if(!node) return;
        if(abs(balancingFactor(node)) >=2)
            throw std::exception();
        cheakBalanced_rec(node->getLeft());
        checkBalanced_rec(node->getRight());
    }

}

#endif //WET1_AVLTREE_H


