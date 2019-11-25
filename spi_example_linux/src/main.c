/*
 * main.c
 *
 * @date 2019/08/09
 * @author Cosmin Tanislav
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "spi.h"


/*
 * Write data from a register of a SPI device.
 *
 * @param dev points to the SPI device to be written to
 * @param reg the register to write to
 * @param buf points to the start of buffer to be written from
 * @param buf_len length of the buffer to be written
 *
 * @return - 0 if the wrote procedure succeeded
 *         - negative if the write procedure failed
 */
int acl2_nwrite_reg(struct SpiDevice *dev, uint8_t reg, uint8_t *buf, int buf_len) {
	uint8_t *full_buf;
	int full_buf_len;

	/*
	 * Allocate a buffer that contains the instruction and
	 * the register address as the first two elements.
	 */
	full_buf_len = buf_len + 2;
	full_buf = malloc(full_buf_len * sizeof(*full_buf));
	full_buf[0] = 0xA;
	full_buf[1] = reg;

	/*
	 * Copy the data to be written into the full buffer.
	 */
	memcpy(full_buf + 2, buf, buf_len);

	/*
	 * Transfer the instruction, register address and data.
	 */
	return spi_transfer(dev, full_buf, NULL, full_buf_len);
}

/*
 * Read data from a register of a SPI device.
 *
 * @param dev points to the SPI device to be read from
 * @param reg the register to read from
 * @param buf points to the start of buffer to be read into
 * @param buf_len length of the buffer to be read
 *
 * @return - 0 if the read procedure succeeded
 *         - negative if the read procedure failed
 */
int acl2_nread_reg(struct SpiDevice *dev, uint8_t reg, uint8_t *buf, int buf_len) {
	uint8_t *full_buf;
	int full_buf_len;
	int rc;

	/*
	 * Allocate a buffer that contains the instruction and
	 * the register address as the first element.
	 */
	full_buf_len = buf_len + 2;
	full_buf = malloc(full_buf_len * sizeof(*full_buf));
	full_buf[0] = 0x0B;
	full_buf[1] = reg;

	/*
	 * Transfer the instruction, register address and data.
	 */
	rc = spi_transfer(dev, full_buf, full_buf, full_buf_len);

	/*
	 * Copy the read data into the buffer.
	 */
	memcpy(buf, full_buf + 2, buf_len);

	return rc;
}

/*
 * Read value from a register of a SPI device
 *
 * @param dev points to the SPI device to be read from
 * @param reg the register to read from
 *
 * @return the value read from the register
 */
int acl2_read_reg(struct SpiDevice *dev, uint8_t reg) {
	uint8_t data = 0;
	acl2_nread_reg(dev, reg, &data, 1);
	return data;
}

/*
 * Write value to the register of a SPI device.
 *
 * @param dev points to the SPI device to be written to
 * @param reg the register to write to
 * @param value the value to be written
 *
 * @return - 0 if the write procedure succeeded
 *         - negative if the write procedure failed
 */
int acl2_write_reg(struct SpiDevice *dev, uint8_t reg, uint8_t value) {
	return acl2_nwrite_reg(dev, reg, &value, 1);
}

int main() {
	uint8_t values[3];
	uint8_t value;
	struct SpiDevice dev;
	int rc;

	/*
	 * Set the SPI bus filename, mode, bits-per-word and speed.
	 * See: https://github.com/torvalds/linux/blob/master/include/uapi/linux/spi/spidev.h
	 */
	dev.filename = "/dev/spidev1.0";
	dev.mode = 0;
	dev.bpw = 8;
	dev.speed = 100000 / 16;

	/*
	 * Start the I2C device.
	 */
	rc = spi_start(&dev);
	if (rc) {
		printf("failed to start SPI device\r\n");
		return rc;
	}

	/*
	 * Read power status register.
	 */
	value = acl2_read_reg(&dev, 0x2D);

	/*
	 * Enable measurement.
	 */
	value |= 0b0000010;

	/*
	 * Write back status register.
	 */
	acl2_write_reg(&dev, 0x2D, value);

	while (1) {
		/*
		 * Burst read 0x08, 0x09, 0x0A.
		 */
		acl2_nread_reg(&dev, 0x08, values, 3);

		printf("x: %2.2X, y: %2.2X, z: %2.2X\r\n",
					values[0], values[1], values[2]);

		sleep(1);
	}

    return 0;
}
