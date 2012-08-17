//retarget/retarget
#include <stdio.h>
#include <rt_misc.h>
#include "retarget.h"

#pragma import(__use_no_semihosting_swi)

extern int fputc_impl(int ch, void* impl);
extern int fflush_impl(void* impl);
extern int fgetc_impl(void* impl);
extern void __backspace_impl(void* impl);
extern int fclose_impl(void* impl);

FILE __stdout;
FILE __stdin;
FILE __stderr;

extern void $Super$$__rt_entry(void);
void $Sub$$__rt_entry(void)
{
    __stdout.handle=-1;
    __stdout.impl=0;

    __stdin.handle=-2;
    __stdin.impl=0;

    __stderr.handle=-3;
    __stderr.impl=0;

    $Super$$__rt_entry();
}

int fputc(int ch, FILE* f){
    return fputc_impl(ch, f->impl);
}
void _ttywrch(int ch){
    fputc_impl(ch, stderr->impl);
}
int fflush(FILE* f){
    return fflush_impl(f->impl);
}
int fgetc (FILE *f){
    return fgetc_impl(f->impl);
}
void __backspace(FILE* f){
    __backspace_impl(f->impl);
}
int fclose(FILE* f){
    return fclose_impl(f->impl);
}

int ferror(FILE* f){ return EOF; }
int fseek (FILE* f, long nPos, int nMode){ return 0; }

void _sys_exit(int return_code){ while(1); }
