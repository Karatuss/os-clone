#include "armcpu.h"

/**
 * 0x80 : 7th bit on cspr IRQ mask
 * 0x40 : 6th bit on FIQ mask
 */ 

void enable_irq(void)
{
    __asm__("push {r0, r1}");
    __asm__("mrs r0, cpsr");
    __asm__("bic r1, r0, #0x80");
    __asm__("msr cpsr, r1");
    __asm__("pop {r0, r1}");
}

void enable_fiq(void)
{
    __asm__("push {r0, r1}");
    __asm__("mrs r0, cpsr");
    __asm__("bic r1, r0, #0x40");
    __asm__("msr cpsr, r1");
    __asm__("pop {r0, r1}");
}

void disable_irq(void)
{
    __asm__("push {r0, r1}");
    __asm__("mrs r0, cpsr");
    __asm__("orr r1, r0, #0x80");
    __asm__("msr cpsr, r1");
    __asm__("pop {r0, r1}");
}

void disable_fiq(void)
{
    __asm__("push {r0, r1}");
    __asm__("mrs r0, cpsr");
    __asm__("orr r1, r0, #0x40");
    __asm__("msr cpsr, r1");
    __asm__("pop {r0, r1}");
}