/****************************************************************************
 * boards/arm/stm32h7/linum-stm32h753bi/src/linum-stm32h753bi.h
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

#ifndef __BOARDS_ARM_STM32H7_LINUM_STM32H753BI_SRC_LINUM_STM32H753BI_H
#define __BOARDS_ARM_STM32H7_LINUM_STM32H753BI_SRC_LINUM_STM32H753BI_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/compiler.h>

#include <stdint.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Configuration ************************************************************/

/* LED of board */

#define GPIO_LD1       (GPIO_OUTPUT|GPIO_PUSHPULL|GPIO_SPEED_50MHz | \
                        GPIO_OUTPUT_SET|GPIO_PORTG|GPIO_PIN2)
#define GPIO_LD2       (GPIO_OUTPUT|GPIO_PUSHPULL|GPIO_SPEED_50MHz | \
                        GPIO_OUTPUT_SET|GPIO_PORTG|GPIO_PIN3)
#define GPIO_LD3       (GPIO_OUTPUT|GPIO_PUSHPULL|GPIO_SPEED_50MHz | \
                        GPIO_OUTPUT_SET|GPIO_PORTB|GPIO_PIN2)

#define GPIO_LED_RED     GPIO_LD1
#define GPIO_LED_GREEN   GPIO_LD2
#define GPIO_LED_BLUE    GPIO_LD3

/* Check if we can support the RTC driver */

#define HAVE_RTC_DRIVER 1
#if !defined(CONFIG_RTC) || !defined(CONFIG_RTC_DRIVER)
#  undef HAVE_RTC_DRIVER
#endif

/* USB OTG FS
 *
 * PA9   OTG_FS_VBUS VBUS sensing
 * PI12  OTG_FS_PowerSwitchOn
 * PI13  OTG_FS_Overcurrent
 */

#define GPIO_OTGFS_VBUS   (GPIO_INPUT|GPIO_FLOAT|GPIO_SPEED_100MHz| \
                           GPIO_OPENDRAIN|GPIO_PORTA|GPIO_PIN9)

#define GPIO_OTGFS_PWRON  (GPIO_OUTPUT|GPIO_FLOAT|GPIO_SPEED_100MHz|  \
                           GPIO_PUSHPULL|GPIO_PORTI|GPIO_PIN12)

#ifdef CONFIG_USBHOST
#  define GPIO_OTGFS_OVER (GPIO_INPUT|GPIO_EXTI|GPIO_FLOAT| \
                           GPIO_SPEED_100MHz|GPIO_PUSHPULL| \
                           GPIO_PORTI|GPIO_PIN13)
#else
#  define GPIO_OTGFS_OVER (GPIO_INPUT|GPIO_FLOAT|GPIO_SPEED_100MHz| \
                           GPIO_PUSHPULL|GPIO_PORTI|GPIO_PIN13)
#endif

/* SD Card
 *
 * PG7  Card detected pin
 * PD7  Enable power supply SD Card pin
 */

#if defined(CONFIG_STM32H7_SDMMC1)
#  define HAVE_SDIO
#endif

#if defined(CONFIG_DISABLE_MOUNTPOINT) || !defined(CONFIG_MMCSD_SDIO)
#  undef HAVE_SDIO
#endif

#define GPIO_SDIO_NCD     (GPIO_INPUT|GPIO_FLOAT|GPIO_EXTI|GPIO_PORTG|GPIO_PIN7) /* PG7 */
#define GPIO_SD1_PWR_EN_N (GPIO_OUTPUT|GPIO_PUSHPULL|GPIO_SPEED_50MHz| \
                           GPIO_OUTPUT_SET|GPIO_PORTD|GPIO_PIN7)                 /* PD7 */

#define SDIO_SLOTNO        0
#define SDIO_MINOR         0

/* PWM */

#define BUZZER_PWMTIMER    4

/* OneShot Timer */

#define BOARD_TONE_ONESHOT_TIM     17  /* Timer 17 - Oneshot timer for note timings */
#define BOARD_TONE_ONESHOT_TIM_RES 10  /* Timer 17 - Oneshot timer resolution (us)  */

/* Ethernet
 *
 * PI4  Reset PHY pin
 */

#define GPIO_ETH_RESET    (GPIO_OUTPUT|GPIO_PUSHPULL|GPIO_SPEED_100MHz|\
                           GPIO_OUTPUT_CLEAR|GPIO_PORTI|GPIO_PIN4)       /* PI4 */

/* Quadrature Encoder
 *
 * Use Timer 5 (TIM3) on channels 2 and 2 for QEncoder, using PB4 and PA7.
 */

#define LINUMSTM32H753BI_QETIMER 5

/* DISP_RESET */

#define GPIO_LCD_DISP      (GPIO_OUTPUT|GPIO_PUSHPULL|GPIO_SPEED_50MHz|\
                            GPIO_OUTPUT_SET|GPIO_PORTI|GPIO_PIN7)

/* DISP_PWM */

#define GPIO_LCD_BL        (GPIO_OUTPUT|GPIO_PUSHPULL|GPIO_SPEED_50MHz|\
                            GPIO_OUTPUT_SET|GPIO_PORTH|GPIO_PIN6)

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_bringup
 *
 * Description:
 *   Perform architecture-specific initialization
 *
 *   CONFIG_BOARD_LATE_INITIALIZE=y :
 *     Called from board_late_initialize().
 *
 *   CONFIG_BOARD_LATE_INITIALIZE=n && CONFIG_BOARDCTL=y &&
 *   CONFIG_NSH_ARCHINIT:
 *     Called from the NSH library
 *
 ****************************************************************************/

int stm32_bringup(void);

/****************************************************************************
 * Name: stm32_usbinitialize
 *
 * Description:
 *   Called from stm32_usbinitialize very early in initialization to setup
 *   USB-related GPIO pins for the LINUM-STM32H753BI board.
 *
 ****************************************************************************/

#ifdef CONFIG_STM32H7_OTGFS
void weak_function stm32_usbinitialize(void);
#endif

/****************************************************************************
 * Name: stm32_dma_alloc_init
 *
 * Description:
 *   Called to create a FAT DMA allocator.
 *
 * Returned Value:
 *   0 on success or -ENOMEM
 *
 ****************************************************************************/

#if defined (CONFIG_FAT_DMAMEMORY)
int stm32_dma_alloc_init(void);
#endif

/****************************************************************************
 * Name: stm32_sdio_initialize
 *
 * Description:
 *   Initialize SDIO-based MMC/SD card support.
 *
 ****************************************************************************/

#ifdef HAVE_SDIO
int stm32_sdio_initialize(void);
#endif

/****************************************************************************
 * Name: stm32_at24_init
 *
 * Description:
 *   Initialize and register the EEPROM for 24XX driver.
 *
 ****************************************************************************/

#ifdef CONFIG_I2C_EE_24XX
int stm32_at24_init(char *path);
#endif

/****************************************************************************
 * Name: stm32_pwm_setup
 *
 * Description:
 *   Initialize PWM and register the PWM device.
 *
 ****************************************************************************/

#ifdef CONFIG_PWM
int stm32_pwm_setup(void);
#endif

/****************************************************************************
 * Name: stm32_spidev_initialize
 *
 * Description:
 *   Called to configure SPI chip select GPIO pins.
 *
 ****************************************************************************/

#ifdef CONFIG_STM32H7_SPI
void stm32_spidev_initialize(void);
#endif

/****************************************************************************
 * Name: stm32_n25qxxx_setup
 *
 * Description:
 *   Initialize and register the FLash for N25QXXX driver.
 *
 ****************************************************************************/

#ifdef CONFIG_MTD_W25QXXXJV
int stm32_w25qxxx_setup(void);
#endif

/****************************************************************************
 * Name: board_qencoder_initialize
 *
 * Description:
 *   Initialize the quadrature encoder driver for the given timer
 *
 ****************************************************************************/

int board_qencoder_initialize(int devno, int timerno);

/****************************************************************************
 * Name: stm32_mfrc522initialize
 *
 * Description:
 *   Initialize and register the MFRC522 RFID driver.
 *
 * Input Parameters:
 *   devpath - The full path to the driver to register. E.g., "/dev/rfid0"
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 ****************************************************************************/

#ifdef CONFIG_CL_MFRC522
int stm32_mfrc522initialize(const char *devpath);
#endif

/****************************************************************************
 * Name: board_tone_initialize
 *
 * Input Parameters:
 *   devno - The device number, used to build the device path as /dev/toneN
 *
 * Description:
 *   Configure and initialize the tone generator.
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_TONE
int board_tone_initialize(int devno);
#endif

/****************************************************************************
 * Name: stm32_tsc_setup
 *
 * Description:
 *   This function is called by board-bringup logic to configure the
 *   touchscreen device.  This function will register the driver as
 *   /dev/inputN where N is the minor device number.
 *
 * Input Parameters:
 *   minor   - The input device minor number
 *
 * Returned Value:
 *   Zero is returned on success.  Otherwise, a negated errno value is
 *   returned to indicate the nature of the failure.
 *
 ****************************************************************************/

#ifdef CONFIG_INPUT_FT5X06
int stm32_tsc_setup(int minor);
#endif

#endif /* __BOARDS_ARM_STM32H7_LINUM_STM32H753BI_SRC_LINUM_STM32H753BI_H */
