/*
 * Copyright (c) 2001, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 * $Id: tapdev.c,v 1.8 2006/06/07 08:39:58 adam Exp $
 */

#include "tapdev.h"
#include "uip_arp.h"

#if (defined ENC28J60_UIP_ENABLE)
    #include "enc28j60_uip.h"
#elif (defined STM32_ETH_UIP_ENABLE)
    #include "stm32_eth_uip.h"
#endif



/*---------------------------------------------------------------------------*/
void tapdev_init(void)
{
#if (defined ENC28J60_UIP_ENABLE)

    enc28j60_port_init();
    enc28j60_init(uip_ethaddr);

#elif (defined STM32_ETH_UIP_ENABLE)

    stm32_eth_init(uip_ethaddr);

#endif
}

/*---------------------------------------------------------------------------*/
uint16_t tapdev_read(void)
{
#if (defined ENC28J60_UIP_ENABLE)

    return enc28j60_rx(uip_buf, UIP_BUFSIZE);

#elif (defined STM32_ETH_UIP_ENABLE)

    return ETH_HandleRxPkt(uip_buf);

#else

    return 0;

#endif
}

/*---------------------------------------------------------------------------*/
void tapdev_send(void)
{
#if (defined ENC28J60_UIP_ENABLE)

    enc28j60_tx(uip_buf, uip_len);

#elif (defined STM32_ETH_UIP_ENABLE)

    ETH_HandleTxPkt(uip_buf,uip_len);

#endif // ENC28J60_UIP_ENABLE
}

/*---------------------------------------------------------------------------*/
// 返回值: 0-未连接, 1-已连接.
uint8_t tapdev_linked(void)
{
#if (defined ENC28J60_UIP_ENABLE)

    return enc28j60_linked();

#elif (defined STM32_ETH_UIP_ENABLE)

    return stm32_eth_linked();

#endif
}

/*---------------------------------------------------------------------------*/
#if UIP_LOGGING == 1
void uip_log(char *msg)
{
    assert_printf("uip_log> %s\r\n", msg);
}
#endif
