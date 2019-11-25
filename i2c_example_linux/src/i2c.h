/*
 * i2c.h
 *
 *  Created on: Aug 6, 2019
 *      Author: cosmin
 */

#include <stdint.h>

#ifndef SRC_I2C_H_
#define SRC_I2C_H_

#define I2C_SUCCESS 0
#define I2C_FAILURE -1

/*
 * Configuration for the I2C device;
 *
 */
struct I2cDevice {
	char* filename; /**< Path of the I2C bus, eg: /dev/i2c-0 */
	uint16_t addr; /**< Address of the I2C slave, eg: 0x48 */

	int fd; /**< File descriptor for the I2C bus */
};

/*
 * Start the I2C device.
 *
 * @param dev points to the I2C device to be started, must have filename and addr populated
 *
 * @return - I2C_SUCCESS if the starting procedure succeeded
 *         - I2C_FAILURE if the starting procedure failed
 */
int i2c_start(struct I2cDevice* dev);

/*
 * Read data from the I2C device.
 *
 * @param dev points to the I2C device to be read from
 * @param buf points to the start of buffer to be read into
 * @param buf_len length of the buffer to be read
 *
 * @return - I2C_SUCCESS if the read procedure succeeded
 *         - I2C_FAILURE if the read procedure failed
 */
int i2c_read(struct I2cDevice* dev, uint8_t *buf, size_t buf_len);

/*
 * Write data to the I2C device.
 *
 * @param dev points to the I2C device to be write to
 * @param buf points to the start of buffer to be written from
 * @param buf_len length of the buffer to be written

 * @return - I2C_SUCCESS if the read procedure succeeded
 *         - I2C_FAILURE if the read procedure failed
 */
int i2c_write(struct I2cDevice* dev, uint8_t *buf, size_t buf_len);

/*
 * Read data from a register of the I2C device.
 *
 * @param dev points to the I2C device to be read from
 * @param reg the register to read from
 * @param buf points to the start of buffer to be read into
 * @param buf_len length of the buffer to be read
 *
 * @return - I2C_SUCCESS if the read procedure succeeded
 *         - I2C_FAILURE if the read procedure failed
 */
int i2c_readn_reg(struct I2cDevice* dev, uint8_t reg, uint8_t *buf, size_t buf_len);

/*
 * Write data to the register of the I2C device.
 *
 * @param dev points to the I2C device to be written to
 * @param reg the register to write to
 * @param buf points to the start of buffer to be written from
 * @param buf_len length of the buffer to be written
 *
 * @return - I2C_SUCCESS if the write procedure succeeded
 *         - I2C_FAILURE if the write procedure failed
 */
int i2c_writen_reg(struct I2cDevice* dev, uint8_t reg, uint8_t *buf, size_t buf_len);

/*
 * Read value from a register of the I2C device.
 *
 * @param dev points to the I2C device to be read from
 * @param reg the register to read from
 *
 * @return the value read from the register, 0 if failed
 */
uint8_t i2c_read_reg(struct I2cDevice* dev, uint8_t reg);

/*
 * Write value to the register of the I2C device.
 *
 * @param dev points to the I2C device to be written to
 * @param reg the register to write to
 * @param value the value to write to the register
 *
 * @return - I2C_SUCCESS if the write procedure succeeded
 *         - I2C_FAILURE if the write procedure failed
 */
int i2c_write_reg(struct I2cDevice* dev, uint8_t reg, uint8_t value);

/*
 * Mask value of to a register of the I2C device.
 *
 * @param dev points to the I2C device to be written to
 * @param reg the register to write to
 * @param mask the mask to apply to the register
 *
 * @return - I2C_SUCCESS if the write procedure succeeded
 *         - I2C_FAILURE if the write procedure failed
 */
int i2c_mask_reg(struct I2cDevice* dev, uint8_t reg, uint8_t mask);

/*
 * Stop the I2C device.
 *
 * @param dev points to the I2C device to be written to
 *
 * @return - I2C_SUCCESS if the stop procedure succeeded
 *         - I2C_FAILURE if the stop procedure failed
 */
int i2c_stop(struct I2cDevice* dev);

#endif /* SRC_I2C_H_ */
