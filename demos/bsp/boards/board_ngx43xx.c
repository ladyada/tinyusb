/*
 * board_ngx4330.c
 *
 *  Created on: Dec 4, 2012
 *      Author: hathach
 */

/*
 * Software License Agreement (BSD License)
 * Copyright (c) 2012, hathach (tinyusb.net)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the tiny usb stack.
 */

#include "board.h"

#if BOARD == BOARD_NGX43XX

#include "lpc43xx_uart.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx_timer.h"
#include "lpc43xx_i2c.h"
#include "lpc43xx_gpdma.h"
#include "lpc43xx_i2s.h"
#include "lpc43xx_emc.h"

#define BOARD_MAX_LEDS  2
const static struct {
  uint8_t port;
  uint8_t pin;
}leds[BOARD_MAX_LEDS] = { {1, 11}, {1,12} };

void board_init(void)
{
  CGU_Init();
	SysTick_Config( CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE)/1000 );	/* 1 ms Timer */

	/* Turn on 5V USB VBUS TODO Should be Host-only */
	scu_pinmux(0x2, 6, MD_PUP | MD_EZI, FUNC4);				// P2_6 USB1_PWR_EN, USB1 VBus function
	scu_pinmux(0x2, 5, MD_PLN | MD_EZI | MD_ZI, FUNC2);		// P2_5 USB1_VBUS, MUST CONFIGURE THIS SIGNAL FOR USB1 NORMAL OPERATION

	/* Turn on 5V USB VBUS TODO Should be Host-only */
#if 1 //(BOARD == BOARD_XPLORER)
	scu_pinmux(0x1, 7, MD_PUP | MD_EZI, FUNC4);				// P1_7 USB0_PWR_EN, USB0 VBus function Xplorer
#else
	scu_pinmux(0x2, 3, MD_PUP | MD_EZI, FUNC7);     		// P2_3 USB0_PWR_EN, USB0 VBus function Farnell
#endif

	// Leds Init
	uint8_t i;
	for (i=0; i<BOARD_MAX_LEDS; i++)
	{
	  scu_pinmux(leds[i].port, leds[i].pin, MD_PUP|MD_EZI|MD_ZI, FUNC0);
	  GPIO_SetDir(leds[i].port, BIT_(leds[i].pin), 1); // output
	}
}

void board_leds(uint32_t mask, uint32_t state)
{
  uint8_t i;
  for(i=0; i<BOARD_MAX_LEDS; i++)
  {
    if ( mask & BIT_(i) )
	{
      (mask & state) ? GPIO_SetValue(leds[i].port, BIT_(leds[i].pin)) : GPIO_ClearValue(leds[i].port, BIT_(leds[i].pin)) ;
	}
  }
}

#endif
