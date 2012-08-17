//retarget_impl.cpp
#include "stdafx.h"
#pragma hdrstop

//Configuration file: You may replace with your own definitions.
#include "retarget.conf"

//Required definitions
#if !defined(USART) || !defined(USART_BAUD) || \
    !defined(USART_TX_PORT) || !defined(USART_TX_PIN) || \
    !defined(USART_RX_PORT) || !defined(USART_RX_PIN) || \
    !defined(DMA_TX) || !defined(DMA_RX) || \
    !defined(RX_BUF_SIZE) || !defined(TX_BUF_SIZE)
STATIC_ASSERT(false);
#endif

//Default settings: no remapping, 8 data bits, no parity, 1 stop bit
#ifndef USART_REMAP
#   define USART_REMAP 0
#endif
#ifndef USART_PARITY
#   define USART_PARITY 0
#endif
#ifndef USART_STOP
#   define USART_STOP 0
#endif

///////////////////////////////////////////////////////

//tx buffer
char tx_buf_1[TX_BUF_SIZE];
char tx_buf_2[TX_BUF_SIZE];
char *tx_curr, *tx_next, *tx_ptr, *tx_end;

//rx buffer
char rx_buf[RX_BUF_SIZE]={0};
U32 qf;

void usart_init(){
    USART_TX_PORT::enable();
    USART_TX_PIN.conf(GPIO::out_50M, GPIO::af_pushpull);

    USART_RX_PORT::enable();
    USART_RX_PIN.conf(GPIO::in, GPIO::in_floating);

	AFIO::enable();

    USART::enable();
    USART::reset();

    USART::remap=USART_REMAP;
    USART::baud_rate=USART_BAUD;
    USART::word_length=USART_M;

#if USART_PARITY==1
    USART::parity_enabled=1;
    USART::parity_type=USART_PARITY_TYPE;
#endif

    USART::stop_bit=USART_STOP;
    USART::UE=true;

    USART::TX_enabled=true;
    USART::RX_enabled=true;

    //USART::RXNE_ie=true;
    //IRQ<USART_IRQn>::enable();

    USART::wait_TC(); //wait for the initial idle frame
}

void dma_tx_init(){
    typedef DMA_TX D;
    D::enable();
    D::reset();

    D::dir=DMA::read_M_write_P;

    D::P_addr=(U32)USART::DR;
    D::P_size=DMA::byte;
    D::P_inc=false;

    D::M_size=DMA::byte;
    D::M_inc=true;
    D::circular=false;
    D::priority=0;

    USART::DMA_TX_enabled=true;

    tx_curr=tx_ptr=tx_buf_1;
    tx_next=tx_buf_2;
    tx_end=tx_ptr+TX_BUF_SIZE;
}

void dma_rx_init(){
    typedef DMA_RX D;
    D::enable();
    D::reset();

    D::dir=DMA::read_P_write_M;
    
    D::P_addr=(U32)USART::DR;
    D::P_size=DMA::byte;
    D::P_inc=false;

    D::M_addr=(U32)rx_buf;
    D::M_size=DMA::byte;
    D::M_inc=true;
    D::circular=true;
    D::priority=1;

    D::data_n=RX_BUF_SIZE;

    USART::DMA_RX_enabled=true;
    D::ch_enabled=true;

    qf=0;
}

extern "C" void init(){
	SystemInit();
    usart_init();
    dma_tx_init();
    dma_rx_init();
}

extern "C" void tx(char c){
    USART::TX(c);
}
extern "C" void tx_flush(){
    typedef DMA_TX D;

	USART::wait_TC(); //always wait for last flush to complete

	if(tx_ptr==tx_curr) return; //stop if buffer's empty

    D::ch_enabled=false;
    D::G_ic.clear();

    D::M_addr=(U32)tx_curr;
    D::data_n=tx_ptr-tx_curr;
    D::ch_enabled=true;
    tx_ptr=tx_next; tx_next=tx_curr; tx_curr=tx_ptr; //std::swap(tx_next, tx_curr);
    tx_end=tx_ptr+TX_BUF_SIZE;
}
extern "C" void tx_buffered(char c){
    *tx_ptr++=c;
    if(c=='\n' || tx_ptr==tx_end) tx_flush();
}

extern "C" char rx(){
    typedef DMA_RX D;

    while(qf==(RX_BUF_SIZE-D::data_n));
    char ret=rx_buf[qf];
    if(++qf==RX_BUF_SIZE) qf=0;
    return ret;
}
extern "C" void un_rx(){
    if(--qf==~0) qf=RX_BUF_SIZE-1;
}
