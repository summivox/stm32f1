//helper/static_assert
//Prerequisite: [helper/token_paste.hpp]
#ifndef _HELPER_STATIC_ASSERT_HPP_
#define _HELPER_STATIC_ASSERT_HPP_

#include "helper/conf.hpp"
#include "helper/def.hpp"
#include "helper/token_paste.hpp"




//Compile-time assertion: Use BOOST_STATIC_ASSERT if available. Otherwise use a similar implementation.
#ifdef BOOST_STATIC_ASSERT

#define STATIC_ASSERT(B) BOOST_STATIC_ASSERT(B)

#else//BOOST_STATIC_ASSERT

template <bool x> struct STATIC_ASSERTION_FAILURE;
template <> struct STATIC_ASSERTION_FAILURE<true>{enum{value=1};};
template <int x> struct STATIC_ASSERT_TEST{};

#define STATIC_ASSERT(B) \
    typedef STATIC_ASSERT_TEST< sizeof(STATIC_ASSERTION_FAILURE<(bool)(B)>) > \
        TOKEN_PASTE(TOKEN_PASTE(TOKEN_PASTE( \
                STATIC_ASSERT_TYPEDEF_, __COUNTER__), _ON_LINE_), __LINE__)

#endif//BOOST_STATIC_ASSERT



#endif /* _HELPER_STATIC_ASSERT_HPP_ */
