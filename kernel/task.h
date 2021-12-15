#ifndef KERNEL_TASK_H_
#define KERNEL_TASK_H_

#include "stdint.h"
#include "MemoryMap.h"

#define NOT_ENOUGH_TASK_NUM     0xFFFFFFFF

#define USR_TASK_STACK_SIZE     0x100000
#define MAX_TASK_NUM            (TASK_STACK_SIZE / USR_TASK_STACK_SIZE)

#define TEST_USER_TASK(num)                                                                   \
        do {                                                                                  \
            uint32_t local = 0;                                                               \
                                                                                              \
            debug_printf("User Task ###num SP=0x%x\n", &local);                               \
                                                                                              \
            while (true) {                                                                    \
                KernelEventFlag_t handle_event = Kernel_wait_events(KernelEventFlag_UartIn);  \
                switch (handle_event) {                                                       \
                case KernelEventFlag_UartIn:                                                  \
                    debug_printf("\nEvent handled\n");                                        \
                    break;                                                                    \
                }                                                                             \
                Kernel_yield();                                                               \
            }                                                                                 \
        } while (0);
                                                                            
typedef struct KernelTaskContext_t {
    uint32_t spsr;
    uint32_t r0_r12[13];
    uint32_t pc;
} KernelTaskContext_t;

typedef struct KernelTcb_t {
    uint32_t sp;
    uint8_t *stack_base;
} KernelTcb_t;

typedef void (*KernelTaskFunc_t)(void);

void Kernel_task_init(void);
void Kernel_task_start(void);
void Kernel_task_scheduler(void);
void Kernel_task_context_switching(void);
uint32_t Kernel_task_create(KernelTaskFunc_t startFunc);

#endif  /* KERNEL_TASK_H_ */