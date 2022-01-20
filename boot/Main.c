#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"

#include "HalUart.h"
#include "HalInterrupt.h"
#include "HalTimer.h"

#include "Kernel.h"
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
    // Timer_test();    // I'll activate this back when the timer device works normally.

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
    debug_printf("enter's ascii code: %u\n", '\n');
}

// not working now
// static void Timer_test(void)
// {
//     while (true) {
//         debug_printf("current count: %u\n", Hal_timer_get_1ms_counter());
//         delay(100);
//     }
// }

static void Kernel_init(void)
{
    uint32_t taskId;

    Kernel_task_init();
    Kernel_event_flag_init();

    taskId = Kernel_task_create(User_task0);
    if (NOT_ENOUGH_TASK_NUM == taskId)
        putstr("Task0 creation failed\n");

    taskId = Kernel_task_create(User_task1);
    if (NOT_ENOUGH_TASK_NUM == taskId)
        putstr("Task1 creation failed\n");

    taskId = Kernel_task_create(User_task2);
    if (NOT_ENOUGH_TASK_NUM == taskId)
        putstr("Task2 creation failed\n");

    Kernel_start();
}

void User_task0(void)
{
    // TEST_USER_TASK(0);
    uint32_t local = 0;
    debug_printf("User task #0 SP=0x%x\n", &local);

    uint8_t  cmdBuf[16] = {0};
    uint32_t cmdBufIdx = 0;
    uint8_t  uartch = 0;

    while (true) {
        KernelEventFlag_t handle_event = Kernel_wait_events(KernelEventFlag_UartIn|KernelEventFlag_CmdOut);

        switch (handle_event) {
        case KernelEventFlag_UartIn:
            // debug_printf("\nEvent handled by Task0\n");
            // Kernel_send_events(KernelEventFlag_CmdIn);

            Kernel_recv_msg(KernelMsgQ_Task0, &uartch, 1);

            // debug_printf("\nenter's ascii code: %u\n", (uint32_t)uartch);         // teeeeeeeeeeeest coooooooooooooodeeeeeeeeeeeee!

            if (uartch == '\r') {   // check the input is enter key.
                cmdBuf[cmdBufIdx] = '\0';

                Kernel_send_msg(KernelMsgQ_Task1, &cmdBufIdx, 1);
                Kernel_send_msg(KernelMsgQ_Task1, cmdBuf, cmdBufIdx);
                Kernel_send_events(KernelEventFlag_CmdIn);
                // debug_printf("I'm here!------------------------\n");

                cmdBufIdx = 0;
            } else {
                cmdBuf[cmdBufIdx++] = uartch;
                cmdBufIdx %= 16;
                // debug_printf("\ncmdBuf: %s\n", cmdBuf);
            }
            break;
        case KernelEventFlag_CmdOut:
            debug_printf("\nCmdOut Event by Task0\n");
            break;
        }
        Kernel_yield();
    }
}

void User_task1(void)
{
    // TEST_USER_TASK(1);
    uint32_t local = 0;

    debug_printf("User task #1 SP=0x%x\n", &local);

    uint8_t cmdlen = 0;
    uint8_t cmd[16] = {0};

    while (true) {
        KernelEventFlag_t handle_event = Kernel_wait_events(KernelEventFlag_CmdIn);
        switch (handle_event) {
        case KernelEventFlag_CmdIn:
            memclr(cmd, 16);
            Kernel_recv_msg(KernelMsgQ_Task1, &cmdlen, 1);
            Kernel_recv_msg(KernelMsgQ_Task1, cmd, cmdlen);
            // debug_printf("\nEvent handled by Task1\n");
            debug_printf("\n[Task1] Recv Cmd: %s\n", cmd);
            break;
        }
        Kernel_yield();
    }
}

void User_task2(void)
{
    // TEST_USER_TASK(2);
    uint32_t local = 0;

    debug_printf("User task #2 SP=0x%x\n", &local);

    while (true) {
        Kernel_yield();
    }
}