#ifndef _APP_H_
#define _APP_H_


#include "ff.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Fcar_Api.h"

typedef enum{
    PASSED = 0,
    FAILED = 1, 
    NEWEST = 2, 
    BINFILEPREFIXERROR = 3, 
}UpdateStatus;

extern UpdateStatus UserAppErase(void);
extern UpdateStatus UserAppUpdate(uint32_t  user_app_address_offset, uint8_t *p_appdata,uint32_t appdatalen);
extern UpdateStatus UserAppLock(void);
extern UpdateStatus UserJumpToAppCheckVerion(Vehicle_Model_Info *jump_check_info);
extern UpdateStatus UserAppUpdateCheckVerion(Vehicle_Model_Info *vehicle_model_info);
extern const TCHAR gBinPath[];
extern const uint8_t gappversion[];
extern const uint8_t gbootversion[];
extern UpdateStatus UserAppUpdateViaBinFileAndCheck(void);
extern UpdateStatus UserBootVerionSave(uint8_t *p_bootversionback,uint8_t check);
extern UpdateStatus get_vehicle_info(Vehicle_Model_Info *version_info);


extern uint32_t  EcFileOpen(char  *filehname,int mode,uint32_t *handle);
extern unsigned int  EcFilePostion(uint32_t filehandle); 
extern FRESULT UserBinFileDelete(TCHAR *pbinfilename);
extern unsigned int  EcFileSeek(uint32_t filehandle,int offset,int position);
extern unsigned int  EcFileRead2(uint32_t filehandle,int length,int *readlength);
extern FRESULT UserBinFileSave(TCHAR *pbinfilename,const void* buff,UINT btw,UINT* bw);
extern void read_binfile(char *filepath);
extern FRESULT UserBinFilescan(FILINFO *filinfo);
extern unsigned int  EcFileClose(uint32_t filehandle);
extern unsigned int  EcFileDelete(char  *filehname);

#endif /* _APP_H_ */

