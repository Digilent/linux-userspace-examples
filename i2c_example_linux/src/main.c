/*
 * main.c
 *
 * @date 2019/08/09
 * @author Cosmin Tanislav
 */

#include <stdio.h>
#include <unistd.h>

#include "i2c.h"

#define TEMP_REG 0x0
#define CONFIG_REG 0x1

#define CONFIG_SHUTDOWN 0b00000001
#define CONFIG_ONESHOT 0b10000000
float tmp3_read_temperature(struct I2cDevice *dev) {
	int rc;

	/*
	 * Start conversion process.
	 */
	i2c_mask_reg(dev, CONFIG_REG, CONFIG_ONESHOT);

	/*
	 * Wait for conversion process to complete.
	 */
	usleep(40000);

	/*
	 * Read temperature register.
	 */
	uint8_t data[2] = {0};
	rc = i2c_readn_reg(dev, TEMP_REG, data, 2);
	if (rc) {
		printf("failed to read i2c register\r\n");
		return rc;
	}

	return (data[0] << 8 | data[1]) / 256.0;
}

int main() {
	struct I2cDevice dev;
	int rc;

	/*
	 * Set the I2C bus filename and slave address,
	 */
	dev.filename = "/dev/i2c-0";
	dev.addr = 0x48;

	/*
	 * Start the I2C device.
	 */
	rc = i2c_start(&dev);
	if (rc) {
		printf("failed to start i2c device\r\n");
		return rc;
	}

	/*
	 * Write shutdown mode to the configuration register,
	 * to use one-shot mode for measurements.
	 */
	i2c_mask_reg(&dev, CONFIG_REG, CONFIG_SHUTDOWN);

	for (size_t i = 0; i < 10; i++) {
		float temperature = tmp3_read_temperature(&dev);
		printf("temperature: %f\n", temperature);
		sleep(1);
	}

	i2c_stop(&dev);

    return 0;
}
