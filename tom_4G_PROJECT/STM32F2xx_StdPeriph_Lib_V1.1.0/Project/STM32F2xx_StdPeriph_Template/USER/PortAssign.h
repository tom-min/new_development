
#define MODE_DIFFERENTIAL_TX_LINE0UP          1 
#define MODE_DIFFERENTIAL_TX_LINE0DOWN        2 
#define MODE_LINE0_LINE1_TX_NORMAL            3 
#define MODE_LINE0_LINE1_TX_REVERSE           4 
#define MODE_ONLY_LINE0_TX_NORMAL             5 
#define MODE_ONLY_LINE0_TX_REVERSE            6 
#define MODE_LINE0_TX_LINE1_GND_NORMAL        7 
#define MODE_LINE0_TX_LINE1_GND_REVERSE       8 
#define MODE_ONLY_LINE1_TX_NORMAL             9 
#define MODE_ONLY_LINE1_TX_REVERSE            10
#define MODE_LINE1_TX_LINE0_GND_NORMAL        11
#define MODE_LINE1_TX_LINE0_GND_REVERSE       12
#define MODE_HIGH_RES													13

#define MODE_LINE0_LINE1_RX_VOLVO						1
#define MODE_DIFFERENTIAL_RX_LINE0UP				2
#define MODE_DIFFERENTIAL_RX_LINE0DOWN			3
#define MODE_LINE0_LINE1_RX_NORMAL					4
#define MODE_LINE0_LINE1_RX_REVERSE					5
#define MODE_LINE0_RX_VOLVO									6
#define MODE_ONLY_LINE0_RX_NORMAL						7
#define MODE_ONLY_LINE0_RX_REVERSE					8
#define MODE_LINE1_RX_VOLVO									9
#define MODE_ONLY_LINE1_RX_NORMAL						10
#define MODE_ONLY_LINE1_RX_REVERSE					11

//#define TIM1_TC TIM_GetCounter(TIM6)+(TIME_OTHER.TIM_IRQ_N<<16)//LPC_TIM1->TC
#define TIM0_TC  1//TIM_GetCounter(TIM2)//+(TIME_ONE.TIM_IRQ_N<<16)//LPC_TIM0->TC
#define TIM0_Prescaler  1//TIM_GetPrescaler(TIM2)// 分

#define TIM0_Restet_TC 1 //TIM_SetCounter(TIM2,0)//检测波特率时用
// #define TIM1_IRQ_FLAG TIM_GetITStatus(TIM6 , TIM_IT_Update)
//#define TIM0_IRQ_FLAG TIM_GetITStatus(TIM2 , TIM_IT_Update)
//*****************
#define mux0_vcc 0//ADC_Channel_0  //quwenqi
#define mux1_vcc 1//ADC_Channel_1
#define cmp_vcc  2//ADC_Channel_2
#define drv_vcc  3//ADC_Channel_3
#define bat_vcc  4//ADC_Channel_4

#define ver_vcc  5//ADC_Channel_5 virtual

//逻辑电平定义
#define LOGIC_1    1
#define LOGIC_0    0
#define PUSH_PULL_FLAG 0 //0 正常模式  1 推挽模式


//****************LINE0控制***************






/*************************发送模式控制****************
编号	TXD_MODE2	TXD_MODE1	TXD_MODE0		处理方式
1				0					0					0					LINE0发送
2				0					0					1					LINE1发送
3				0					1					0					LINE0,LINE1同时发送，通过SET_TX0_REVSERSE_HIGH控制同时正向，通过SET_TX0_REVSERSE_LOW控制同时反向
4				0					1					1					LINE0,LINE1差分发送,能过TXD_INV=1,LINE0为差发正线,LINE1为差分负线;TXD_INV=0正好相反
5				1					0					0					禁止发送数据，配合上下拉来实现高阻
6       1         0         1         LINE0发送，LINE1接地
7       1         1         0         LINE1发送，LINE0接地
8				1         1         1         line0发送，line1高阻
9                                     line1发送，line0高阻
*/
//上下拉函数设置

#if 1
#define LINE0_NO_LOAD				ChannelBuf[0]&=~0X05
#define LINE0_UP					ChannelBuf[0]&=~0X05;ChannelBuf[0]|=0X01
#define LINE0_DOWN					ChannelBuf[0]&=~0X05;ChannelBuf[0]|=0X04
#define LINE1_NO_LOAD				ChannelBuf[1]&=~0X01;ChannelBuf[0]&=~0X08
#define LINE1_UP					ChannelBuf[0]&=~0X08;ChannelBuf[1]|=0X01
#define LINE1_DOWN					ChannelBuf[1]&=~0X01;ChannelBuf[0]|=0X08
#define LINE_NO_LOAD				ChannelBuf[1]&=~0X01;ChannelBuf[0]&=~0X0D

#define clk_en						  ChannelBuf[0]|=0x10;//tx_c_channel();
#define clk_dis						  ChannelBuf[0]&=~(0x10);////tx_c_channel();
// #define can_sec_en					ChannelBuf[0]|=0x2
// #define can_sec_dis					ChannelBuf[0]&=~(0x2)
#define can_fir_en 					ChannelBuf[0]|=0x20	
#define kl_en								ChannelBuf[0]&=~(0x20);

//继电器分断
#define LINE1_0_5					ChannelBuf[7]&=~(0x80)
#define LINE1_6_12				ChannelBuf[7]|=0X80
#define LINE0_0_5					ChannelBuf[7]&=~(0x40)
#define LINE0_6_12				ChannelBuf[7]|=0X40
//********************can1线上60欧电阻控制接口
#define SET_60RES							ChannelBuf[0]|=0x40;//tx_c_channel();
#define CLR_60RES							ChannelBuf[0]&=~(0x40);//tx_c_channel();

//发送模式函数设置
#define LINE0_TX						ChannelBuf[2]&=0xF0;ChannelBuf[2]|=0x00
#define LINE1_TX ChannelBuf[2]&=0xF0;ChannelBuf[2]|=0x01
#define LINE0_LINE1_TX			ChannelBuf[2]&=0xF0;ChannelBuf[2]|=0x02
#define DIFFERENTIAL				ChannelBuf[2]&=0xF0;ChannelBuf[2]|=0x03
#define HIGH_RES_CH					ChannelBuf[2]&=0xF0;ChannelBuf[2]|=0x0f
#define SINGLE_CAN_TX       ChannelBuf[2]&=0xF0;ChannelBuf[2]|=0x05
#define HIGH_CAN_TX         ChannelBuf[2]&=0xF0;ChannelBuf[2]|=0x06
#define RS232_MODE_TX       ChannelBuf[2]&=0xF0;ChannelBuf[2]|=0x07
//#define RS485_MODE_TX       ChannelBuf[2]&=0xF0;ChannelBuf[2]|=0x08//485模式无用，从物理上j1708的做法和我们用三极管搭的电路一样，没必要重做
//发送正反转设置
#define TX_NORMAL					ChannelBuf[3]&=0x02;ChannelBuf[3]|=0x01
#define TX_REVERSE					ChannelBuf[3]&=0x02;ChannelBuf[3]|=0x00
//是否开启自发自收
#define DIS_SELF_TX_RX			ChannelBuf[3]|=0x04
#define EN_SELF_TX_RX				ChannelBuf[3]&=~(0x04)
//在设置模式时，设一引脚，使到1768往外输出时，输入的引脚为高电平，不会收到其它杂波
#define G_RXD_DIS //GPIO_SetBits(GPIOB,GPIO_Pin_15)//LPC_GPIO1->FIOSET=(1<<14); 
#define G_RXD_EN  //GPIO_ResetBits(GPIOB,GPIO_Pin_15)//LPC_GPIO1->FIOCLR=(1<<14); 
//p1.2版本中删除，不用
//#define RXD_EN				LPC_GPIO0->FIOCLR=(1<<0)//低电平允许接收,
//#define RXD_DIS				LPC_GPIO0->FIOSET=(1<<0)//高电平禁止接收,禁止串口接收，io口不禁止
																		//发送模式															发送是否正反相													//上下拉																	
#define ONLY_LINE0_TX_NORMAL 								LINE0_TX;				TX_NORMAL;				LINE_NO_LOAD;		LINE0_UP;			//tx_c_channel();	PassThruConfig.TxMode = MODE_ONLY_LINE0_TX_NORMAL;//TXD_MOD3_LOW;	TXD_MOD2_LOW;	TXD_MOD1_LOW;		TXD_MOD0_LOW;	 SET_TX0_REVSERSE_HIGH;	SetEcuLineLoadMode(LINE0, UP_LOAD)//在line0上正常发送
#define ONLY_LINE0_TX_REVERSE 							LINE0_TX;				TX_REVERSE;				LINE_NO_LOAD;		LINE0_DOWN;		//tx_c_channel();	PassThruConfig.TxMode = MODE_ONLY_LINE0_TX_REVERSE;//TXD_MOD3_LOW;	TXD_MOD2_LOW;	TXD_MOD1_LOW;		TXD_MOD0_LOW;	 SET_TX0_REVSERSE_LOW;	SetEcuLineLoadMode(LINE0, DOWN_LOAD)//line0上反转发送
#define ONLY_LINE1_TX_NORMAL 								LINE1_TX;				TX_NORMAL;				LINE_NO_LOAD;		LINE1_UP;			//tx_c_channel();	PassThruConfig.TxMode = MODE_ONLY_LINE1_TX_NORMAL;//TXD_MOD3_LOW;	TXD_MOD2_LOW;	TXD_MOD1_LOW;		TXD_MOD0_HIGH; SET_TX0_REVSERSE_HIGH;	SetEcuLineLoadMode(LINE1, UP_LOAD)//line1上正常发送
#define ONLY_LINE1_TX_REVERSE 							LINE1_TX;				TX_REVERSE;				LINE_NO_LOAD;		LINE1_DOWN;		//tx_c_channel();	PassThruConfig.TxMode = MODE_ONLY_LINE1_TX_REVERSE;//TXD_MOD3_LOW;	TXD_MOD2_LOW;	TXD_MOD1_LOW;		TXD_MOD0_HIGH; SET_TX0_REVSERSE_LOW;	SetEcuLineLoadMode(LINE0, DOWN_LOAD)//line1上反转发送
#define LINE0_LINE1_TX_NORMAL 							LINE0_LINE1_TX;	TX_NORMAL;				LINE_NO_LOAD;		LINE0_UP;			LINE1_UP;			 	PassThruConfig.TxMode = MODE_LINE0_LINE1_TX_NORMAL;//tx_c_channel();//TXD_MOD3_LOW;	TXD_MOD2_LOW;	TXD_MOD1_HIGH;	TXD_MOD0_LOW;  SET_TX0_REVSERSE_HIGH;	SetEcuLineLoadMode(LINE0, UP_LOAD)	;	SetEcuLineLoadMode(LINE1, UP_LOAD)//LINE0line1上正常发送
#define LINE0_LINE1_TX_REVERSE 							LINE0_LINE1_TX;	TX_REVERSE;				LINE_NO_LOAD;		LINE0_DOWN;		LINE1_DOWN;		 	PassThruConfig.TxMode = MODE_LINE0_LINE1_TX_REVERSE;//tx_c_channel();//TXD_MOD3_LOW;	TXD_MOD2_LOW;	TXD_MOD1_HIGH;	TXD_MOD0_LOW;  SET_TX0_REVSERSE_LOW;	SetEcuLineLoadMode(LINE0, DOWN_LOAD);	SetEcuLineLoadMode(LINE1, DOWN_LOAD)//LINE0line1上反转发送
#define DIFFERENTIAL_TX_LINE0DOWN 					DIFFERENTIAL;		TX_REVERSE;				LINE_NO_LOAD;		LINE0_DOWN;		LINE1_UP;			 	PassThruConfig.TxMode = MODE_DIFFERENTIAL_TX_LINE0DOWN;//tx_c_channel();//TXD_MOD3_LOW;	TXD_MOD2_LOW;	TXD_MOD1_HIGH;	TXD_MOD0_HIGH;SET_TX0_REVSERSE_LOW;	SetEcuLineLoadMode(LINE0, DOWN_LOAD);	SetEcuLineLoadMode(LINE1, UP_LOAD)//LINE0LINE1上差分发送，
#define DIFFERENTIAL_TX_LINE0UP							DIFFERENTIAL;		TX_NORMAL;				LINE_NO_LOAD;		LINE0_UP;			LINE1_DOWN;		 	PassThruConfig.TxMode = MODE_DIFFERENTIAL_TX_LINE0UP;//tx_c_channel();//TXD_MOD3_LOW;	TXD_MOD2_LOW;	TXD_MOD1_HIGH;	TXD_MOD0_HIGH;SET_TX0_REVSERSE_LOW;	SetEcuLineLoadMode(LINE0, DOWN_LOAD);	SetEcuLineLoadMode(LINE1, UP_LOAD)//LINE0LINE1上差分发送，
#define HIGH_RES														HIGH_RES_CH;		TX_NORMAL;				LINE_NO_LOAD;		LINE1_NO_LOAD;LINE1_NO_LOAD;//tx_c_channel();	PassThruConfig.TxMode = MODE_HIGH_RES;//TXD_MOD2_LOW; TXD_MOD2_HIGH;TXD_MOD1_LOW;		TXD_MOD0_LOW;  SET_TX0_REVSERSE_HIGH;	SetEcuLineLoadMode(LINE0, NO_LOAD);		SetEcuLineLoadMode(LINE1, NO_LOAD)//LINE0 LIN1高阻
#define LINE0_TX_LINE1_GND_NORMAL						LINE0_TX;				TX_NORMAL;				LINE_NO_LOAD;		LINE0_UP;			LINE1_UP;				PassThruConfig.TxMode = MODE_LINE0_TX_LINE1_GND_NORMAL;LINE0_OR_LINE1_LOW;	//tx_c_channel();//LINE0上正常发送,line1接地
#define LINE0_TX_LINE1_GND_REVERSE					LINE0_TX;				TX_REVERSE;				LINE_NO_LOAD;		LINE0_DOWN;		LINE1_UP;				PassThruConfig.TxMode = MODE_LINE0_TX_LINE1_GND_REVERSE;LINE0_OR_LINE1_LOW;	//tx_c_channel();//LINE0上反转发送,line1接地
#define LINE1_TX_LINE0_GND_NORMAL						LINE1_TX;				TX_NORMAL;				LINE_NO_LOAD;		LINE0_UP;			LINE1_UP;				PassThruConfig.TxMode = MODE_LINE1_TX_LINE0_GND_NORMAL;LINE0_OR_LINE1_LOW;	//tx_c_channel();//LINE0上正常发送,line1接地
#define LINE1_TX_LINE0_GND_REVERSE					LINE1_TX;				TX_REVERSE;				LINE_NO_LOAD;		LINE0_UP;			LINE1_DOWN;			PassThruConfig.TxMode = MODE_LINE1_TX_LINE0_GND_REVERSE;LINE0_OR_LINE1_LOW;	//tx_c_channel();//LINE0上反转发送,line1接地

#define LINE0_LINE1_TWOCONTROL_NORMAL				LINE0_TX;				TX_NORMAL;				LINE1_UP;		LINE0_UP;			LINE0_RX;					RX_NORMAL;		EN_SELF_TX_RX; // tx_c_channel();//TXD_MOD3_LOW;	TXD_MOD2_LOW;	TXD_MOD1_LOW;		TXD_MOD0_LOW;	 SET_TX0_REVSERSE_HIGH;	SetEcuLineLoadMode(LINE0, UP_LOAD)//在line0上正常发送
#define LINE0_LINE1_TWOCONTROL_REVERSE 			LINE0_TX;				TX_REVERSE;				LINE1_DOWN;	LINE0_DOWN;		LINE0_RX;					RX_REVERSE;		EN_SELF_TX_RX;  //tx_c_channel();//TXD_MOD3_LOW;	TXD_MOD2_LOW;	TXD_MOD1_LOW;		TXD_MOD0_LOW;	 SET_TX0_REVSERSE_LOW;	SetEcuLineLoadMode(LINE0, DOWN_LOAD)//line0上反转发送

#define FIRST_SINGLE_CAN_TX 								LINE0_DOWN;		  SINGLE_CAN_TX;		//tx_c_channel();
#define FIRST_HIGH_CAN_TX									  LINE_NO_LOAD;		HIGH_CAN_TX;		//	tx_c_channel();
#define RS232_TXD												    LINE_NO_LOAD;		RS232_MODE_TX;	//	tx_c_channel();
//#define RS485_TXD												    LINE0_UP;				LINE1_DOWN;				RS485_MODE_TX;		tx_c_channel();不使用
#define BENZ_TX															LINE0_TX;				TX_NORMAL;				LINE0_UP;			LINE1_UP;			//qu tx_c_channel();
#define BENZ_TX_RX													LINE0_TX;				TX_NORMAL;				LINE_NO_LOAD;	LINE0_RX;			RX_NORMAL;		EN_SELF_TX_RX;		////qu tx_c_channel();
#define TOYO_PULSE													LINE0_TX;				TX_NORMAL;				LINE0_RX;			LINE_NO_LOAD;	////qu tx_c_channel();
//接收函数模式控制
#define LINE0_RX								ChannelBuf[2]&=0x0F;ChannelBuf[2]|=0x00
#define LINE1_RX								ChannelBuf[2]&=0x0F;ChannelBuf[2]|=0x10
#define LINE0_LINE1_RX_R				ChannelBuf[2]&=0x0F;ChannelBuf[2]|=0x20
#define LINE0_LINE1_RX_N				ChannelBuf[2]&=0x0F;ChannelBuf[2]|=0x30
#define DIFFERENTIAL_R					ChannelBuf[2]&=0x0F;ChannelBuf[2]|=0x40
#define SINGLE_CAN_R  					ChannelBuf[2]&=0x0F;ChannelBuf[2]|=0x50
#define HIGH_CAN_R  					  ChannelBuf[2]&=0x0F;ChannelBuf[2]|=0x60
#define RS232_R  								ChannelBuf[2]&=0x0F;ChannelBuf[2]|=0x70
//#define RS485_R  								ChannelBuf[2]&=0x0F;ChannelBuf[2]|=0x80//不使用
#define DISABLE_R								ChannelBuf[2]&=0x0F;ChannelBuf[2]|=0xF0
//接收正反转
#define RX_NORMAL								ChannelBuf[3]&=0x01;ChannelBuf[3]|=0x02
#define RX_REVERSE							ChannelBuf[3]&=0x01;ChannelBuf[3]|=0x00
#define tx_c_channel1() 

#define ONLY_LINE0_RX_NORMAL    			LINE0_RX;					RX_NORMAL;		EN_SELF_TX_RX;	LINE0_UP;				//tx_c_channel1();	 PassThruConfig.RxMode = MODE_ONLY_LINE0_RX_NORMAL;
#define ONLY_LINE0_RX_REVERSE   			LINE0_RX;					RX_REVERSE;		EN_SELF_TX_RX;	LINE0_DOWN;			//tx_c_channel1();	 PassThruConfig.RxMode = MODE_ONLY_LINE0_RX_REVERSE;
#define ONLY_LINE1_RX_NORMAL					LINE1_RX;					RX_NORMAL;		EN_SELF_TX_RX;	LINE1_UP;			//	tx_c_channel1();	 PassThruConfig.RxMode = MODE_ONLY_LINE1_RX_NORMAL;
#define ONLY_LINE1_RX_REVERSE					LINE1_RX;					RX_REVERSE;		EN_SELF_TX_RX;	LINE1_DOWN;			//tx_c_channel1();	 PassThruConfig.RxMode = MODE_ONLY_LINE1_RX_REVERSE;
#define LINE0_LINE1_RX_NORMAL   			LINE0_LINE1_RX_N;	RX_NORMAL;		EN_SELF_TX_RX;	LINE0_UP;				LINE1_UP;				//tx_c_channel()1;	 PassThruConfig.RxMode = MODE_LINE0_LINE1_RX_NORMAL;
#define LINE0_LINE1_RX_REVERSE 				LINE0_LINE1_RX_R;	RX_REVERSE;		EN_SELF_TX_RX;	LINE0_DOWN;			LINE1_DOWN;			//tx_c_channel1();	 PassThruConfig.RxMode = MODE_LINE0_LINE1_RX_REVERSE;
#define DIFFERENTIAL_RX_LINE0DOWN			DIFFERENTIAL_R;		RX_REVERSE;		EN_SELF_TX_RX;	//tx_c_channel1();	 PassThruConfig.RxMode = MODE_DIFFERENTIAL_RX_LINE0DOWN;
#define DIFFERENTIAL_RX_LINE0UP				DIFFERENTIAL_R;		RX_NORMAL;		EN_SELF_TX_RX;	//tx_c_channel1();	 PassThruConfig.RxMode = MODE_DIFFERENTIAL_RX_LINE0UP;//和上面相反，为line0上拉，此时上拉电阻为1k，波形会好点


#define FIRST_SINGLE_CAN_RX						SINGLE_CAN_R;			EN_SELF_TX_RX;	//tx_c_channel();			
#define FIRST_HIGH_CAN_RX							HIGH_CAN_R;				//tx_c_channel();
#define RS232_RXD											RS232_R;					EN_SELF_TX_RX;	//tx_c_channel();	
//#define RS485_RXD											RS485_R;					DIS_SELF_TX_RX;	tx_c_channel();	//不使用

#define DISABLE_RECEIVE 				DISABLE_R;				RX_NORMAL;		//tx_c_channel();
#define LINE0_RX_VOLVO				  LINE0_RX;					RX_REVERSE;		LINE0_UP;				//tx_c_channel();
#define LINE1_RX_VOLVO					LINE1_RX;					RX_REVERSE;		LINE1_UP;				//tx_c_channel();
#define LINE0_LINE1_RX_VOLVO 		LINE1_RX;					RX_REVERSE;		LINE0_UP;				LINE1_UP;				//tx_c_channel();

/*************************接收模式控制****************
编号	RXD_MODE2	RXD_MODE1	RXD_MODE0	处理方式
1				0					0					0	LINE0透传接收端数据到rxd或gpio口
2				0					0					1	LINE1透传接收端数据到rxd或gpio口
3				0					1					0	LINE0,LINE1同时反相接收数据到rxd,注意:该工作方式下，当line0和line1在一根线发送数据时，另一根线保持低电平才可收发数据,
														此模式下，SET_TX0_REVSERSE_LOW一定要设置
4				0					1					1	LINE0,LINE1同时正相接收数据到rxd,注意:该工作方式下，当line0和line1在一根线发送数据时，另一根线保持高电平才可收发数据
														此模式下，SET_TX0_REVSERSE_HIGH一定要设置
5				1					0					0	LINE0,LINE1作为差分接收，传输结果至rxd
6				1					0					1	预留
7				1					1					0	预留
8				1					1					1	禁止接收使能*/

#define ONLY_LINE0_RX_NORMAL_NO_LOAD    LINE0_RX;					RX_NORMAL;		// qu tx_c_channel();//SetEcuLineLoadMode(LINE0, UP_LOAD)//RXD_MOD2_LOW;	RXD_MOD1_LOW;		RXD_MOD0_LOW;  SET_RX0_REVERSE_HIGH;	SetEcuLineLoadMode(LINE0, UP_LOAD)	//line0正常输入	
#define ONLY_LINE0_RX_REVERSE_NO_LOAD   LINE0_RX;					RX_REVERSE;		//qu tx_c_channel();//SetEcuLineLoadMode(LINE0, DOWN_LOAD)//RXD_MOD2_LOW;	RXD_MOD1_LOW;		RXD_MOD0_LOW;  SET_RX0_REVERSE_LOW;		SetEcuLineLoadMode(LINE0, DOWN_LOAD)	//line0正常输入	
#define ONLY_LINE1_RX_NORMAL_NO_LOAD		LINE1_RX;					RX_NORMAL;	//qu	tx_c_channel();//SetEcuLineLoadMode(LINE1, UP_LOAD)//RXD_MOD2_LOW;	RXD_MOD1_LOW;		RXD_MOD0_HIGH; SET_RX0_REVERSE_HIGH;	SetEcuLineLoadMode(LINE1, UP_LOAD)	//line1正常输入	
#define ONLY_LINE1_RX_REVERSE_NO_LOAD		LINE1_RX;					RX_REVERSE;		// qutx_c_channel();//SetEcuLineLoadMode(LINE1, DOWN_LOAD)//RXD_MOD2_LOW;	RXD_MOD1_LOW;		RXD_MOD0_HIGH; SET_RX0_REVERSE_LOW;		SetEcuLineLoadMode(LINE1, DOWN_LOAD)//line1正常输入	
#define LINE0_LINE1_RX_NORMAL_NO_LOAD   LINE0_LINE1_RX_N;	RX_NORMAL;		// qu tx_c_channel();//RXD_MOD2_LOW; RXD_MOD1_HIGH;	RXD_MOD0_HIGH; SET_RX0_REVERSE_HIGH//line0line1常态高电平，正常输入	
#define LINE0_LINE1_RX_REVERSE_NO_LOAD 	LINE0_LINE1_RX_R;	RX_REVERSE;		LINE0_DOWN;			//qu tx_c_channel();//RXD_MOD2_LOW; RXD_MOD1_HIGH;	RXD_MOD0_LOW;	 SET_RX0_REVERSE_LOW//LINE0line1常态低电平，反转输入	
#define DIFFERENTIAL_RX_NO_LOAD   			DIFFERENTIAL_R;		RX_REVERSE;		//qu tx_c_channel();//RXD_MOD2_HIGH;RXD_MOD1_LOW;		RXD_MOD0_LOW;	 SET_RX0_REVERSE_LOW//line0line1差分输入

//*******************控制line0 line1线上电压

//PDU硬件版本宏 以下有且只能定义一个
//#define  MINI_HW_001		1    //第一版PDU硬件

#define  MINI_HW_002		1    //第二版PDU硬件

//#define  MINI_CAN_retransmission		1     

#endif

