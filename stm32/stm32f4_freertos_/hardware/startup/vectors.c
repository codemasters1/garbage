/* Symbol __main_stack_end jest tworzony w skrypcie linkera */
extern const char __main_stack_end;

void Reset_Handler(void);
static void __Default_Handler(void) __attribute__ ((interrupt));

/* Wszystkie niewykorzystane przerwania przekierowujemy do default handlera */
static void __Default_Handler(void)
{
   while (1);
}

void NMI_Handler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void HardFault_Handler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void MemManage_Handler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void BusFault_Handler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void UsageFault_Handler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void __Reserved_0x1C_Handler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void __Reserved_0x20_Handler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void __Reserved_0x24_Handler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void __Reserved_0x28_Handler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void SVC_Handler(void)
__attribute__((interrupt, weak, alias("__Default_Handler")));
void DebugMon_Handler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void __Reserved_0x34_Handler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void PendSV_Handler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void SysTick_Handler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void WWDG_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void PVD_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void TAMP_STAMP_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void RTC_WKUP_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void FLASH_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void RCC_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void EXTI0_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void EXTI1_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void EXTI2_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void EXTI3_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void EXTI4_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void DMA1_Stream0_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void DMA1_Stream1_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void DMA1_Stream2_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void DMA1_Stream3_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void DMA1_Stream4_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void DMA1_Stream5_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void DMA1_Stream6_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void ADC_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void CAN1_TX_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void CAN1_RX0_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void CAN1_RX1_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void CAN1_SCE_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void EXTI9_5_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void TIM1_BRK_TIM9_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void TIM1_UP_TIM10_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void TIM1_TRG_COM_TIM11_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void TIM1_CC_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void TIM2_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void TIM3_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void TIM4_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void I2C1_EV_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void I2C1_ER_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void I2C2_EV_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void I2C2_ER_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void SPI1_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void SPI2_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void USART1_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void USART2_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void USART3_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void EXTI15_10_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void RTC_Alarm_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void OTG_FS_WKUP_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void TIM8_BRK_TIM12_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void TIM8_UP_TIM13_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void TIM8_TRG_COM_TIM14_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void TIM8_CC_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void DMA1_Stream7_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void FSMC_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void SDIO_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void TIM5_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void SPI3_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void UART4_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void UART5_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void TIM6_DAC_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void TIM7_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void DMA2_Stream0_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void DMA2_Stream1_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void DMA2_Stream2_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void DMA2_Stream3_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void DMA2_Stream4_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void ETH_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void ETH_WKUP_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void CAN2_TX_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void CAN2_RX0_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void CAN2_RX1_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void CAN2_SCE_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void OTG_FS_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void DMA2_Stream5_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void DMA2_Stream6_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void DMA2_Stream7_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void USART6_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void I2C3_EV_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void I2C3_ER_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void OTG_HS_EP1_OUT_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void OTG_HS_EP1_IN_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void OTG_HS_WKUP_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void OTG_HS_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void DCMI_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void CRYP_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void HASH_RNG_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));
void FPU_IRQHandler(void)
__attribute__ ((interrupt, weak, alias("__Default_Handler")));

/* Wektor przerwan dla STM32F4xx */
void (* const vectors[])(void) __attribute__ ((section(".vectors"))) =
{
   (void (*)(void))&__main_stack_end,
         Reset_Handler,
         NMI_Handler,
         HardFault_Handler,
         MemManage_Handler,
         BusFault_Handler,
         UsageFault_Handler,
         __Reserved_0x1C_Handler,
         __Reserved_0x20_Handler,
         __Reserved_0x24_Handler,
         __Reserved_0x28_Handler,
         SVC_Handler,
         DebugMon_Handler,
         __Reserved_0x34_Handler,
         PendSV_Handler,
         SysTick_Handler,
         WWDG_IRQHandler,
         PVD_IRQHandler,
         TAMP_STAMP_IRQHandler,
         RTC_WKUP_IRQHandler,
         FLASH_IRQHandler,
         RCC_IRQHandler,
         EXTI0_IRQHandler,
         EXTI1_IRQHandler,
         EXTI2_IRQHandler,
         EXTI3_IRQHandler,
         EXTI4_IRQHandler,
         DMA1_Stream0_IRQHandler,
         DMA1_Stream1_IRQHandler,
         DMA1_Stream2_IRQHandler,
         DMA1_Stream3_IRQHandler,
         DMA1_Stream4_IRQHandler,
         DMA1_Stream5_IRQHandler,
         DMA1_Stream6_IRQHandler,
         ADC_IRQHandler,
         CAN1_TX_IRQHandler,
         CAN1_RX0_IRQHandler,
         CAN1_RX1_IRQHandler,
         CAN1_SCE_IRQHandler,
         EXTI9_5_IRQHandler,
         TIM1_BRK_TIM9_IRQHandler,
         TIM1_UP_TIM10_IRQHandler,
         TIM1_TRG_COM_TIM11_IRQHandler,
         TIM1_CC_IRQHandler,
         TIM2_IRQHandler,
         TIM3_IRQHandler,
         TIM4_IRQHandler,
         I2C1_EV_IRQHandler,
         I2C1_ER_IRQHandler,
         I2C2_EV_IRQHandler,
         I2C2_ER_IRQHandler,
         SPI1_IRQHandler,
         SPI2_IRQHandler,
         USART1_IRQHandler,
         USART2_IRQHandler,
         USART3_IRQHandler,
         EXTI15_10_IRQHandler,
         RTC_Alarm_IRQHandler,
         OTG_FS_WKUP_IRQHandler,
         TIM8_BRK_TIM12_IRQHandler,
         TIM8_UP_TIM13_IRQHandler,
         TIM8_TRG_COM_TIM14_IRQHandler,
         TIM8_CC_IRQHandler,
         DMA1_Stream7_IRQHandler,
         FSMC_IRQHandler,
         SDIO_IRQHandler,
         TIM5_IRQHandler,
         SPI3_IRQHandler,
         UART4_IRQHandler,
         UART5_IRQHandler,
         TIM6_DAC_IRQHandler,
         TIM7_IRQHandler,
         DMA2_Stream0_IRQHandler,
         DMA2_Stream1_IRQHandler,
         DMA2_Stream2_IRQHandler,
         DMA2_Stream3_IRQHandler,
         DMA2_Stream4_IRQHandler,
         ETH_IRQHandler,
         ETH_WKUP_IRQHandler,
         CAN2_TX_IRQHandler,
         CAN2_RX0_IRQHandler,
         CAN2_RX1_IRQHandler,
         CAN2_SCE_IRQHandler,
         OTG_FS_IRQHandler,
         DMA2_Stream5_IRQHandler,
         DMA2_Stream6_IRQHandler,
         DMA2_Stream7_IRQHandler,
         USART6_IRQHandler,
         I2C3_EV_IRQHandler,
         I2C3_ER_IRQHandler,
         OTG_HS_EP1_OUT_IRQHandler,
         OTG_HS_EP1_IN_IRQHandler,
         OTG_HS_WKUP_IRQHandler,
         OTG_HS_IRQHandler,
         DCMI_IRQHandler,
         CRYP_IRQHandler,
         HASH_RNG_IRQHandler,
         FPU_IRQHandler,
};
