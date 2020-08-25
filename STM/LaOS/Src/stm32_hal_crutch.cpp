/* Because STM's HAL is full of bags,
 * this file was born. */

/*! HAL almost always forget
 * to create ISR for its timebase timer */
#if(0)
void TIM1_UP_IRQHandler(void)
{
    extern TIM_HandleTypeDef htim1;
    HAL_TIM_IRQHandler(&htim1);
}
#endif
