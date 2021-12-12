#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"

#include "HalUart.h"
#include "HalInterrupt.h"
#include "HalTimer.h"

#include "task.h"

static void Hw_init(void);
static void Kernel_init(void);

static void Printf_test(void);
static void Timer_test(void);

void User_task0(void);
void User_task1(void);
void User_task2(void);

int main()
{
    Hw_init();

    uint32_t i = 100;
    while (i--)
        Hal_uart_put_char('N');
    Hal_uart_put_char('\n');
    putstr("Hello World!\n");

    Printf_test();
    Timer_test();

    Kernel_init();

    while (true);
}

static void Hw_init(void)
{
    Hal_interrupt_init();
    Hal_uart_init();
    Hal_timer_init();
}

static void Printf_test(void)
{
    char *str = "printf pointer test";
    char *nullptr = 0;
    uint32_t i = 5;
    uint32_t *sysctrl0 = (uint32_t *)0x10001000;

    debug_printf("%s\n", "Hello printf");
    debug_printf("output string pointer: %s\n", str);
    debug_printf("%s is null pointer, %u number\n", nullptr, 10);
    debug_printf("%u = 5\n", i);
    debug_printf("dec=%u hex=%x\n", 0xff, 0xff);
    debug_printf("print zero %u, overflow %u\n\n", 0, -1);
    debug_printf("SYSCTRL0 %x\n", *sysctrl0);
}

// not working now
static void Timer_test(void)
{
    while (true) {
        debug_printf("current count: %u\n", Hal_timer_get_1ms_counter());
        delay(100);
    }
}

static void Kernel_init(void)
{
    uint32_t taskId;

    Kernel_task_init();

    taskId = Kernel_task_create(User_task0);
    if (NOT_ENOUGH_TASK_NUM == taskId)
        putstr("Task0 creation failed\n");

    taskId = Kernel_task_create(User_task1);
    if (NOT_ENOUGH_TASK_NUM == taskId)
        putstr("Task1 creation failed\n");

    taskId = Kernel_task_create(User_task2);
    if (NOT_ENOUGH_TASK_NUM == taskId)
        putstr("Task2 creation failed\n");
}

void User_task0(void)
{
    debug_printf("User Task #0\n");
    while (true);
}
void User_task1(void)
{
    debug_printf("User Task #1\n");
    while (true);
}
void User_task2(void)
{
    debug_printf("User Task #2\n");
    while (true);
}