/*
 * spi.h
 *
 * @date 2019/08/09
 * @author Cosmin Tanislav
 */

#include <stdint.h>

#ifndef SPI_H
#define SPI_H

#define SPI_SUCCESS 0
#define SPI_FAILURE -1

struct SpiDevice {
	char *filename; /**< Path of the SPI bus, eg: /dev/spidev1.0 */
	uint8_t mode; /**< Mode of the SPI bus */
	uint8_t bpw; /**< Bits-per-word of the SPI bus */
	uint32_t speed; /**< Speed of the SPI bus */

	int fd; /**< File descriptor for the SPI bus */
};

/*
 * Start the SPI device.
 *
 * @param dev points to the SPI device to be started, must have filename,
 *  mode, bpw and speed populated.
 *
 * @return - 0 if the starting procedure succeeded
 *         - negative if the starting procedure failed
 */
int spi_start(struct SpiDevice *dev);

/*
 * Transfer data with the SPI device.
 *
 * @param dev points to the I2C device to be read from
 * @param write_buf points to the start of the buffer to be written from
 * @param read_buf points to the start of the buffer to be read into
 * @param buf_len length of the buffers
 *
 * @return - 0 if the read procedure succeeded
 *         - negative if the read procedure failed
 */
int spi_transfer(struct SpiDevice *dev, uint8_t *write_buf, uint8_t *read_buf, uint32_t buf_len);

/*
 * Stop the SPI device.
 *
 * @param dev points to the SPI device to be stopped to
 *
 * @return - 0 if the stop procedure succeeded
 *         - negative if the stop procedure failed
 */
void spi_stop(struct SpiDevice *dev);

#endif // SPI_H
