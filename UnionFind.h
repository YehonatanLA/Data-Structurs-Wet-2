//
// Created by alonb on 02/01/2022.
//

#ifndef WET2_UNIONFIND_H
#define WET2_UNIONFIND_H

#include <ostream>
#include <string>
#include "AVLTree.h"
#include "UnionFindException.h"


typedef int T; //TODO template

template<class U> //TODO LevelTree - Lachman
class LevelTree: public binaryTree::AVLTree<U>{
    int a{};
    U b{};
};



namespace union_find {



    // Todo template<class T>
    class UnionFind {
        int size;
        int *parent; // parent[i] is the parent of i
        int *groupSize; // groupSize[i] is EmptyGroup, or if i is a root, the size of his group.
        LevelTree<T> **data;


    public:

        explicit UnionFind(int k);
        ~UnionFind() = default;

        void makeSet(int index, T value);
        int find(int index);
        void unionSet(int i, int j);
        bool isRoot(int index) const;


        friend std::ostream &operator<<(std::ostream &os, const UnionFind &find);

    private:

        bool inRange(int i) const;
        static const int NoParent = -1;
        static const int EmptyGroup = -1;
        static const int NotRoot = -2;
    };

    UnionFind::UnionFind(int k): size(k) {
        parent = new int[size];
        groupSize = new (std::nothrow) int[size];
        data = new (std::nothrow) LevelTree<T>*[size];
        if(!groupSize) throw std::bad_alloc(); //valgrind
        if(!data) {delete groupSize; throw std::bad_alloc();} //valgrind
        for (int i = 0; i < size ; ++i) {
            parent[i] = NoParent;
            groupSize[i] = EmptyGroup;
            data[i] = nullptr;
        }
    }

    void UnionFind::makeSet(int index, T value) {
        if (!inRange(index))
            throw out_of_range();
        parent[index] = NoParent;
        groupSize[index] = 1;
        data[index] = new LevelTree<T>();
        data[index]->add(index, value);
    }

    int UnionFind::find(int index) { //recursion
        if(!inRange(index))
            throw out_of_range();
        if(isRoot(index))
            return index;
        int root = find(parent[index]);
        parent[index] = root; // optimization?
        return root;
    }

    void UnionFind::unionSet(int i, int j) {
        if(!inRange(i) || !inRange(j))
            throw out_of_range();
        int root_i = find(i), root_j = find(j);
        int size_i = groupSize[root_i], size_j = groupSize[root_j];
        if(root_i == root_j)
            throw already_unioned();
        int minRoot = size_i <= size_j ? root_i : root_j;
        int maxRoot = size_i >  size_j ? root_i : root_j;
        parent[minRoot] = maxRoot;
        groupSize[maxRoot] += groupSize [minRoot];
        groupSize[minRoot] = NotRoot;
        LevelTree<T>* merged_tree = (LevelTree<T>*) LevelTree<T>::merge(*data[maxRoot], *data[minRoot]);
        delete data[minRoot];
        delete data[maxRoot];
        data[maxRoot] = merged_tree;
    }

    bool UnionFind::isRoot(int index) const{
        return parent[index] == NoParent;
    }

    bool UnionFind::inRange(int i) const {
        return (0<= i && i <= size);
    }

    std::ostream &operator<<(std::ostream &os, const UnionFind &union_find) {
        os << "size: " << union_find.size << std::endl;
        os << "i: " << "\tparent:" << "\tgroupSize:" << "\tdata:" << std::endl;
        for (int i = 0; i < union_find.size; ++i) {
            os << i;
            os << "\t";
            if(union_find.parent[i] == UnionFind::NoParent)
                os << "NoParent";
            else
                os << std::to_string(union_find.parent[i]);
            os << "\t";
            if(union_find.groupSize[i] == UnionFind::EmptyGroup)
                os << "EmptyGroup";
            else
                os << union_find.groupSize[i];
            os << "\t";
            if(union_find.data[i] == nullptr)
                os << "NoTree";
            else
                os << union_find.data[i]->size();
            os << std::endl;
        }
        return os;
    }
}

#endif //WET2_UNIONFIND_H
