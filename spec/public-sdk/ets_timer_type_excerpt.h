/*
 * SPDX-FileCopyrightText: 2010-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/* Declaration excerpt selected by supervisor from ESP-IDF 6.0.3 S3 rom/ets_sys.h. */
#include <stdint.h>
typedef void ETSTimerFunc(void *timer_arg);/**< timer handler*/

typedef struct _ETSTIMER_ {
    struct _ETSTIMER_    *timer_next;   /**< timer linker*/
    uint32_t              timer_expire; /**< abstract time when timer expire*/
    uint32_t              timer_period; /**< timer period, 0 means timer is not periodic repeated*/
    ETSTimerFunc         *timer_func;   /**< timer handler*/
    void                 *timer_arg;    /**< timer handler argument*/
} ETSTimer;
