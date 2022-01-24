#include "stdbool.h"
#include "stdio.h"
#include "synch.h"

#define DEF_SEM_MAX 8

static int32_t sSemMax;
static int32_t sSem;

/* If a developer needs to use multiple mutexs, just make them array. */
static KernelMutex_t sMutex;

static bool sSpinLock = false;

/* Semaphore functions */

/**
 * @brief Initialize the max count of semaphore.
 * 
 * @param max The count of semaphore. Scope: [1~8]
 */
void Kernel_sem_init(int32_t max)
{
    sSemMax = (max <= 0) ? DEF_SEM_MAX : max;
    sSemMax = (max >= DEF_SEM_MAX) ? DEF_SEM_MAX : max;

    sSem = sSemMax;
}

bool Kernel_sem_test(void)
{
    if (sSem <= 0)
        return false;

    sSem--;

    return true;
}

void Kernel_sem_release(void)
{
    sSem++;

    if (sSem >= sSemMax)
        sSem = sSemMax;
}

/* Mutex functions */

void Kernel_mutex_init(void)
{
    sMutex.owner = 0;
    sMutex.lock = false;
}

bool Kernel_mutex_lock(uint32_t owner)
{
    if (sMutex.lock)
        return false;

    sMutex.owner = owner;
    sMutex.lock = true;

    return true;
}

bool Kernel_mutex_unlock(uint32_t owner)
{
    if (!sMutex.lock) {
        debug_printf("\n[Error: Kernel_mutex_unlock] Not locked.\n");
        return false;
    }

    if (sMutex.owner != owner) {
        debug_printf("\n[Error: Kernel_mutex_unlock] Lock owner not matched up.\n");
        return false;
    }

    sMutex.owner = 0;       // prevent ex-owner's info from leaking others
    sMutex.lock = false;    // turn to false for unlocking.

    return true;
}

/**
 * [spinlock (busy waiting)] 
 * We cannot implement and use this acutally
 * because the RealViewPB in QEMU is emulated with only SINGLE CORE.
 * 
 * It is useless to be implemented here,
 * but let's see how it looks like.
 */
void spin_lock(void)
{
    while (sSpinLock);
    sSpinLock = true;
}

void spin_unlock(void)
{
    sSpinLock = false;
}