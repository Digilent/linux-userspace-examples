/*
 * uart.c
 *
 * @date 2019/08/09
 * @author Cosmin Tanislav
 * @author Cristian Fatu
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <limits.h>

#include "uart.h"

int uart_start(struct UartDevice* dev, bool canonical) {
	struct termios *tty;
	int fd;
	int rc;

	fd = open(dev->name, O_RDWR | O_NOCTTY);
	if (fd < 0) {
		printf("%s: failed to open UART device\r\n", __func__);
		return fd;
	}

	tty = malloc(sizeof(*tty));
	if (!tty) {
		printf("%s: failed to allocate UART TTY instance\r\n", __func__);
		return -ENOMEM;
	}

	memset(tty, 0, sizeof(*tty));

	/*
	 * Set baud-rate.
	 */
	tty->c_cflag |= dev->rate;

    /* Ignore framing and parity errors in input. */
    tty->c_iflag |=  IGNPAR;

    /* Use 8-bit characters. This too may affect standard streams,
     * but any sane C library can deal with 8-bit characters. */
    tty->c_cflag |=  CS8;

    /* Enable receiver. */
    tty->c_cflag |=  CREAD;

    if (canonical) {
        /* Enable canonical mode.
         * This is the most important bit, as it enables line buffering etc. */
        tty->c_lflag |= ICANON;
    } else {
        /* To maintain best compatibility with normal behaviour of terminals,
         * we set TIME=0 and MAX=1 in noncanonical mode. This means that
         * read() will block until at least one byte is available. */
        tty->c_cc[VTIME] = 0;
        tty->c_cc[VMIN] = 1;
    }

	/*
	 * Flush port.
	 */
	tcflush(fd, TCIFLUSH);

	/*
	 * Apply attributes.
	 */
	rc = tcsetattr(fd, TCSANOW, tty);
	if (rc) {
		printf("%s: failed to set attributes\r\n", __func__);
		return rc;
	}

	dev->fd = fd;
	dev->tty = tty;

	return 0;
}

int uart_writen(struct UartDevice* dev, char *buf, size_t buf_len) {
	return write(dev->fd, buf, buf_len);
}

int uart_writes(struct UartDevice* dev, char *string) {
	size_t len = strlen(string);
	return uart_writen(dev, string, len);
}

int uart_readn(struct UartDevice* dev, char *buf, size_t buf_len) {
	size_t received;

	received = read(dev->fd, buf, buf_len - 1);
	buf[received] = '\0';

	return received;
}

int uart_reads(struct UartDevice* dev, char *string) {
	return uart_readn(dev, string, SSIZE_MAX);
}

void uart_stop(struct UartDevice* dev) {
	free(dev->tty);
}
