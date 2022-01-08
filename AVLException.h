//
// Created by alonb on 11/12/2021.
//

#ifndef WET1_AVLEXCEPTION_H
#define WET1_AVLEXCEPTION_H

#include <exception>

#define UNION_FIND_EXCEPTION_DEFINE_H_FILE(exception_class_name) \
class exception_class_name : public binaryTree::AVLException{\
public:\
const char* what() const noexcept override;\
};

namespace binaryTree {
    class AVLException : public std::exception {
    protected:
        public:const char* what() const noexcept override = 0;
    };

    UNION_FIND_EXCEPTION_DEFINE_H_FILE(KeyNotExist)
    UNION_FIND_EXCEPTION_DEFINE_H_FILE(KeyAlreadyExist)
    UNION_FIND_EXCEPTION_DEFINE_H_FILE(EmptyTree)
    UNION_FIND_EXCEPTION_DEFINE_H_FILE(EmptySubTree)

}

#undef UNION_FIND_EXCEPTION_DEFINE_H_FILE
#endif //WET1_AVLEXCEPTION_H
