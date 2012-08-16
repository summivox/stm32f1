//misc
#ifndef _MISC_HPP_
#define _MISC_HPP_




////armcc related

//OFFSETOF() macro: offsetof() that can be recognized by Eclipse CDT
#define OFFSETOF(t, memb) ((U32)(&(((t*)0)->memb)))

//Customized runtime assertion: ARMCC's CRT won't report anything valueable
#define PRINT_ERROR(...) fprintf(stderr, __VA_ARGS__)
#define DIEDIEDIE() do{ \
    __disable_irq(); \
    NVIC_SystemReset(); \
    while(1); \
}while(false)

#ifndef NDEBUG
#define assert(x) if(!(x)){ \
    PRINT_ERROR( \
        "\r\n--ASSERTION FAILURE--\r\nFile: %s\r\nLine: %d\r\nCond: %s\r\n", \
        __FILE__, __LINE__, #x \
    ); \
    DIEDIEDIE(); \
}
#else //NDEBUG
#define assert(x) if(!(x)) DIEDIEDIE();
#endif//NDEBUG

//Shortcut for placing code in SRAM (used in conjunction with linker script)
//Switch off for better debugging
//Usage: append to function declaration
#ifndef NDEBUG
#define IN_RAM /*NOP*/
#else //NDEBUG
#define IN_RAM __attribute__((section ("RAMCODE")))
#endif//NDEBUG

//Shortcut for disabling function inlining
//NOTE: 2012-07-05 "Flash, SRAM, inline, Axis::update(), I-Code/D-Code bus malfunction" workaround
//Usage: append to function declaration
#define NO_INLINE __attribute__((noinline))

//Shortcut for making function autorun before main()
//Usage: append to function declaration
#define INIT_FUNC __attribute__((constructor))




////Cortex-M3 related

//Shortcut for declaring ISR and specifying interrupt priority
//Place in source file once only--ISRs should NEVER be called from thread code
//Usage: IRQ_DECL(TIM2_IRQn, TIM2_IRQHandler, 2, 0) IN_RAM;
void IRQ_priority_init(IRQn_Type IRQn, U8 P1, U8 P2);
#define IRQ_DECL(IRQn, IRQHandler, P1, P2) \
    void IRQn##_priority_init() __attribute__((weak, constructor)); \
    void IRQn##_priority_init(){ \
        IRQ_priority_init(IRQn, P1, P2); \
    } \
    extern "C" void IRQHandler()




////STM32F1 related

//Shortcut to peripheral bitband
//Usage: PERIPH_*BIT(I2C, I2C1, CR1, PE)

#define PERIPH_PBIT(type, periph, reg, bit) \
	PBIT( \
		(periph##_BASE)+OFFSETOF(type##_TypeDef, reg), \
		LOWBITn(type##_##reg##_##bit) \
	)

#define PERIPH_SBIT(type, periph, reg, bit) \
	SBIT( \
		(periph##_BASE)+OFFSETOF(type##_TypeDef, reg), \
		LOWBITn(type##_##reg##_##bit) \
	)

//Shortcut to peripheral clock control bit
//NOTE: operates on compile-time constant bitfield alias address
//Usage: RCC_ENR(APB2, IOPAEN)=1
#define RCC_xR(bus, periph, x) \
	PERIPH_SBIT(RCC, RCC, bus##x, periph)

#define RCC_ENR(bus, periph) RCC_xR(bus, periph, ENR)
#define RCC_RSTR(bus, periph) RCC_xR(bus, periph, RSTR)




//RTX kernel related

//in RTX operations with timeout, 0xffff means "no timeout" or "forever"
static const U16 FOREVER=0xFFFF;

//sleep for given duration, blocking
extern void wait_ms(U32 Tms);

//simple type-safe wrapper class for RTX memory pool
//roughly following the naming convention of std::allocator
template <typename T, size_t N>
struct Pool{
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T* const_pointer;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    _declare_box(pool, sizeof(T), N);
    int remaining;

    Pool(){
        init();
    }
    void init(){
        _init_box(pool, sizeof(pool), sizeof(T));
        remaining=N;
    }
    pointer allocate(){
        assert(remaining-->0);
        return (pointer)_alloc_box(pool);
    }
    void deallocate(pointer p){
        assert(!_free_box(pool, p)); //return 1 means error--just terminate
        ++remaining;
    }
};




////Data structure

//singly-linked list with length counter, dynamically allocated on heap
template <typename T>
struct Linklist{
    struct Node{
        T data;
        Node* next;
    };
    Node *first, *last;
    int size;
    Linklist():first(0), last(0), size(0){}

    void push_back(const T& data){
        Node* p=(Node*)malloc(sizeof(Node));
        assert(p);
        p->data=data;
        p->next=0;
        if(first) last=last->next=p;
        else first=last=p;
        ++size;
    }
    void push_front(const T& data){
        Node* p=(Node*)malloc(sizeof(Node));
        assert(p);
        p->data=data;
        p->next=first;
        if(!first) last=p;
        first=p;
        ++size;
    }
    void pop_front(){
        assert(first);
        Node* p=first;
        first=first->next;
        if(!first) last=0;
        free(p);
        --size;
    }
};

#endif//_MISC_HPP_
