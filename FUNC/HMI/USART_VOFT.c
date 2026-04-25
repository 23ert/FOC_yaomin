#include "USART_VOFT.h"
#include "func_main.h"


// 使用时初始化
struct Frame FrameV = {
    .fdata = {0.0f},
	.data = {0,0,0,0},
    .tail = {0x00, 0x00, 0x80, 0x7f}
};

static int append_array_fixed(uint8_t* c, 
                       const uint8_t* a, size_t a_len, 
                       const uint8_t* b, size_t b_len) 
{
    // 参数检查
    if (c == NULL || a == NULL || b == NULL) {
        return -1;
    }
    
    // 复制 a 数组
    memcpy(c, a, a_len * sizeof(uint8_t));
    
    // 复制 b 数组到 a 后面
    memcpy(c + a_len, b, b_len * sizeof(uint8_t));
    
    return 0; // 成功
}


void VOFT_comClark(void)
{
    Uint16 Xbufsize;
    Xbufsize = 0;
    FrameV.fdata[Xbufsize++] = ia;
    FrameV.fdata[Xbufsize++] = ib;
    FrameV.fdata[Xbufsize++] = ic;
    FrameV.fdata[Xbufsize++] = rtY.ialpha;
    FrameV.fdata[Xbufsize++] = rtY.ibeta;
}


void VOFT_SendData(void)
{
    //数据合并拷贝到数据buf
	memcpy(FrameV.data,(uint8_t*)&FrameV.fdata,sizeof(FrameV.fdata));
	
	size_t a_len = CH_COUNT*4;
    size_t b_len = 4;
    size_t total_len = a_len + b_len;    
    // 分配输出数组
    uint8_t c[total_len];
    //加上数据头
    int ret = append_array_fixed(c, FrameV.data, a_len, FrameV.tail, b_len);
	if (ret == 0) 
	{
		HAL_UART_Transmit_DMA(&huart3,(uint8_t*)c,total_len);
	}
	HAL_Delay(1);

}

void VOFT_sendPWM(void)
{
    uint8_t Xbufsize = 0;
    if((GPIOA->IDR & GPIO_PIN_8) !=0 )
    {
       FrameV.fdata[Xbufsize++] = 1.0f; 
    }
    else
    {
      FrameV.fdata[Xbufsize++] = 0.0f;
    }

    if((GPIOA->IDR & GPIO_PIN_9) !=0 )
    {
       FrameV.fdata[Xbufsize++] = 3.0f; 
    }
    else
    {
      FrameV.fdata[Xbufsize++] = 2.0f;
    }

    if((GPIOA->IDR & GPIO_PIN_10) !=0 )
    {
       FrameV.fdata[Xbufsize++] = 5.0f; 
    }
    else
    {
      FrameV.fdata[Xbufsize++] = 4.0f;
    }	

}


void VOFT_sendUdc(void)
{
//	//n个数据单独获取
//	uint8_t bufsize = 0;
//	HAL_ADC_Start(&hadc1);
//	temp[bufsize++] = HAL_ADC_GetValue(&hadc1);
//	temp[bufsize++] = 1;
//	//数据合并拷贝到数据buf
//	memcpy(FrameV.data,(uint8_t*)&temp,sizeof(temp));

  HAL_ADCEx_InjectedStart_IT(&hadc1);
  HAL_ADCEx_InjectedStart_IT(&hadc2);
	
	//n个数据单独获取
	uint8_t Xbufsize = 0;
	HAL_ADC_Start(&hadc1);
	FrameV.fdata[Xbufsize++] = HAL_ADC_GetValue(&hadc1); //电位器          //阻塞式读取，后续优化
	
	HAL_ADC_Start(&hadc2);
	FrameV.fdata[Xbufsize++] = HAL_ADC_GetValue(&hadc2)*0.02094726f; //母线电压 
	//4096对应3.3V
	//x对应3.3*x/4096V*78/3  V
	
	//使用Q15定点
	//0.02094726使用// 结果 result 就是物理量的 Q15 定点数表示
	//int32_t result = ((int32_t)x * 686) >> 15;
	//FrameV.fdata[Xbufsize++] = (HAL_ADC_GetValue(&hadc2)*686)>>15; //母线电压 
	
	
	

}

int fputc(int ch,FILE *f)
{
	while((USART3->ISR  & 0x40) == 0);
	USART3->TDR = (uint8_t)ch;
	return ch;
}
