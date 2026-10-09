/* SPDX-License-Identifier: MPL-2.0 */
#pragma once
#include <stdint.h>
#ifdef COEX_HOST_TEST
/* Tests record barriers separately from each aligned 32-bit transaction. */
void coex_test_barrier(void);
uint32_t coex_test_read32(uint32_t address);
void coex_test_write32(uint32_t address, uint32_t value);
#define coex_barrier coex_test_barrier
#define coex_read_transaction coex_test_read32
#define coex_write_transaction coex_test_write32
#elif defined(__XTENSA__)
static inline void coex_barrier(void) { __asm__ volatile("memw" ::: "memory"); }
static inline uint32_t coex_read_transaction(uint32_t address) { return *(volatile uint32_t *)(uintptr_t)address; }
static inline void coex_write_transaction(uint32_t address, uint32_t value) { *(volatile uint32_t *)(uintptr_t)address = value; }
#else
#error "Select COEX_HOST_TEST for a simulated MMIO build; target MMIO requires Xtensa."
#endif
static inline uint32_t coex_read32(uint32_t address)
{ coex_barrier(); return coex_read_transaction(address); }
static inline void coex_write32(uint32_t address, uint32_t value)
{ coex_barrier(); coex_write_transaction(address, value); }
