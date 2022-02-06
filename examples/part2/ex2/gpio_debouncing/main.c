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
  *          Программа производит подсчет количества нажатий на кнопку SB1.
  *          Количество нажатий выводится на светодиоды LED1-LED16 в двоичном коде.
  *          Программа работает в режиме 0 учебного стенда (S1 = 0, S2 = 0).
  ******************************************************************************
  */


/* Подключение заголовочного файла с макроопределениями всех регистром специальных
   функций МК STM32F072RBT6. */
#include <stm32f0xx.h>

/* Функция программной временной задержки */
void software_delay(uint32_t ticks)
{
    while (ticks > 0)
    {
        ticks = ticks - 1;
    }
}

/* Функция инициализации светодиодов */
void led_init(void)
{
    /* Включение тактирования порта C */
    RCC->AHBENR = RCC->AHBENR | RCC_AHBENR_GPIOCEN;

    /* Настройка на вывод линий PC0 - PC15 (LED1 - LED16) */
    GPIOC->MODER = GPIOC->MODER |
        (GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 |
         GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 |
         GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 |
         GPIO_MODER_MODER9_0 | GPIO_MODER_MODER10_0 | GPIO_MODER_MODER11_0 |
         GPIO_MODER_MODER12_0 | GPIO_MODER_MODER13_0 | GPIO_MODER_MODER14_0 |
         GPIO_MODER_MODER15_0);
}

/* Функция установки состояния светодиодов */
void led_set(uint16_t led)
{
    /* Так как 0 - светодиод включен, то инвертируем аргумент led
       до записи в регистр ODR */
    GPIOC->ODR = ~led;
}

/* Функция инициализации кнопки SB1 */
void sb1_init(void)
{
    /* Включение подтягивающих резисторов PB4 (SB1) */
    GPIOB->PUPDR = GPIOB->PUPDR | GPIO_PUPDR_PUPDR4_0;
}

/* Перечисление с состояниями кнопки */
typedef enum {
    SB_PRESSED,     /* Кнопка нажата */
    SB_UNPRESSED    /* Кнопка отжата */
} sb_state_t;

/* Функция получения состояния кнопки SB1 с антидребезгом */
sb_state_t sb1_get_state(void)
{
    /* Здесь предложен следующий алгоритм антидребезга:
       В переменную pin_state через некоторый интервал 
       записывается состояние линии PB4 через переменную pin.
       Если записывать состояние кнопки, то состояние
       переменной pin_state будет следующим:
       11111010101010000000000000000000101010101111111111
       | 1  |   2   |           3      |    4  |    5   |
       Состояние 1,5 - кнопка отжата,
                 2,4 - дребезг при нажатии и отжатии,
                 3 - кнопка нажата.
       Выбирая количество повторяющихся нулей или единиц можно
       выбирать чувствительность антидребезга. Чем длительнее интервалы
       тем дольше нужно нажимать кнопку.
    */

    /* Объявление статической переменной, которая сохраняет свое значение
       между вызовами функции */
    static uint16_t pin_state = 0xFFFF;

    /* Программная задержка */
    software_delay(100);

    /* Чтение состояния кнопки SB1 */
    uint16_t pin = (GPIOB->IDR & (1 << 4)) >> 4;

    /* Сохранение нового состояния в переменную pin_state */
    pin_state = (pin_state << 1) | pin;

    /* Если 16 раз подряд сотояние SB1 было 0, то кнопка нажата */
    if (pin_state == 0x0000)
        return SB_PRESSED;
    else
        return SB_UNPRESSED;
}

/* Функция main - точка входа в программу */
int main(void)
{
    /* Инициализации светодиодов */
    led_init();
    /* Инициализация кнопки SB1 */
    sb1_init();

    /* Объявление переменной счетчика нажатий. Начально значение - 0 */
    uint16_t cnt = 0;

    /* Бесконечный цикл */
    while (1)
    {
        /* Проверка состояния кнопки */
        if (sb1_get_state() == SB_PRESSED)
        {
            /* Если кнопка нажата, то инкрементируем счетчик */
            cnt++;
            /* Передаем обновление значение счетчика на светодиоды */
            led_set(cnt);
        }
    }
}
