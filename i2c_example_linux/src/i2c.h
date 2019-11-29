/*
 * i2c.h
 *
 * @date 2019/08/09
 * @author Cosmin Tanislav
 */

#include <stdint.h>

#ifndef SRC_I2C_H_
#define SRC_I2C_H_

/*
 * Configuration for the I2C device;
 */
struct I2cDevice {
	char* filename; /**< Path of the I2C bus, eg: /dev/i2c-0 */
	uint16_t addr; /**< Address of the I2C slave, eg: 0x48 */

	int fd; /**< File descriptor for the I2C bus */
};

int i2c_start(struct I2cDevice* dev);
int i2c_read(struct I2cDevice* dev, uint8_t *buf, size_t buf_len);
int i2c_write(struct I2cDevice* dev, uint8_t *buf, size_t buf_len);
int i2c_readn_reg(struct I2cDevice* dev, uint8_t reg, uint8_t *buf, size_t buf_len);
int i2c_writen_reg(struct I2cDevice* dev, uint8_t reg, uint8_t *buf, size_t buf_len);
uint8_t i2c_read_reg(struct I2cDevice* dev, uint8_t reg);
int i2c_write_reg(struct I2cDevice* dev, uint8_t reg, uint8_t value);
int i2c_mask_reg(struct I2cDevice* dev, uint8_t reg, uint8_t mask);
void i2c_stop(struct I2cDevice* dev);

#endif /* SRC_I2C_H_ */
