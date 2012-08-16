//helper/token_paste
//Prerequisite: none
#ifndef _HELPER_TOKEN_PASTE_HPP_
#define _HELPER_TOKEN_PASTE_HPP_

#include "helper/conf.hpp"
#include "helper/def.hpp"




#ifdef BOOST_JOIN

#define TOKEN_PASTE(A, B) BOOST_JOIN(A, B)

#else

#define TOKEN_PASTE_2(A, B) A##B
#define TOKEN_PASTE_1(A, B) TOKEN_PASTE_2(A, B)
#define TOKEN_PASTE(A, B) TOKEN_PASTE_1(A, B)

#endif//BOOST_JOIN



#endif /* TOKEN_PASTE_HPP_ */
