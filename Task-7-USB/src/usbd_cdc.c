/**
  ******************************************************************************
  * @file    usbd_cdc_vcp.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    22-July-2011
  * @brief   Generic media access Layer.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED 
#pragma     data_alignment = 4 
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc.h"
#include "stm32f4_discovery.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* These are external variables imported from CDC core to be used for IN 
   transfer management. */
extern uint8_t  APP_Rx_Buffer []; /* Write CDC received data in this buffer.
                                     These data will be sent over USB IN endpoint
                                     in the CDC core functions. */
extern uint32_t APP_Rx_ptr_in;    /* Increment this pointer or roll it back to
                                     start address when writing received data
                                     in the buffer APP_Rx_Buffer. */

/* Private function prototypes -----------------------------------------------*/

static uint16_t cdc_Init     (void);
static uint16_t cdc_DeInit   (void);
static uint16_t cdc_Ctrl     (uint32_t Cmd, uint8_t* Buf, uint32_t Len);
static uint16_t cdc_DataTx   (uint8_t* Buf, uint32_t Len);
static uint16_t cdc_DataRx   (uint8_t* Buf, uint32_t Len);


CDC_IF_Prop_TypeDef cdc_fops =
{
  cdc_Init,
  cdc_DeInit,
  cdc_Ctrl,
  cdc_DataTx,
  cdc_DataRx
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  cdc_Init
  *         Initializes the Media on the STM32
  * @param  None
  * @retval Result of the opeartion (USBD_OK in all cases)
  */
static uint16_t cdc_Init(void)
{
  return USBD_OK;
}


static uint16_t cdc_DeInit(void)
{

  return USBD_OK;
}


/**
  * @brief  cdc_Ctrl
  *         Manage the CDC class requests
  * @param  Cmd: Command code            
  * @param  Buf: Buffer containing command data (request parameters)
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the opeartion (USBD_OK in all cases
  */
//not really necessary for this example
static uint16_t cdc_Ctrl (uint32_t Cmd, uint8_t* Buf, uint32_t Len)
{ 
  switch (Cmd)
  {
  case SEND_ENCAPSULATED_COMMAND:
    /* Not  needed for this driver */
    break;

  case GET_ENCAPSULATED_RESPONSE:
    /* Not  needed for this driver */
    break;

  case SET_COMM_FEATURE:
    /* Not  needed for this driver */
    break;

  case GET_COMM_FEATURE:
    /* Not  needed for this driver */
    break;

  case CLEAR_COMM_FEATURE:
    /* Not  needed for this driver */
    break;

  case SET_LINE_CODING:

    break;

  case GET_LINE_CODING:

    break;

  case SET_CONTROL_LINE_STATE:
    /* Not  needed for this driver */
    break;

  case SEND_BREAK:
    /* Not  needed for this driver */
    break;    
    
  default:
    break;
  }

  return USBD_OK;
}

/**
  * @brief  cdc_DataTx
  *         CDC received data to be send over USB IN endpoint are managed in 
  *         this function.
  * @param  Buf: Buffer of data to be sent
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the opeartion: USBD_OK
  */
static uint16_t cdc_DataTx (uint8_t* Buf, uint32_t Len)
{

	uint32_t i;
	//loop through buffer
	for( i = 0; i < Len; i++ )
	{
		//push data into transfer buffer
		APP_Rx_Buffer[APP_Rx_ptr_in] = Buf[i] ;
		//increase pointer value
		APP_Rx_ptr_in++;
		/* To avoid buffer overflow */
		if(APP_Rx_ptr_in == APP_RX_DATA_SIZE)
		{
			APP_Rx_ptr_in = 0;
		}
	}

	return USBD_OK;
}

/**
  * @brief  cdc_DataRx
  *         Data received over USB OUT endpoint are sent over CDC interface 
  *         through this function.
  *           
  *         For this example we are just going to send received data right back to sender.
  *
  *         @note
  *         This function will block any OUT packet reception on USB endpoint 
  *         untill exiting this function. If you exit this function before transfer
  *         is complete on CDC interface (ie. using DMA controller) it will result 
  *         in receiving more data while previous ones are still not sent.
  *                 
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the opeartion: USBD_OK
  */
static uint16_t cdc_DataRx (uint8_t* Buf, uint32_t Len)
{
	uint32_t i;
	//loop through buffer
	for (i = 0; i < Len; i++)
	{
		//if there is an 'a' in buffer
		if (*(Buf + i) == 'a' || *(Buf + i) == 'A' )
		{
			//turn on the blue led
			STM32F4_Discovery_LEDOn(LED6);
		}
		//if there is an 's' in buffer
		else if (*(Buf + i) == 's' || *(Buf + i) == 'S' )
		{
			//turn off the blue led
			STM32F4_Discovery_LEDOff(LED6);
		}
	}
	//send received data back to sender
	cdc_DataTx( Buf, Len );
	return USBD_OK;
}


//this function is run when the user button is pushed
void DISCOVERY_EXTI_IRQHandler(void)
{
	uint8_t buffer[] = "terve"; // "terve" is hello in finnish :)
	cdc_DataTx (buffer, (uint32_t) 5);
}



/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
