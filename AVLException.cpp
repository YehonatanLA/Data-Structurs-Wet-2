//
// Created by alonb on 11/12/2021.
//

#include "AVLException.h"

#define AVL_EXCEPTION_DEFINE_CPP_FILE(exception_class_name) \
const char *binaryTree::exception_class_name::what() const noexcept  {\
return "An AVL Tree related error has occurred:" #exception_class_name;\
}

namespace binaryTree {
    AVL_EXCEPTION_DEFINE_CPP_FILE(KeyNotExist)
    AVL_EXCEPTION_DEFINE_CPP_FILE(KeyAlreadyExist)
    AVL_EXCEPTION_DEFINE_CPP_FILE(EmptyTree)
    AVL_EXCEPTION_DEFINE_CPP_FILE(EmptySubTree)

}

#undef AVL_EXCEPTION_DEFINE_CPP_FILE
