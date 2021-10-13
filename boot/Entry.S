#include "ARMv7AR.h"
#include "MemoryMap.h"

.text
    .code 32

    .global vector_start
    .global vector_end

    vector_start:
        ldr pc, reset_handler_addr
        ldr pc, undef_handler_addr
        ldr pc, svc_handler_addr
        ldr pc, pftch_abt_handler_addr
        ldr pc, data_abt_handler_addr
        b .
        ldr pc, irq_abt_handler_addr
        ldr pc, fiq_abt_handler_addr

        reset_handler_addr:     .word reset_handler
        undef_handler_addr:     .word dummy_handler
        svc_handler_addr:       .word dummy_handler
        pftch_abt_handler_addr: .word dummy_handler
        data_abt_handler_addr:  .word dummy_handler
        irq_abt_handler_addr:   .word Irq_Handler
        fiq_abt_handler_addr:   .word Fiq_Handler

    vector_end:
        
    reset_handler:
        mrs r0, cpsr
        bic r1, r0, #0x1f
        orr r1, r1, # ARM_MODE_BIT_SVC
        msr cpsr, r1
        ldr sp, =SVC_STACK_TOP

        mrs r0, cpsr
        bic r1, r0, #0x1f
        orr r1, r1, # ARM_MODE_BIT_IRQ
        msr cpsr, r1
        ldr sp, =FIQ_STACK_TOP

        mrs r0, cpsr
        bic r1, r0, #0x1f
        orr r1, r1, # ARM_MODE_BIT_FIQ
        msr cpsr, r1
        ldr sp, =IRQ_STACK_TOP

        mrs r0, cpsr
        bic r1, r0, #0x1f
        orr r1, r1, # ARM_MODE_BIT_ABT
        msr cpsr, r1
        ldr sp, =ABT_STACK_TOP

        mrs r0, cpsr
        bic r1, r0, #0x1f
        orr r1, r1, # ARM_MODE_BIT_UND
        msr cpsr, r1
        ldr sp, =UND_STACK_TOP

        mrs r0, cpsr
        bic r1, r0, #0x1f
        orr r1, r1, # ARM_MODE_BIT_SYS
        msr cpsr, r1
        ldr sp, =USRSYS_STACK_TOP

        bl  main

    dummy_handler:
        b .
.end