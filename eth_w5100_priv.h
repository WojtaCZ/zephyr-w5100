/* W5100 Stand-alone Ethernet Controller with SPI
 *
 * Copyright (c) 2023 Vojtěch Vosáhlo
 * Author: Vojtěch Vosáhlo <https://vosahlov.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>

#ifndef _W5100_
#define _W5100_

/*
 * W5100 common registers
 */
#define W5100_COMMON_REGS	0x0000
#define W5100_MR		0x0000 /* Mode Register */
#define W5100_GW		0x0001
#define MR_RST			0x80 /* S/W reset */
#define MR_PB			0x10 /* Ping block */
#define MR_AI			0x02 /* Address Auto-Increment */
#define MR_IND			0x01 /* Indirect mode */
#define W5100_SHAR		0x0009 /* Source MAC address */
#define W5100_IR		0x0015 /* Interrupt Register */
#define W5100_COMMON_REGS_LEN	0x0040

#define W5100_Sn_MR		0x0000 /* Sn Mode Register */
#define W5100_Sn_CR		0x0001 /* Sn Command Register */
#define W5100_Sn_IR		0x0002 /* Sn Interrupt Register */
#define W5100_Sn_SR		0x0003 /* Sn Status Register */
#define W5100_Sn_TX_FSR		0x0020 /* Sn Transmit free memory size */
#define W5100_Sn_TX_RD		0x0022 /* Sn Transmit memory read pointer */
#define W5100_Sn_TX_WR		0x0024 /* Sn Transmit memory write pointer */
#define W5100_Sn_RX_RSR		0x0026 /* Sn Receive free memory size */
#define W5100_Sn_RX_RD		0x0028 /* Sn Receive memory read pointer */
#define W5100_Sn_IMR		0x002C /* Sn Interrupt Mask Register */


#define W5100_S0_REGS		0x0400

#define W5100_S0_MR		(W5100_S0_REGS + W5100_Sn_MR)
#define S0_MR_MACRAW		0x04 /* MAC RAW mode */
#define S0_MR_MF		0x40 /* MAC Filter for W5100 */
#define W5100_S0_CR		(W5100_S0_REGS + W5100_Sn_CR)
#define S0_CR_OPEN		0x01 /* OPEN command */
#define S0_CR_CLOSE		0x10 /* CLOSE command */
#define S0_CR_SEND		0x20 /* SEND command */
#define S0_CR_RECV		0x40 /* RECV command */
#define W5100_S0_IR		(W5100_S0_REGS + W5100_Sn_IR)
#define S0_IR_SENDOK		0x10 /* complete sending */
#define S0_IR_RECV		0x04 /* receiving data */
#define W5100_S0_SR		(W5100_S0_REGS + W5100_Sn_SR)
#define S0_SR_MACRAW		0x42 /* mac raw mode */
#define W5100_S0_TX_FSR		(W5100_S0_REGS + W5100_Sn_TX_FSR)
#define W5100_S0_TX_RD		(W5100_S0_REGS + W5100_Sn_TX_RD)
#define W5100_S0_TX_WR		(W5100_S0_REGS + W5100_Sn_TX_WR)
#define W5100_S0_RX_RSR		(W5100_S0_REGS + W5100_Sn_RX_RSR)
#define W5100_S0_RX_RD		(W5100_S0_REGS + W5100_Sn_RX_RD)
#define W5100_S0_IMR		(W5100_S0_REGS + W5100_Sn_IMR)

#define W5100_S0_MR_MF		7 /* MAC Filter for W5100 */
#define W5100_Sn_REGS_LEN	0x0040
#define W5100_IMR		0x0016 /* Interrupt Mask Register */
#define IR_S0			0x01
#define RTR_DEFAULT		2000
#define W5100_RTR		0x0017 /* Retry Time-value Register */ 


#define W5100_Sn_RXMEM_SIZE(n)	\
		(0x041e + (n) * 0x0100) /* Sn RX Memory Size */
#define W5100_Sn_TXMEM_SIZE(n)	\
		(0x041f + (n) * 0x0100) /* Sn TX Memory Size */

#define W5100_Sn_TX_MEM_START	0x4000
#define W5100_TX_MEM_SIZE	0x2000
#define W5100_Sn_RX_MEM_START	0x6000
#define W5100_RX_MEM_SIZE	0x2000

/* Delay for PHY write/read operations (25.6 us) */
#define W5100_PHY_ACCESS_DELAY		26U
struct w5100_config {
	struct spi_dt_spec spi;
	struct gpio_dt_spec interrupt;
	struct gpio_dt_spec reset;
	void (*config_func)(void);
	uint8_t full_duplex;
	int32_t timeout;
};

struct w5100_runtime {
	struct net_if *iface;

	K_THREAD_STACK_MEMBER(thread_stack,
			      CONFIG_ETH_W5100_RX_THREAD_STACK_SIZE);
	struct k_thread thread;
	uint8_t mac_addr[6];
	struct gpio_callback gpio_cb;
	struct k_sem tx_sem;
	struct k_sem int_sem;
	void (*generate_mac)(uint8_t *mac);
	uint8_t buf[NET_ETH_MAX_FRAME_SIZE];
};

#endif /*_W5100_*/
