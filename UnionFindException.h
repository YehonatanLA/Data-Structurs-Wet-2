//
// Created by alonb on 06/01/2022.
//

#ifndef WET2_UNIONFINDEXCEPTION_H
#define WET2_UNIONFINDEXCEPTION_H

#include <exception>

#define UNION_FIND_EXCEPTION_DEFINE_H_FILE(exception_class_name) \
class exception_class_name : public union_find::UnionFindException{\
public:\
const char* what() const noexcept override;\
};


namespace union_find {
    class UnionFindException : public std::exception {
    protected:
    public:const char* what() const noexcept override = 0;
    };

    UNION_FIND_EXCEPTION_DEFINE_H_FILE(out_of_range)
    UNION_FIND_EXCEPTION_DEFINE_H_FILE(already_unioned)

}

#undef UNION_FIND_EXCEPTION_DEFINE_H_FILE

#endif //WET2_UNIONFINDEXCEPTION_H
