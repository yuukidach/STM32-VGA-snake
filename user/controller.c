/**
  ******************************************************************************
  * @author        Yuuki_Dach
  * @version       V1.1.1
  * @date          7-November-2016
  * @description   Functions of controller. 
  ******************************************************************************
  * @attention
  *
  * OUR CONTROLLER IS PS2 WHICH HAS A HIGH COST-EFFECTIVE. ITS INSTRUCTIONS CAN
  * BE DOWNLOADED FROM THE INTERNET. OR YOU CAN FIND SOME OF THEM IN MY GITHUB'S
  * PROJECTS FILES. MY GITHUB IS: https://github.com/yuukidach .
  *
  * <h2><center>&copy; COPYRIGHT 2016 Yuuki_Dach</center></h2>
  ******************************************************************************
  */

#include "controller.h"
#include "delay.h"

uint32_t cnt = 0;
uint8_t part3Dir = 0;
uint16_t buttonValue;
uint8_t PS2_Cmd[2] = {0x01, 0x42};
uint8_t PS2_Data[9] = {0x00};
uint8_t PS2_Mask[]={
		PSB_SELECT,
    PSB_L3,
    PSB_R3 ,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1 ,
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK
};


/**
 * @brief	Configurate the LED of PE5 to show the status of the control mode.
 * @param	None
 * @retval None
 */
void controlModeConfirm_Config(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}


void controller_config(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(PS2_CLK_GPIO, ENABLE);
    GPIO_InitStructure.GPIO_Pin = PS2_DO | PS2_CS | PS2_CLK;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PS2_GPIO, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin = PS2_DI;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(PS2_GPIO, &GPIO_InitStructure);
  
    controlModeConfirm_Config();
	
    shortPoll();
    shortPoll();
    shortPoll();
    enterSettings();
    turnOnModeChange();
    turnOnVibrationMode();
    saveChangesAndExit();
	
    clrPS2Buff();
}


void sendCmd2PS2(uint8_t command){
    __IO  uint16_t ref = 0x01;
    PS2_Data[1] = 0x00;
	
    for(ref = 0x01; ref < 0x0100; ref <<= 1){
        if(ref & command)
            GPIO_SetBits(PS2_GPIO, PS2_DO);
        else 
            GPIO_ResetBits(PS2_GPIO, PS2_DO);
		
        GPIO_SetBits(PS2_GPIO, PS2_CLK);
        delay_us(2);
        GPIO_ResetBits(PS2_GPIO, PS2_CLK);
        delay_us(2);
        GPIO_SetBits(PS2_GPIO, PS2_CLK);
        if(GPIO_ReadInputDataBit(PS2_GPIO, PS2_DI))
            PS2_Data[1] = ref | PS2_Data[1];
    }
    delay_us(16);
}


void sendCmd2MCU(void){
    __IO uint16_t ref = 0x01;
    __IO uint8_t byte = 0;
	
    GPIO_ResetBits(PS2_GPIO, PS2_CS);
    sendCmd2PS2( PS2_Cmd[PS2_START]);
    sendCmd2PS2( PS2_Cmd[PS2_REQUEST_DATA]);
	
    for( byte = 2; byte < 9; ++byte){
        for(ref = 0x01; ref < 0x0100; ref <<= 1){
            GPIO_SetBits(PS2_GPIO, PS2_CLK);
            delay_us(2);
            GPIO_ResetBits(PS2_GPIO, PS2_CLK);
            delay_us(2);
            GPIO_SetBits(PS2_GPIO, PS2_CLK);
            if(GPIO_ReadInputDataBit(PS2_GPIO, PS2_DI))
                PS2_Data[byte] = ref | PS2_Data[byte];
            }
        delay_us(16);
        }
    GPIO_SetBits(PS2_GPIO, PS2_CS);
}


uint8_t isStickMode(void){
    GPIO_ResetBits(PS2_GPIO, PS2_CS);
    sendCmd2PS2( PS2_Cmd[PS2_START]);
    sendCmd2PS2( PS2_Cmd[PS2_REQUEST_DATA]);
    GPIO_SetBits(PS2_GPIO, PS2_CS);
	
    if(PS2_Data[1] == 0x41) return NO;
    else return YES;
}


uint8_t getStickData(uint8_t stick){
    getButtonData();
    return PS2_Data[stick];
}


uint8_t getButtonData(void){
    uint8_t idx;
	
    clrPS2Buff();
    sendCmd2MCU();
	
    buttonValue = (PS2_Data[4] << 8) | PS2_Data[3];
    for(idx = 0; idx < 16; ++idx){
        if((buttonValue & (1<<PS2_Mask[idx]) ) == PRESSED)
            return idx;
    }
    return 0;
}


void clrPS2Buff(void){
    uint8_t i;
    for(i = 0; i < 9; ++i)
        PS2_Data[i] = 0x00;
}


void shortPoll(void){
    GPIO_ResetBits(PS2_GPIO, PS2_CS);
    delay_us(16);
    sendCmd2PS2(0x01);  
    sendCmd2PS2(0x42);
    sendCmd2PS2(0x00);
    sendCmd2PS2(0x00);
    sendCmd2PS2(0x00);
    GPIO_SetBits(PS2_GPIO, PS2_CS);
    delay_us(16);	
}


void enterSettings(void){
    GPIO_ResetBits(PS2_GPIO, PS2_CS);
    delay_us(16);
    sendCmd2PS2(0x01);   
    sendCmd2PS2(0x43);  
    sendCmd2PS2(0x00); 
    sendCmd2PS2(0x01); 
    sendCmd2PS2(0x00);
    sendCmd2PS2(0x00);
    sendCmd2PS2(0x00);
    sendCmd2PS2(0x00);
    sendCmd2PS2(0x00);
    GPIO_SetBits(PS2_GPIO, PS2_CS);
    delay_us(16);
}


void turnOnModeChange(void){
    GPIO_ResetBits(PS2_GPIO, PS2_CS);
    sendCmd2PS2(0x01);
    sendCmd2PS2(0x44);
    sendCmd2PS2(0x00);
    sendCmd2PS2(0x01);		// stick mode = 0x01; button mode = 0x00  Set the sent mode
    sendCmd2PS2(0xee);		// Ox03: Latch settings. this means we cannot use button "MODE" to set the mode 
                        // 0xee: Do not latch.
    sendCmd2PS2(0x00);
    sendCmd2PS2(0x00);
    sendCmd2PS2(0x00);
    sendCmd2PS2(0x00);
    GPIO_SetBits(PS2_GPIO, PS2_CS);
    delay_us(16);
}


void turnOnVibrationMode(void){    
    GPIO_ResetBits(PS2_GPIO, PS2_CS);
    delay_us(16);
    sendCmd2PS2(0x01);  
    sendCmd2PS2(0x4d);  
    sendCmd2PS2(0x00);
    sendCmd2PS2(0x00);
    sendCmd2PS2(0x01);
    GPIO_SetBits(PS2_GPIO, PS2_CS);
    delay_us(16);	
}


void saveChangesAndExit(void){
    GPIO_ResetBits(PS2_GPIO, PS2_CS);
    delay_us(16);
    sendCmd2PS2(0x01); 
    sendCmd2PS2(0x43);  
    sendCmd2PS2(0x00);
    sendCmd2PS2(0x00);
    sendCmd2PS2(0x5a);
    sendCmd2PS2(0x5a);
    sendCmd2PS2(0x5a);
    sendCmd2PS2(0x5a);
    sendCmd2PS2(0x5a);
    GPIO_SetBits(PS2_GPIO, PS2_CS);
    delay_us(16);
}


//After the vibration begins, we must have a 1000ms delay.
void PS2_Vibration(u8 motor1, u8 motor2){    
    GPIO_ResetBits(PS2_GPIO, PS2_CS);
    delay_us(16);
    sendCmd2PS2(0x01); 
    sendCmd2PS2(0x42);  
    sendCmd2PS2(0X00);
    sendCmd2PS2(motor1);
    sendCmd2PS2(motor2);
    sendCmd2PS2(0x00);
    sendCmd2PS2(0x00);
    sendCmd2PS2(0x00);
    sendCmd2PS2(0x00);
    GPIO_SetBits(PS2_GPIO, PS2_CS);
    delay_us(16);  
}


uint8_t isAutoControl(void) {
    if(getButtonData() == PSB_L1) {
        for (int i = 0; i < 48; ++i) while(getButtonData() == PSB_L1);
        ++cnt;
    }
    if (cnt & 0x1) {
        GPIO_SetBits(GPIOE, GPIO_Pin_5);
        return 1;
    } else {
        GPIO_ResetBits(GPIOE, GPIO_Pin_5);
        return 0;
    }
}

/*
 * @brief	Tell the car which direction to go during part 3.
 *        The default return value is PART3LEFT(0), once it is change to PART3RIGHT(1), it cannot be changed back.
 * @param	None
 * @retval PART3RIGHT (1): Means turning right at part 3
 *         PART3LEFT  (0): Means turning left at part 3
 */
uint8_t getPart3Direction(void) {
    uint8_t i = 0;
    for (i = 0; i < 10; ++i) while(getButtonData() == PSB_L2);
    if (i == 10 && getButtonData() == PSB_L2) {        
        part3Dir = PART3RIGHT;
    }
    return part3Dir;
}
/******************* (C) COPYRIGHT 2016 Yuuki_Dach *************END OF FILE****/









