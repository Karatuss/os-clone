#include "stdint.h"
#include "stdbool.h"

#include "ARMv7AR.h"
#include "MemoryMap.h"
#include "task.h"

/* Context Switching */
static KernelTcb_t *sCurrent_tcb;
static KernelTcb_t *sNext_tcb;

static KernelTcb_t  sTask_list[MAX_TASK_NUM];
static uint32_t     sAllocated_tcb_index;       // indicating of the allocated last index
static uint32_t     sCurrent_tcb_index;

static KernelTcb_t  *Scheduler_round_robin_algorithm(void);
static void Save_context(void);
static void Restore_context(void);

void Kernel_task_init(void)
{
    sAllocated_tcb_index = 0;
    sCurrent_tcb_index = 0;     // init 0 for scheduling

    // Divide the stack into MAX_TASK_NUM pieces
    for (uint32_t i = 0; i < MAX_TASK_NUM; i++) {
        sTask_list[i].stack_base = (uint8_t*)(TASK_STACK_START + (i * USR_TASK_STACK_SIZE));
        sTask_list[i].sp         = (uint32_t)sTask_list[i].stack_base + USR_TASK_STACK_SIZE - 4;

        // save a KernelTaskContext_t below the sp
        sTask_list[i].sp -= sizeof(KernelTaskContext_t);
        KernelTaskContext_t *ctx = (KernelTaskContext_t*)sTask_list[i].sp;
        ctx->pc = 0;
        ctx->spsr = ARM_MODE_BIT_SYS;   // set to SYS mode
    }
}

uint32_t Kernel_task_create(KernelTaskFunc_t startFunc)
{
    KernelTcb_t *new_tcb = &sTask_list[sAllocated_tcb_index++];

    // error control: check it is over MAX_TASK_NUM
    if (sAllocated_tcb_index > MAX_TASK_NUM)
        return NOT_ENOUGH_TASK_NUM;

    KernelTaskContext_t *ctx = (KernelTaskContext_t*)new_tcb->sp;    
    ctx->pc = (uint32_t)startFunc;

    return (sAllocated_tcb_index - 1);
}

/**
 * Use only once when the kernel starts.
 */
void Kernel_task_start(void)
{
    sNext_tcb = &sTask_list[sCurrent_tcb_index];
    Restore_context();
}

void Kernel_task_scheduler(void)
{
    sCurrent_tcb = &sTask_list[sCurrent_tcb_index];
    sNext_tcb = Scheduler_round_robin_algorithm();

    Kernel_task_context_switching();
}

/**
 * __attiribute__((naked))
 * : this directive is for forbidding making
 *   a compiler insert a stack restoration code back and forth.
 * 
 *   The reason why it needs for context switching is that
 *   we DO NOT want to change the LR(Link Register).
 */
__attribute__((naked))
void Kernel_task_context_switching(void)
{
    __asm__("B Save_context");
    __asm__("B Restore_context");
}

__attribute__((naked))
static void Save_context(void)
{
    // save current task context into the current task stack
    __asm__("push  {lr}");
    __asm__("push  {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}");
    __asm__("mrs   r0, cpsr");
    __asm__("push  {r0}");
    // save current task stack pointer(sp) into the current TCB
    __asm__("ldr   r0, =sCurrent_tcb");
    __asm__("ldr   r0, [r0]");
    __asm__("stmia r0!, {sp}");     // stmia (Store Multiple Increment After)
}

/**
 * @brief Just do the exactly reversed things of 'Save_context'
 */
__attribute__((naked))
static void Restore_context(void)
{
    // restore next task stack pointer(sp) from the next TCB
    __asm__("ldr   r0, =sNext_tcb");
    __asm__("ldr   r0, [r0]");
    __asm__("ldmia r0!, {sp}");
    // restore next task context from the next task stack
    __asm__("pop   {r0}");
    __asm__("msr   cpsr, r0");
    __asm__("pop   {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}");
    __asm__("pop   {pc}");
}

static KernelTcb_t *Scheduler_round_robin_algorithm(void)
{
    sCurrent_tcb_index++;
    sCurrent_tcb_index %= sAllocated_tcb_index;

    return &sTask_list[sCurrent_tcb_index];
}