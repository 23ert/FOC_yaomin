#include "USART_VOFT.h"
#include "func_main.h"


// 濞达綀娉曢弫銈夊籍鐠哄搫鐏ュ┑顔碱儏鐎碉拷
struct Frame FrameV = {
    .fdata = {0.0f},
	.data = {0,0,0,0},
    .tail = {0x00, 0x00, 0x80, 0x7f}
};

static int append_array_fixed(uint8_t* c, 
                       const uint8_t* a, size_t a_len, 
                       const uint8_t* b, size_t b_len) 
{
    // 闁告瑥鍊归弳鐔肺涢埀顒勫蓟閿燂拷
    if (c == NULL || a == NULL || b == NULL) {
        return -1;
    }
    
    // 濠㈣泛绉撮崺锟� a 闁轰焦澹嗙划锟�
    memcpy(c, a, a_len * sizeof(uint8_t));
    
    // 濠㈣泛绉撮崺锟� b 闁轰焦澹嗙划宥夊礆閿燂拷 a 闁告艾閰ｅ锟�
    memcpy(c + a_len, b, b_len * sizeof(uint8_t));
    
    return 0; // 闁瑰瓨鍔曟慨锟�
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
    //闁轰胶澧楀畵渚€宕ラ崼婵婂珯闁归鏌夌粈澶愬礆閻楀牊娈堕柟鍦暁uf
	memcpy(FrameV.data,(uint8_t*)&FrameV.fdata,sizeof(FrameV.fdata));
	
	size_t a_len = CH_COUNT*4;
    size_t b_len = 4;
    size_t total_len = a_len + b_len;    
    // 闁告帒妫濋崢銈嗘綇閹惧啿姣夐柡浣瑰缁拷
    uint8_t c[total_len];
    //闁告梻濮崇粭鍌炲极閻楀牆绁﹀璁规嫹
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
//	//n濞戞搩浜濋弳鐔煎箲椤旂厧绀嬮柣娆樺墲楠炲繘宕ｉ敓锟�
//	uint8_t bufsize = 0;
//	HAL_ADC_Start(&hadc1);
//	temp[bufsize++] = HAL_ADC_GetValue(&hadc1);
//	temp[bufsize++] = 1;
//	//闁轰胶澧楀畵渚€宕ラ崼婵婂珯闁归鏌夌粈澶愬礆閻楀牊娈堕柟鍦暁uf
//	memcpy(FrameV.data,(uint8_t*)&temp,sizeof(temp));

  HAL_ADCEx_InjectedStart_IT(&hadc1);
  HAL_ADCEx_InjectedStart_IT(&hadc2);
	
	//n濞戞搩浜濋弳鐔煎箲椤旂厧绀嬮柣娆樺墲楠炲繘宕ｉ敓锟�
	uint8_t Xbufsize = 0;
	HAL_ADC_Start(&hadc1);
	FrameV.fdata[Xbufsize++] = HAL_ADC_GetValue(&hadc1); //闁汇垹鍚€缂嶅懘宕抽敓锟�          //闂傚啳顕ч、锝咁嚕韫囨凹鍤㈤柛娆愮壄缁辨繈宕ユ惔锝囨暰濞村吋锚鐎碉拷
	
	HAL_ADC_Start(&hadc2);
	FrameV.fdata[Xbufsize++] = HAL_ADC_GetValue(&hadc2)*0.02094726f; //婵絽绉堕崵搴ㄦ偨闂堟稑绔� 
	//4096閻庣數鎳撶花锟�3.3V
	//x閻庣數鎳撶花锟�3.3*x/4096V*78/3  V
	
	//濞达綀娉曢弫顥�15閻庤姘ㄩ崑锟�
	//0.02094726濞达綀娉曢弫锟�// 缂備焦鎸婚悘锟� result 閻忓繗椴稿Σ鎼佹偋閳哄啯鍊為梺鎻掔箳濞堬拷 Q15 閻庤姘ㄩ崑锝夊极閹峰被鈧啰绮堥敓锟�
	//int32_t result = ((int32_t)x * 686) >> 15;
	//FrameV.fdata[Xbufsize++] = (HAL_ADC_GetValue(&hadc2)*686)>>15; //婵絽绉堕崵搴ㄦ偨闂堟稑绔� 
	
	
	

}

int fputc(int ch,FILE *f)
{
	while((USART3->ISR  & 0x40) == 0);
	USART3->TDR = (uint8_t)ch;
	return ch;
}
