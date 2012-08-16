//register.hpp: unified Cortex-M3 register manipulation interface
#ifndef _REGISTER_HPP_
#define _REGISTER_HPP_

#include "common.hpp"

//Compiler directives for optimization
#pragma push
#pragma Otime

//Prefix for register access functions
//Inlining is required--function call overhead would be too much compared to the work done
#define REG_OP __forceinline static

//Comment this to disable "whois" tracing of class hierarchy
//#define REG_WHOIS

#ifdef REG_WHOIS
#include <cstdio>
using std::printf;
#endif//REG_WHOIS

//Register Interface:
//  [ref()]: returns the true reference to the register
//  x=read(): read to x
//  write(x): write x
//  set([x]): set [marked] bits
//  clear([x]): clear [marked] bits
//  toggle([x]): toggle [marked] bits

//Addr[Lsb+Width-1 : Lsb]
template <U32 Addr, U32 Lsb, U32 Width> struct Reg_Bitfield{
    typedef U32 value_type;
    enum{
        addr=Addr,
        lsb=Lsb,
        width=Width,
        msb=Lsb+Width-1,
    };
    static const U32 all=(1u<<(width-1))-1+(1u<<(width-1));	//circumvents 0xffffffff problem
    static const U32 mask=all<<Lsb;
#ifdef REG_WHOIS
    static void whois(){
        printf("Reg_Bitfield<0x%08X, %d, %d>\r\n", addr, lsb, width);
    }
#else
    static void whois(){}
#endif//REG_WHOIS
    REG_OP value_type read(){
        return (VU32_AT(addr)&mask)>>lsb;
    }
    REG_OP void write(value_type x){
        VU32_AT(addr)=VU32_AT(addr) & (~mask) | ((x<<lsb)&mask);
    }
    REG_OP void set(){
        VU32_AT(addr)|=mask;
    }
    REG_OP void set(value_type x){
        VU32_AT(addr)|=(x<<lsb)&mask;
    }
    REG_OP void clear(){
        VU32_AT(addr)&=~mask;
    }
    REG_OP void clear(value_type x){
        VU32_AT(addr)&=~((x<<lsb)&mask);
    }
    REG_OP void toggle(){
        VU32_AT(addr)^=mask;
    }
    REG_OP void toggle(value_type x){
        VU32_AT(addr)^=((x<<lsb)&mask);
    }
};

//Addr[Msb : Lsb]
template <U32 Addr, U32 Msb, U32 Lsb> struct Reg_Range : Reg_Bitfield<Addr, Lsb, Msb-Lsb+1> {};

//Byte/Half-word/Word access
template <U32 Addr, typename T> struct Reg_Whole{
    typedef T value_type;
    typedef volatile T* ptr_type;
    enum{
        addr=Addr,
        lsb=0,
        width=8*sizeof(T),
        msb=width-1,
    };
    static const U32 all=(1u<<msb)-1u+(1u<<msb); //circumvents 0xffffffff problem
    static const U32 mask=all;
#ifdef REG_WHOIS
    static void whois(){
        printf("Reg_Whole<0x%08X, U%d>\r\n", addr, width);
    }
#else
    static void whois(){}
#endif//REG_WHOIS
    REG_OP value_type& ref(){
        return *(ptr_type)(addr);
    }
    REG_OP value_type read(){
        return *(ptr_type)(addr);
    }
    REG_OP void write(value_type x){
        *(ptr_type)(addr)=x;
    }
    REG_OP void set(){
        *(ptr_type)(addr)=all;
    }
    REG_OP void set(value_type x){
        *(ptr_type)(addr)|=x;
    }
    REG_OP void clear(){
        *(ptr_type)(addr)=0;
    }
    REG_OP void clear(value_type x){
        *(ptr_type)(addr)&=~x;
    }
    REG_OP void toggle(){
        *(ptr_type)(addr)^=mask;
    }
    REG_OP void toggle(value_type x){
        *(ptr_type)(addr)^=x;
    }
};

template <U32 Addr> struct Reg_Bitfield<Addr, 0,  8> : Reg_Whole<Addr+0, U8>{};
template <U32 Addr> struct Reg_Bitfield<Addr, 8,  8> : Reg_Whole<Addr+1, U8>{};
template <U32 Addr> struct Reg_Bitfield<Addr, 16, 8> : Reg_Whole<Addr+2, U8>{};
template <U32 Addr> struct Reg_Bitfield<Addr, 24, 8> : Reg_Whole<Addr+3, U8>{};
template <U32 Addr> struct Reg_Bitfield<Addr, 0,  16> : Reg_Whole<Addr+0, U16>{};
template <U32 Addr> struct Reg_Bitfield<Addr, 16, 16> : Reg_Whole<Addr+2, U16>{};
template <U32 Addr> struct Reg_Bitfield<Addr, 0,  32> : Reg_Whole<Addr,   U32>{};

//Addr[Bit]
template <U32 Addr, U32 Bit, bool Use_bitband> struct Reg_Bit_impl;
template <U32 Addr, U32 Bit> struct Reg_Bit : Reg_Bit_impl<Addr, Bit, SBIT_COND(Addr)>{};
template <U32 Addr, U32 Lsb> struct Reg_Bitfield<Addr, Lsb, 1> : Reg_Bit<Addr, Lsb>{};

//Case false: Normal register
template <U32 Addr, U32 Bit> struct Reg_Bit_impl<Addr, Bit, false>{
    typedef bool value_type;
    enum{
        addr=Addr,
        bit=Bit,
    };
    static const U32 all=1;
    static const U32 mask=all<<Bit;
#ifdef REG_WHOIS
    static void whois(){
        printf("Reg_Bit_impl<0x%08X, %d, false>\r\n", addr, bit);
    }
#else
    static void whois(){}
#endif//REG_WHOIS
    REG_OP value_type read(){
        return !!(VU32_AT(addr)&mask);
    }
    REG_OP void write(value_type x){
        if(x) set(); else clear();
    }
    REG_OP void set(){
        VU32_AT(addr)|=mask;
    }
    REG_OP void set(value_type x){
        VU32_AT(addr)|=x<<bit;
    }
    REG_OP void clear(){
        VU32_AT(addr)&=~mask;
    }
    REG_OP void clear(value_type x){
        VU32_AT(addr)&=~(x<<bit);
    }
    REG_OP void toggle(){
        VU32_AT(addr)^=mask;
    }
    REG_OP void toggle(value_type x){
        VU32_AT(addr)^=x<<bit;
    }
};
//Case true: Use bitband access
template <U32 Addr, U32 Bit> struct Reg_Bit_impl<Addr, Bit, true>{
    typedef bool value_type;
    enum{
        addr=Addr,
        bit=Bit,
        pbit=PBIT(Addr, Bit),
    };
    static const U32 all=1;
    static const U32 mask=all<<Bit;
#ifdef REG_WHOIS
    static void whois(){
        printf("Reg_Bit_impl<0x%08X, %d, true>\r\n", addr, bit);
    }
#else
    static void whois(){}
#endif//REG_WHOIS
    REG_OP VU32& ref(){
        return VU32_AT(pbit);
    }
    REG_OP value_type read(){
        return VU32_AT(pbit);
    }
    REG_OP void write(value_type x){
        VU32_AT(pbit)=x;
    }
    REG_OP void set(){
        VU32_AT(pbit)=1;
    }
    REG_OP void set(value_type x){
        if(x) set();
    }
    REG_OP void clear(){
        VU32_AT(pbit)=0;
    }
    REG_OP void clear(value_type x){
        if(x) clear();
    }
    REG_OP void toggle(){
        VU32_AT(pbit)=!VU32_AT(pbit);
    }
    REG_OP void toggle(value_type x){
        if(x) toggle();
    }
};

//Add operators
struct Reg_Optr{ template <typename R> struct apply{
    struct type : R{
        typedef typename R::value_type value_type;
#ifdef REG_WHOIS
        static void whois(){
            printf("(Optr)");
            R::whois();
        }
#else
        static void whois(){}
#endif//REG_WHOIS
        //20120217: Workaround for RVCT v4.1 build 894
        //Problem: RVCT won't recognize this particular idiom of operator=
        //Solution: Just screw it.
        /*
        __forceinline type& operator=(value_type x){
            R::write(x);
            return *this;
        }
        */
        __forceinline bool operator=(value_type x){
            R::write(x);
            return x;
        }
        __forceinline operator value_type(){
            return R::read();
        }
    };
};};

//Combine different registers into one virtual register

//Helper register types:

//NOP: Use empty implementation
struct Reg_Nop{
    typedef U32 value_type;
#ifdef REG_WHOIS
    static void whois(){
        printf("(Nop)\r\n");
    }
#else
    static void whois(){}
#endif//REG_WHOIS
    REG_OP value_type read(){return value_type();}
    REG_OP void write(value_type x){}
    REG_OP void set(){}
    REG_OP void set(value_type x){}
    REG_OP void clear(){}
    REG_OP void clear(value_type x){}
    REG_OP void toggle(){}
    REG_OP void toggle(value_type x){}
};
template <U32 Addr, U32 Lsb> struct Reg_Bitfield<Addr, Lsb, 0> : Reg_Nop {};

//Default: Use default implementation(relying on other operations)
struct Reg_Default{
    typedef U32 value_type;
#ifdef REG_WHOIS
    static void whois(){
        printf("(Default)\r\n");
    }
#else
    static void whois(){}
#endif//REG_WHOIS
};

//Null: Operation unimplemented(will generate compile error if called by accident)
struct Reg_Null{
    typedef U32 value_type;
#ifdef REG_WHOIS
    static void whois(){
        printf("(Null)\r\n");
    }
#else
    static void whois(){}
#endif//REG_WHOIS
};

//Combine(Metafunction Class)
struct Reg_Combine{
    //xxx_Sel: Select the correct implementation of operation from source register
    //Null is handled by specialization

    template <typename T, typename Read> struct Read_Sel{
        REG_OP T read(){return Read::read();}
    };
    template <typename T> struct Read_Sel<T, Reg_Null>{};

    template <typename T, typename Write> struct Write_Sel{
        REG_OP void write(T x){Write::write(x);}
    };
    template <typename T> struct Write_Sel<T, Reg_Null>{};

    template <typename T, typename Set> struct Set_Sel{
        REG_OP void set(){Set::set();}
        REG_OP void set(T x){Set::set(x);}
    };
    template <typename T> struct Set_Sel<T, Reg_Null>{};

    template <typename T, typename Clear> struct Clear_Sel{
        REG_OP void clear(){Clear::clear();}
        REG_OP void clear(T x){Clear::clear(x);}
    };
    template <typename T> struct Clear_Sel<T, Reg_Null>{};

    template <typename T, typename Toggle> struct Toggle_Sel{
        REG_OP void toggle(){Toggle::toggle();}
        REG_OP void toggle(T x){Toggle::toggle(x);}
    };
    template <typename T> struct Toggle_Sel<T, Reg_Null>{};

    template <typename T, typename Read, typename Write, typename Set, typename Clear, typename Toggle> struct apply{
        struct type : Read_Sel<T, Read>, Write_Sel<T, Write>, Set_Sel<T, Set>, Clear_Sel<T, Clear>, Toggle_Sel<T, Toggle>{
            typedef T value_type;
#ifdef REG_WHOIS
            static void whois(){
                printf("*Combine*\r\n");
                printf("--Read  : "); Read::whois();
                printf("--Write : "); Write::whois();
                printf("--Set   : "); Set::whois();
                printf("--Clear : "); Clear::whois();
                printf("--Toggle: "); Toggle::whois();
            }
#else
            static void whois(){}
#endif//REG_WHOIS
        };
    };

    //Default: Toggle
    template <typename T, typename Read, typename Write, typename Set, typename Clear>
        struct apply<T, Read, Write, Set, Clear, Reg_Default>{
            struct type : apply<T, Read, Write, Set, Clear, Reg_Null>::type{
                typedef T value_type;
#ifdef REG_WHOIS
                static void whois(){
                    printf("*Combine*\r\n");
                    printf("--Read  : "); Read::whois();
                    printf("--Write : "); Write::whois();
                    printf("--Set   : "); Set::whois();
                    printf("--Clear : "); Clear::whois();
                    printf("--Toggle: "); Reg_Default::whois();
                }
#else
                static void whois(){}
#endif//REG_WHOIS
                REG_OP void toggle(){
                    Write::write(~Read::read());
                }
                REG_OP void toggle(value_type x){
                    Write::write(Read::read()^x);
                }
            };
        };

    //Specialization: Bit operation
    template <typename Read, typename Write, typename Set, typename Clear>
        struct apply<bool, Read, Write, Set, Clear, Reg_Default>{
            struct type : apply<bool, Read, Write, Set, Clear, Reg_Null>::type{
                typedef bool value_type;
#ifdef REG_WHOIS
                static void whois(){
                    printf("*Combine(bool)*\r\n");
                    printf("--Read  : "); Read::whois();
                    printf("--Write : "); Write::whois();
                    printf("--Set   : "); Set::whois();
                    printf("--Clear : "); Clear::whois();
                    printf("--Toggle: "); Reg_Default::whois();
                }
#else
                static void whois(){}
#endif//REG_WHOIS
                REG_OP void toggle(){
                    Write::write(!Read::read());
                }
                REG_OP void toggle(value_type x){
                    if(x) toggle();
                }
            };
        };

    //Default: Set, Clear and Toggle
    //NOTE: Toggle uses above default implementation
    template <typename T, typename Read, typename Write>
        struct apply<T, Read, Write, Reg_Default, Reg_Default, Reg_Default>{
            struct type : apply<T, Read, Write, Reg_Null, Reg_Null, Reg_Default>::type{
                typedef T value_type;
#ifdef REG_WHOIS
                static void whois(){
                    printf("*Combine*\r\n");
                    printf("--Read  : "); Read::whois();
                    printf("--Write : "); Write::whois();
                    printf("--Set   : "); Reg_Default::whois();
                    printf("--Clear : "); Reg_Default::whois();
                    printf("--Toggle: "); Reg_Default::whois();
                }
#else
                static void whois(){}
#endif//REG_WHOIS
                REG_OP void set(){
                    Write::write(Write::all);
                }
                REG_OP void set(value_type x){
                    Write::write(Read::read()|x);
                }
                REG_OP void clear(){
                    Write::write(0);
                }
                REG_OP void clear(value_type x){
                    Write::write(Read::read()&(~x));
                }
            };
        };

    //Specialization: Bit operation
    template <typename Read, typename Write>
        struct apply<bool, Read, Write, Reg_Default, Reg_Default, Reg_Default>{
            struct type : apply<bool, Read, Write, Reg_Null, Reg_Null, Reg_Default>::type{
                typedef bool value_type;
#ifdef REG_WHOIS
                static void whois(){
                    printf("*Combine(bool)*\r\n");
                    printf("--Read  : "); Read::whois();
                    printf("--Write : "); Write::whois();
                    printf("--Set   : "); Reg_Default::whois();
                    printf("--Clear : "); Reg_Default::whois();
                    printf("--Toggle: "); Reg_Default::whois();
                }
#else
                static void whois(){}
#endif//REG_WHOIS
                REG_OP void set(){
                    Write::write(1);
                }
                REG_OP void set(value_type x){
                    if(x) set();
                }
                REG_OP void clear(){
                    Write::write(0);
                }
                REG_OP void clear(value_type x){
                    if(x) clear();
                }
            };
        };

    //Default: Write and Toggle
    template <typename T, typename Read, typename Set, typename Clear>
        struct apply<T, Read, Reg_Default, Set, Clear, Reg_Default>{
            struct type : apply<T, Read, Reg_Null, Set, Clear, Reg_Null>{
                typedef T value_type;
#ifdef REG_WHOIS
                static void whois(){
                    printf("*Combine*\r\n");
                    printf("--Read  : "); Read::whois();
                    printf("--Write : "); Reg_Default::whois();
                    printf("--Set   : "); Set::whois();
                    printf("--Clear : "); Clear::whois();
                    printf("--Toggle: "); Reg_Default::whois();
                }
#else
                static void whois(){}
#endif//REG_WHOIS
                REG_OP void write(value_type x){
                    Set::set(x); Clear::clear(~x);
                }
                REG_OP void toggle(){
                    write(~Read::read());
                }
                REG_OP void toggle(value_type x){
                    write(Read::read()^x);
                }
            };
        };

    //Specialization: Bit operation
    template <typename Read, typename Set, typename Clear>
        struct apply<bool, Read, Reg_Default, Set, Clear, Reg_Default>{
            struct type : apply<bool, Read, Reg_Null, Set, Clear, Reg_Null>::type{
                typedef bool value_type;
#ifdef REG_WHOIS
                static void whois(){
                    printf("*Combine(bool)*\r\n");
                    printf("--Read  : "); Read::whois();
                    printf("--Write : "); Reg_Default::whois();
                    printf("--Set   : "); Set::whois();
                    printf("--Clear : "); Clear::whois();
                    printf("--Toggle: "); Reg_Default::whois();
                }
#else
                static void whois(){}
#endif//REG_WHOIS
                REG_OP void write(value_type x){
                    if(x) Set::set(); else Clear::clear();
                }
                REG_OP void toggle(){
                    if(Read::read()) Clear::clear(); else Set::set();
                }
                REG_OP void toggle(value_type x){
                    if(x) toggle();
                }
            };
        };

};//struct Reg_Combine

//Shortcut macros for register combining
#define Reg_Comb_Base(...) APPLY(Reg_Optr, typename APPLY(Reg_Combine, __VA_ARGS__) )
#define Reg_Comb(...) typename Reg_Comb_Base(__VA_ARGS__)

//Register Access Modifier

//Read/Write
struct Reg_RW{ template <typename R> struct apply{
    struct type : R{
        typedef typename R::value_type value_type;
#ifdef REG_WHOIS
        static void whois(){
            printf("(RW)");
            R::whois();
        }
#else
        static void whois(){}
#endif//REG_WHOIS
    };
};};

//Read Only: Null for write
struct Reg_RO{ template <typename R> struct apply{
    struct type : Reg_Comb_Base(typename R::value_type, R, Reg_Null, Reg_Null, Reg_Null, Reg_Null){
        typedef typename R::value_type value_type;
#ifdef REG_WHOIS
        static void whois(){
            printf("(RO)");
            R::whois();
        }
#else
        static void whois(){}
#endif//REG_WHOIS
    };
};};

//Write Only: reading behavior UNDEFINED--still makes a read anyway
struct Reg_WO{ template <typename R> struct apply{
    struct type : R{
        typedef typename R::value_type value_type;
#ifdef REG_WHOIS
        static void whois(){
            printf("(WO)");
            R::whois();
        }
#else
        static void whois(){}
#endif//REG_WHOIS
    };
};};

//FIXME: RC_Wx does not work on bits located on memory regions where bitbanding is disabled.
//Read/Clear by writing 1
struct Reg_RC_W1{ template <typename R> struct apply{
    struct type : Reg_Comb_Base(typename R::value_type, R, Reg_Null, Reg_Null, Reg_Null, Reg_Null){
        typedef typename R::value_type value_type;
#ifdef REG_WHOIS
        static void whois(){
            printf("(RC_W1)");
            R::whois();
        }
#else
        static void whois(){}
#endif//REG_WHOIS
        REG_OP void clear(){
            R::write(R::all);
        }
        REG_OP void clear(value_type x){
            R::write(x);
        }
    };
};};

//Read/Clear by writing 0
struct Reg_RC_W0{ template <typename R> struct apply{
    struct type : Reg_Comb_Base(typename R::value_type, R, Reg_Null, Reg_Null, R, Reg_Null){
        typedef typename R::value_type value_type;
#ifdef REG_WHOIS
        static void whois(){
            printf("(RC_W0)");
            R::whois();
        }
#else
        static void whois(){}
#endif//REG_WHOIS
        REG_OP void clear(){
            R::write(0);
        }
        REG_OP void clear(value_type x){
            R::write(~x);
        }
    };
};};

//Read/Clear by reading
//NOTE: Works only on whole register/bitband, NOT selectively on "group of bits".
struct Reg_RC_R{ template <typename R> struct apply{
    struct type : Reg_Comb_Base(typename R::value_type, R, Reg_Null, Reg_Null, Reg_Null, Reg_Null){
        typedef typename R::value_type value_type;
#ifdef REG_WHOIS
        static void whois(){
            printf("(RC_R)");
            R::whois();
        }
#else
        static void whois(){}
#endif//REG_WHOIS
        REG_OP void clear(){
            R::read();
        }
        //REG_OP void clear(value_type x) = delete;
    };
};};

//Read/Set
struct Reg_RS{ template <typename R> struct apply{
    struct type : Reg_Comb_Base(typename R::value_type, R, Reg_Null, R, Reg_Null, Reg_Null){
        typedef typename R::value_type value_type;
#ifdef REG_WHOIS
        static void whois(){
            printf("(RS)");
            R::whois();
        }
#else
        static void whois(){}
#endif//REG_WHOIS
    };
};};

//Shortcut macros for common register definitions
#define Reg_Base(Modifier, Name, ...) APPLY(Reg_Optr, typename APPLY(Modifier, Name< __VA_ARGS__ >))
#define Reg(Modifier, Name, ...) typename Reg_Base(Modifier, Name,  __VA_ARGS__)

#define Reg_Norm(...) Reg(Reg_RW, Reg_Bitfield, __VA_ARGS__)


#undef REG_OP

#pragma pop

#endif//_REGISTER_HPP_
