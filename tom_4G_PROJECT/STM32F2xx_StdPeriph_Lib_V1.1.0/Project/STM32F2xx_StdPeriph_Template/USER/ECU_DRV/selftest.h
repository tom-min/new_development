#ifndef __selftest__
#define __selftest__
extern void TestPin(uint8 up_down, uint8 logic_0_1, uint8 *pins);
extern void TestShortCiruit(void);
extern void TestOpenCiruit(void);
extern void setPortStatus(uint8 *InstPack);
extern void getPortStatus(uint8 *InstPack);
//extern void PassThru_CAN_SELFTEST(uint8 selfmode,uint8 pin,uint32 canbaud,uint8 set_res);
extern void Drv_Test(void);
extern void Circle_test(void);
extern void up_down_test(void);
extern void Cmp_Test(void);
extern void test_kl_can(void);
extern void test_pulse(void);
extern void kl_setup(uint8 * buf);
extern uint8 pulse_kl_test_time(uint8 ByteNum,uint8 (*GetLogic)(void));
extern void nissan_pulse_test(uint8 * buf);
extern void kl_pulse(void);
extern void adjust_pulse_width(void);
extern void GetP0status(void);
extern void SetTxPort(uint8 *buf);
extern void drv_vcc_test(void);
void TestOpen_iobd(void);
#endif
