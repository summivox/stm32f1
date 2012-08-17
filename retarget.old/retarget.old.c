//retarget/retarget
#include <stdio.h>
#include <rt_misc.h>

#pragma import(__use_no_semihosting_swi)

extern void init(void);
extern void tx(char c);
extern void tx_buffered(char c);
extern void tx_flush(void);
extern char rx(void);
extern void un_rx(void);

struct __FILE {
    int handle;
    void* data;
};
FILE __stdout;
FILE __stdin;
FILE __stderr;

extern void $Super$$__rt_entry(void);
void $Sub$$__rt_entry(void)
{
    __stdout.handle=-1;
    __stdin.handle=-2;
    __stderr.handle=-3;
    init();
    $Super$$__rt_entry();
}

int fputc(int ch, FILE* f){
    if(f == &__stdout)
        return tx_buffered(ch), ch;
    if(f == &__stderr)
        return tx(ch), ch;
    return EOF;
}
void _ttywrch(int ch){
    tx(ch); //this is always unbuffered
}
int fflush (FILE* pStream){ tx_flush(); return (0); }

static int un_count=0;

int fgetc (FILE *f){
    if(f == &__stdin){
        char ret=rx();
		if(un_count) --un_count;
		else{
        	tx_buffered(ret);
			if(ret=='\r')
				tx_buffered('\n');
			tx_flush();
		}
        return ret;
    }
    return EOF;
}
void __backspace(FILE* f){
    un_rx();
	++un_count;
}

int fclose(FILE* f){ return 0; }
int ferror(FILE* f){ return EOF; }
int fseek (FILE* f, long nPos, int nMode){ return 0; }

void _sys_exit(int return_code){ while(1); }
