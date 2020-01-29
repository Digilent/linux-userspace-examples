/*
 * main.c
 *
 * @date 2019/08/09
 * @author Cosmin Tanislav
 * @author Cristian Fatu
 */

#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include "uart.h"

#define LOOPBACK_FORMAT "loopback: %s\r\n"
#define LOOPBACK_FORMAT_LEN strlen(LOOPBACK_FORMAT)
#define MAX_READ_SIZE 235
#define MAX_LOOPBACK_SIZE MAX_READ_SIZE + LOOPBACK_FORMAT_LEN

int main() {
	struct UartDevice dev;
	int rc;

	dev.filename = "/dev/ttyUL1";
	dev.rate = B9600;

	rc = uart_start(&dev, false);
	if (rc) {
		return rc;
	}

	char read_data[MAX_READ_SIZE];
	char loopback_data[MAX_LOOPBACK_SIZE];
	size_t read_data_len;

	printf("UART DEMO\r\n");
	uart_writes(&dev, "UART DEMO\r\n");

	while (1) {
		read_data_len = uart_reads(&dev, read_data, MAX_READ_SIZE);

		if (read_data_len > 0) {
			printf("%s", read_data);
			snprintf(loopback_data, MAX_LOOPBACK_SIZE, LOOPBACK_FORMAT, read_data);
			uart_writes(&dev, loopback_data);
		}
	}

	uart_stop(&dev);

    return 0;
}
