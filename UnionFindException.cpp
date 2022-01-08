//
// Created by alonb on 06/01/2022.
//

#include "UnionFindException.h"


#define UNION_FIND_EXCEPTION_DEFINE_CPP_FILE(exception_class_name) \
const char *union_find::exception_class_name::what() const noexcept  {\
return "An Union Find related error has occurred:" #exception_class_name;\
};


namespace union_find{
    UNION_FIND_EXCEPTION_DEFINE_CPP_FILE(out_of_range)
    UNION_FIND_EXCEPTION_DEFINE_CPP_FILE(already_unioned)

}

#undef UNION_FIND_EXCEPTION_DEFINE_CPP_FILE
