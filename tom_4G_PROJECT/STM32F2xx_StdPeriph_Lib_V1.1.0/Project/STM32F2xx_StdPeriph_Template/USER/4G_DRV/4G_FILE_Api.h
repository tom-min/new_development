#ifndef _2G_FILE_API_
#define _2G_FILE_API_


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
char  * gefiletstrinfo(char *src,char  *src_start,char  *sep_start,char  *sep_end,char  *dst);
extern uint32_t  EcFileOpen(char  *filehname,int mode,uint32_t *handle); //AT+QFOPEN="RAM:1.bin",0
extern uint32_t  EcFileQFLDS(char  *filehname,int mode); 
extern uint32_t  EcFileSeek(uint32_t filehandle,int offset,int position) ;
			
extern uint32_t EcFileRead(uint32_t filehandle,int length,int *readlength) ;

extern uint32_t EcFileDelete(uint32_t filehandle) ;

extern uint32_t EcFilePostion(uint32_t filehandle) ;

extern uint32_t EcFileClose(uint32_t filehandle) ;
extern uint32_t  EcFileRead2(uint32_t filehandle,int length,int *readlength);
#endif
#endif

