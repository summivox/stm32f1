// vim: ft=cpp
//<<< Use Configuration Wizard in Context Menu >>>

//  <o> Retarget to <1=>USART1 <2=>USART2 <3=>USART3
#define USART_N 1

//  <h> USART Remapping(chosen only)
#if USART_N==1

//      <o> USART1
//          <0=> 0: TX=PA9 ; RX=PA10;
//          <1=> 1: TX=PB6 ; RX=PB7 ;
#   define USART_REMAP 0

#elif USART_N==2

//      <o> USART2
//          <0=> 0: TX=PA2 ; RX=PA3 ;
//          <1=> 1: TX=PD5 ; RX=PD6 ;
#   define USART_REMAP 0

#elif USART_N==3

//      <o> USART3
//          <0=> 00: TX=PB10; RX=PB11;
//          <1=> 01: TX=PC10; RX=PC11;
//          <3=> 11: TX=PD8 ; RX=PD9 ;
#   define USART_REMAP 0

#endif
//  </h>


//  <h> USART Protocol Configuration

//      <o> Baud Rate <0-72000000>
#define USART_BAUD 115200

//      <o> Word length <0=>8 bits(standard) <1=>9 bits
#define USART_M 0

//      <e> Parity bit(standard:no parity)
#define USART_PARITY 0
#if USART_PARITY==1

//          <o> Parity type <0=>Even <1=>Odd
#   define USART_PARITY_TYPE 1

#endif
//      </e>

//      <o> Stop bit <0=>1(standard) <1=>0.5 <2=>2 <3=>1.5
#define USART_STOP 0

//  </h>



//  <h> Standard Input
//      <o> Buffer Size (in Bytes) <16-32768:4>
//  </h>
#define RX_BUF_SIZE 32

//  <h> Standard Output (Line Buffered)
//      <o> Line Buffer Size (in Bytes) <16-32768:4>
//  </h>
#define TX_BUF_SIZE 32

//<<< end of configuration section >>>


//Select the correct peripherals according to configuration

#if USART_N==1

#   define USART USART1
#   if USART_REMAP==0
#       define USART_TX_PORT PA
#       define USART_TX_PIN PA9
#       define USART_RX_PORT PA
#       define USART_RX_PIN PA10
#   elif USART_REMAP==1
#       define USART_TX_PORT PB
#       define USART_TX_PIN PB6
#       define USART_RX_PORT PB
#       define USART_RX_PIN PB7
#   endif

//TODO: DMA on/off
#   define DMA_TX DMA1<4>
#   define DMA_RX DMA1<5>

#elif USART_N==2

#   define USART USART2
#   if USART_REMAP==0
#       define USART_TX_PORT PA
#       define USART_TX_PIN PA2
#       define USART_RX_PORT PA
#       define USART_RX_PIN PA3
#   elif USART_REMAP==1
#       define USART_TX_PORT PD
#       define USART_TX_PIN PD5
#       define USART_RX_PORT PD
#       define USART_RX_PIN PD6
#   endif

#   define DMA_TX DMA1<7>
#   define DMA_RX DMA1<6>

#elif USART_N==3

#   define USART USART3
#   if USART_REMAP==0
#       define USART_TX_PORT PB
#       define USART_TX_PIN PB10
#       define USART_RX_PORT PB
#       define USART_RX_PIN PB11
#   elif USART_REMAP==1
#       define USART_TX_PORT PC
#       define USART_TX_PIN PC10
#       define USART_RX_PORT PC
#       define USART_RX_PIN PC11
#   elif USART_REMAP==3
#       define USART_TX_PORT PD
#       define USART_TX_PIN PD8
#       define USART_RX_PORT PD
#       define USART_RX_PIN PD9
#   endif

#   define DMA_TX DMA1<2>
#   define DMA_RX DMA1<3>

#endif
