#ifndef MOCK_HPP_
#define MOCK_HPP_


#include "pp_macros.h"
#include "MockScope.hpp"
#include <functional>
#include <type_traits>
#include <tuple>


template<typename F>
struct FunctionTraits;

template<typename R, typename... A>
struct FunctionTraits<R(*)(A...)> {
    using StdFunctionType = std::function<R(A...)>;
    using ReturnType = R;

    template<int N>
    struct Arg {
        using Type = std::remove_reference_t<decltype(std::get<N>(std::tuple<A...>()))>;
    };
};


/**
 Temporarily replace func with the passed-in lambda:
 e.g.
 auto mock = MOCK(send, [](auto...) { return -1; })
 This causes every call to `send` in the production code to
 return -1 no matter what
 */
#define MOCK(func, lambda) mockScope(mock_##func, lambda)

/**
 Declares a mock function. The parameters are the function name and
 the types of its parameters. This is simply the declaration, the
 implementation is done with IMPL_MOCK. e.g.
 int foo(int, float);
 extern std::function<int(int, float)> mock_foo;
 */
#define DECL_MOCK(func) extern FunctionTraits<decltype(&func)>::StdFunctionType mock_##func

/**
 Definition of the std::function that will store the implementation.
 Defaults to the "real" function. e.g.
 int foo(int, float);
 std::function<int(int, float)> mock_foo = foo;
 */
#define MOCK_STORAGE(func) decltype(mock_##func) mock_##func = func

/**
 A name for the ut_ function argument at position index
 */
#define UT_ARG(index) arg_##index

/**
 Type and name for ut_ function argument at position index
 */
#define UT_TYPE_AND_ARG(func, index) FunctionTraits<decltype(&func)>::Arg<index>::Type UT_ARG(index)


#define UT_ARGS_0(func)
#define UT_FWD_0

#define UT_ARGS_1(func) UT_ARGS_0(func) UT_TYPE_AND_ARG(func, 0)
#define UT_FWD_1 UT_FWD_0 UT_ARG(0)

#define UT_ARGS_2(func) UT_ARGS_1(func), UT_TYPE_AND_ARG(func, 1)
#define UT_FWD_2 UT_FWD_1, UT_ARG(1)

#define UT_ARGS_3(func) UT_ARGS_2(func), UT_TYPE_AND_ARG(func, 2)
#define UT_FWD_3 UT_FWD_2, UT_ARG(2)

#define UT_ARGS_4(func) UT_ARGS_3(func), UT_TYPE_AND_ARG(func, 3)
#define UT_FWD_4 UT_FWD_3, UT_ARG(3)


#define IMPL_MOCK(func, num_args) \
    MOCK_STORAGE(func); \
    extern "C" FunctionTraits<decltype(&func)>::ReturnType ut_##func(UT_ARGS_##num_args(func)) { \
        return mock_##func(UT_FWD_##num_args); \
    }

#define IMPL_MOCK_0(func) IMPL_MOCK(func, 0)
#define IMPL_MOCK_1(func) IMPL_MOCK(func, 1)
#define IMPL_MOCK_2(func) IMPL_MOCK(func, 2)
#define IMPL_MOCK_3(func) IMPL_MOCK(func, 3)
#define IMPL_MOCK_4(func) IMPL_MOCK(func, 4)



#endif // MOCK_HPP_
