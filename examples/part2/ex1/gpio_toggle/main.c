/**
  ******************************************************************************
  * \file    main.c
  * \author  Александр Смирнов
  * \version 1.0.0
  * \date    1.02.2022
  * \brief   Программа на языке C для учебного стенда на базе
  *          STM32F072RBT6 в среде разработки Keil uVision 5.
  *          Подключение библиотек поддержки МК STM32F072RBT6 осуществляется
  *          средствами IDE Keil через Run-Time Enviroment (RTE).
  *          В верхнем положении ключа SW1 включается светодиод LED16,
  *          в нижнем светодиод LED16 выключается.
  *          Программа работает в режиме 0 учебного стенда (S1 = 0, S2 = 0).
  ******************************************************************************
  */

/* Подключение заголовочного файла с макроопределениями всех регистром специальных
   функций МК STM32F072RBT6. */
#include <stm32f0xx.h>

/* Функция main - точка входа в программу */
int main(void)
{
    /* Инициализация периферийных модулей */

    /* Включение тактирования модулей */
    RCC->AHBENR = RCC->AHBENR | RCC_AHBENR_GPIOAEN; /* Включение тактирования порта A */
    RCC->AHBENR = RCC->AHBENR | RCC_AHBENR_GPIOCEN; /* Включение тактирования порта C */

    /* Настройка на вывод линии PC15 (LED16) */
    GPIOC->MODER = GPIOC->MODER | GPIO_MODER_MODER15_0;

    /* Включение подтягивающего резистора линии PA11 (SW1) */
    GPIOA->PUPDR = GPIOA->PUPDR | GPIO_PUPDR_PUPDR11_0;

    /* Бесконечный цикл */
    while (1)
    {
        /* Объявление переменной sw1 и чтение состояние линии PA11 (SW1) */
        int sw1 = GPIOA->IDR & (1 << 11);

        /* Исполнение ветви в зависимости от состояния sw1 */
        if (sw1 != 0)
        {
            /* Включить светодиод PC15 (LED16).
               Типовой конструкцией для установки бита в определенной позиции
               является:
               Reg = Reg | (1 << n), где Reg - регистр, n - номер позиции. */
            GPIOC->ODR = GPIOC->ODR | (1 << 15);
        }
        else
        {
            /* Выключить светодиод PC15 (LED16).
               Типовой конструкцией для сброса бита в определенной позиции
               является:
               Reg = Reg & ~(1 << n), где Reg - регистр, n - номер позиции. */
            GPIOC->ODR = GPIOC->ODR & ~(1 << 15);
        }
    }
}
