//retarget/retarget
#include <stdio.h>
#include <rt_misc.h>
#include "retarget.h"

#pragma import(__use_no_semihosting_swi)

extern void rt_entry_impl(void);
extern int fputc_impl(int ch, void* impl);
extern int fflush_impl(void* impl);
extern int fgetc_impl(void* impl);
extern void backspace_impl(void* impl);
extern int fclose_impl(void* impl);

FILE __stdout;
FILE __stdin;
FILE __stderr;

extern void $Super$$__rt_entry(void);
void $Sub$$__rt_entry(void)
{
    //TODO: make this more generic (duh)
    __stdout.handle=-1;
    __stdout.data=(void*)0;

    __stdin.handle=-1;
    __stdin.data=(void*)1;

    __stderr.handle=-1;
    __stderr.data=(void*)2;

    rt_entry_impl();

    $Super$$__rt_entry();
}

int fputc(int ch, FILE* f){
    return fputc_impl(ch, f);
}
void _ttywrch(int ch){
    fputc_impl(ch, stderr);
}
int fflush(FILE* f){
    return fflush_impl(f);
}
int fgetc (FILE *f){
    return fgetc_impl(f);
}
void __backspace(FILE* f){
    backspace_impl(f);
}
int fclose(FILE* f){
    return fclose_impl(f);
}

int ferror(FILE* f){ return EOF; }
int fseek (FILE* f, long nPos, int nMode){ return 0; }

void _sys_exit(int return_code){ while(1); }
