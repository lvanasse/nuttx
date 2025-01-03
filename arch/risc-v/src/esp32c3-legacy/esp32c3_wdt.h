/****************************************************************************
 * arch/risc-v/src/esp32c3-legacy/esp32c3_wdt.h
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#ifndef __ARCH_RISCV_SRC_ESP32C3_LEGACY_ESP32C3_WDT_H
#define __ARCH_RISCV_SRC_ESP32C3_LEGACY_ESP32C3_WDT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdint.h>
#include <nuttx/irq.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Helpers ******************************************************************/

#define ESP32C3_WDT_START(d)                      ((d)->ops->start(d))
#define ESP32C3_WDT_STOP(d)                       ((d)->ops->stop(d))
#define ESP32C3_WDT_LOCK(d)                       ((d)->ops->enablewp(d))
#define ESP32C3_WDT_UNLOCK(d)                     ((d)->ops->disablewp(d))
#define ESP32C3_MWDT_PRE(d, v)                    ((d)->ops->pre(d, v))
#define ESP32C3_WDT_STO(d, v, s)                  ((d)->ops->settimeout(d, v, s))
#define ESP32C3_WDT_FEED(d)                       ((d)->ops->feed(d))
#define ESP32C3_WDT_STG_CONF(d, s, c)             ((d)->ops->stg_conf(d, s, c))
#define ESP32C3_MWDT_UPD_CONF(d)                  ((d)->ops->upd_conf(d))
#define ESP32C3_RWDT_CLK(d)                       ((d)->ops->rtc_clk(d))
#define ESP32C3_WDT_SETISR(d, hnd, arg)           ((d)->ops->setisr(d, hnd, arg))
#define ESP32C3_WDT_ENABLEINT(d)                  ((d)->ops->enableint(d))
#define ESP32C3_WDT_DISABLEINT(d)                 ((d)->ops->disableint(d))
#define ESP32C3_WDT_ACKINT(d)                     ((d)->ops->ackint(d))

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Instances of Watchdog Timer  */

enum esp32c3_wdt_inst_e
{
  ESP32C3_WDT_MWDT0 = 0,  /* Main System Watchdog Timer (MWDT) of Timer Group 0 */
  ESP32C3_WDT_MWDT1,      /* Main System Watchdog Timer (MWDT) of Timer Group 1 */
  ESP32C3_WDT_RWDT        /* RTC Watchdog Timer (RWDT) */
};

/* Stages of a Watchdog Timer. A WDT has 4 stages. */

enum esp32c3_wdt_stage_e
{
  ESP32C3_WDT_STAGE0 = 0,     /* Stage 0 */
  ESP32C3_WDT_STAGE1 = 1,     /* Stage 1 */
  ESP32C3_WDT_STAGE2 = 2,     /* Stage 2 */
  ESP32C3_WDT_STAGE3 = 3      /* Stage 3 */
};

/* Behavior of the WDT stage if it times out.
 *
 * Note: These enum values should be compatible with the
 *       corresponding register field values.
 */

enum esp32c3_wdt_stage_action_e
{
  ESP32C3_WDT_STAGE_ACTION_OFF = 0,           /* Disabled. This stage will have no effects on the system. */
  ESP32C3_WDT_STAGE_ACTION_INT = 1,           /* Trigger an interrupt when the stage expires. */
  ESP32C3_WDT_STAGE_ACTION_RESET_CPU = 2,     /* Reset a CPU core when the stage expires. */
  ESP32C3_WDT_STAGE_ACTION_RESET_SYSTEM = 3,  /* Reset the main system when the stage expires.
                                               * This includes the CPU and all peripherals.
                                               * The RTC is an exception and will not be reset.
                                               */
  ESP32C3_WDT_STAGE_ACTION_RESET_RTC = 4      /* Reset the main system and the RTC when the stage expires.
                                               * ONLY AVAILABLE FOR RWDT.
                                               */
};

/* ESP32-C3 WDT device */

struct esp32c3_wdt_dev_s
{
  struct esp32c3_wdt_ops_s *ops;
};

/* ESP32-C3 WDT ops */

/* This is a struct containing the pointers to the wdt operations */

struct esp32c3_wdt_ops_s
{
  /* WDT tasks */

  void (*start)(struct esp32c3_wdt_dev_s *dev);
  void (*stop)(struct esp32c3_wdt_dev_s *dev);

  /* WDT configuration */

  void (*enablewp)(struct esp32c3_wdt_dev_s *dev);
  void (*disablewp)(struct esp32c3_wdt_dev_s *dev);
  void (*pre)(struct esp32c3_wdt_dev_s *dev, uint16_t value);
  int32_t (*settimeout)(struct esp32c3_wdt_dev_s *dev,
                        uint32_t value,
                        enum esp32c3_wdt_stage_e stage);
  void (*feed)(struct esp32c3_wdt_dev_s *dev);
  int32_t (*stg_conf)(struct esp32c3_wdt_dev_s *dev,
                      enum esp32c3_wdt_stage_e stage,
                      enum esp32c3_wdt_stage_action_e conf);
  void (*upd_conf)(struct esp32c3_wdt_dev_s *dev);
  uint16_t (*rtc_clk)(struct esp32c3_wdt_dev_s *dev);

  /* WDT interrupts */

  int32_t (*setisr)(struct esp32c3_wdt_dev_s *dev, xcpt_t handler,
                     void * arg);
  void (*enableint)(struct esp32c3_wdt_dev_s *dev);
  void (*disableint)(struct esp32c3_wdt_dev_s *dev);
  void (*ackint)(struct esp32c3_wdt_dev_s *dev);
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

struct esp32c3_wdt_dev_s *esp32c3_wdt_init(enum esp32c3_wdt_inst_e wdt_id);
void esp32c3_wdt_early_deinit(void);
void esp32c3_wdt_deinit(struct esp32c3_wdt_dev_s *dev);
bool esp32c3_wdt_is_running(struct esp32c3_wdt_dev_s *dev);

#endif /* __ARCH_RISCV_SRC_ESP32C3_LEGACY_ESP32C3_WDT_H */
