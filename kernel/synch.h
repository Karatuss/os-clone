#ifndef KERNEL_SYNCH_H_
#define KERNEL_SYNCH_H_

#include "stdint.h"

typedef struct KernelMutex_t {
    uint32_t owner;
    bool lock;
} KernelMutex_t;

void Kernel_sem_init(int32_t max);
bool Kernel_sem_test(void);
void Kernel_sem_release(void);

void Kernel_mutex_init(void);
bool Kernel_mutex_lock(uint32_t owner);
bool Kernel_mutex_unlock(uint32_t owner);

/* [spinlock (busy waiting)] */
void spin_lock(void);
void spin_unlock(void);

#endif /* KERNEL_SYNCH_H_ */