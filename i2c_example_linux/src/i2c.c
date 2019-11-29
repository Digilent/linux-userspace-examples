/*
 * i2c.c
 *
 * @date 2019/08/09
 * @author Cosmin Tanislav
 */

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "i2c.h"

/*
 * Start the I2C device.
 *
 * @param dev points to the I2C device to be started, must have filename and addr populated
 *
 * @return - 0 if the starting procedure succeeded
 *         - negative if the starting procedure failed
 */
int i2c_start(struct I2cDevice* dev) {
	int fd;
	int rc;

	/*
	 * Open the given I2C bus filename.
	 */
	fd = open(dev->filename, O_RDWR);
	if (fd < 0) {
		rc = fd;
		goto fail_open;
	}

	/*
	 * Set the given I2C slave address.
	 */
	rc = ioctl(fd, I2C_SLAVE, dev->addr);
	if (rc < 0) {
		goto fail_set_i2c_slave;
	}

	dev->fd = fd;

	return 0;

fail_set_i2c_slave:
	close(fd);
fail_open:
	return rc;

}

/*
 * Read data from the I2C device.
 *
 * @param dev points to the I2C device to be read from
 * @param buf points to the start of buffer to be read into
 * @param buf_len length of the buffer to be read
 *
 * @return - 0 if the read procedure succeeded
 *         - negative if the read procedure failed
 */
int i2c_read(struct I2cDevice* dev, uint8_t *buf, size_t buf_len) {
	return read(dev->fd, buf, buf_len);
}

/*
 * Write data to the I2C device.
 *
 * @param dev points to the I2C device to be write to
 * @param buf points to the start of buffer to be written from
 * @param buf_len length of the buffer to be written

 * @return - 0 if the read procedure succeeded
 *         - negative if the read procedure failed
 */
int i2c_write(struct I2cDevice* dev, uint8_t *buf, size_t buf_len) {
	return write(dev->fd, buf, buf_len);
}

/*
 * Read data from a register of the I2C device.
 *
 * @param dev points to the I2C device to be read from
 * @param reg the register to read from
 * @param buf points to the start of buffer to be read into
 * @param buf_len length of the buffer to be read
 *
 * @return - 0 if the read procedure succeeded
 *         - negative if the read procedure failed
 */
int i2c_readn_reg(struct I2cDevice* dev, uint8_t reg, uint8_t *buf, size_t buf_len) {
	int rc;

	/*
	 * Write the I2C register address.
	 */
	rc = i2c_write(dev, &reg, 1);
	if (rc <= 0) {
		printf("%s: failed to write i2c register address\r\n", __func__);
		return rc;
	}

	/*
	 * Read the I2C register data.
	 */
	rc = i2c_read(dev, buf, buf_len);
	if (rc <= 0) {
		printf("%s: failed to read i2c register data\r\n", __func__);
		return rc;
	}

	return rc;
}

/*
 * Write data to the register of the I2C device.
 *
 * @param dev points to the I2C device to be written to
 * @param reg the register to write to
 * @param buf points to the start of buffer to be written from
 * @param buf_len length of the buffer to be written
 *
 * @return - 0 if the write procedure succeeded
 *         - negative if the write procedure failed
 */
int i2c_writen_reg(struct I2cDevice* dev, uint8_t reg, uint8_t *buf, size_t buf_len) {
	uint8_t *full_buf;
	int full_buf_len;
	int rc;
	int i;

	/*
	 * Allocate a buffer that also contains the register address as
	 * the first element.
	 */
	full_buf_len = buf_len + 1;
	full_buf = malloc(sizeof(uint8_t) * full_buf_len);

	full_buf[0] = reg;
	for (i = 0; i < buf_len; i++) {
		full_buf[i + 1] = buf[i];
	}

	/*
	 * Write the I2C register address and data.
	 */
	rc = i2c_write(dev, full_buf, full_buf_len);
	if (rc <= 0) {
		printf("%s: failed to write i2c register address and data\r\n", __func__);
		goto fail_send;
	}

	free(full_buf);
	return 0;

fail_send:
	free(full_buf);
	return rc;
}

/*
 * Read value from a register of the I2C device.
 *
 * @param dev points to the I2C device to be read from
 * @param reg the register to read from
 *
 * @return the value read from the register, 0 if failed
 */
uint8_t i2c_read_reg(struct I2cDevice* dev, uint8_t reg) {
	uint8_t value = 0;
	i2c_readn_reg(dev, reg, &value, 1);
	return value;
}

/*
 * Write value to the register of the I2C device.
 *
 * @param dev points to the I2C device to be written to
 * @param reg the register to write to
 * @param value the value to write to the register
 *
 * @return - 0 if the write procedure succeeded
 *         - negative if the write procedure failed
 */
int i2c_write_reg(struct I2cDevice* dev, uint8_t reg, uint8_t value) {
	return i2c_writen_reg(dev, reg, &value, 1);
}

/*
 * Mask value of to a register of the I2C device.
 *
 * @param dev points to the I2C device to be written to
 * @param reg the register to write to
 * @param mask the mask to apply to the register
 *
 * @return - 0 if the write procedure succeeded
 *         - negative if the write procedure failed
 */
int i2c_mask_reg(struct I2cDevice* dev, uint8_t reg, uint8_t mask) {
	uint8_t value = 0;
	int rc;

	rc = i2c_readn_reg(dev, reg, &value, 1);
	if (rc < 0) {
		return rc;
	}

	value |= mask;
	i2c_write_reg(dev, reg, value);
}

/*
 * Stop the I2C device.
 *
 * @param dev points to the I2C device to be written to
 */
void i2c_stop(struct I2cDevice* dev) {
	/*
	 * Close the I2C bus file descriptor.
	 */
	close(dev->fd);
}
