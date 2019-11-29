/*
 * spi.c
 *
 * @date 2019/08/09
 * @author Cosmin Tanislav
 */

#include <linux/spi/spidev.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spi.h"

/*
 * Start the SPI device.
 *
 * @param dev points to the SPI device to be started, must have filename,
 *  mode, bpw and speed populated.
 *
 * @return - 0 if the starting procedure succeeded
 *         - negative if the starting procedure failed
 */
int spi_start(struct SpiDevice *dev) {
	int fd;
	int rc;

	fd = open(dev->filename, O_RDWR);
	if (fd < 0) {
		printf("%s: failed to start SPI\r\n", __func__);
		goto fail_open;
	}

	/*
	 * Set the given SPI mode.
	 */
	rc = ioctl(fd, SPI_IOC_WR_MODE, &dev->mode);
	if (rc < 0) {
		printf("%s: failed to set SPI write mode\r\n", __func__);
		goto fail_configure;
	}

	rc = ioctl(fd, SPI_IOC_RD_MODE, &dev->mode);
	if (rc < 0) {
		printf("%s: failed to set SPI read mode\r\n", __func__);
		goto fail_configure;
	}

	/*
	 * Set the given SPI bits-per-word.
	 */
	rc = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &dev->bpw);
	if (rc < 0) {
		printf("%s: failed to set SPI write bits-per-word\r\n", __func__);

		goto fail_configure;
	}

	rc = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &dev->bpw);
	if (rc < 0) {
		printf("%s: failed to set SPI read bits-per-word\r\n", __func__);
		goto fail_configure;
	}

	/*
	 * Set the given SPI speed.
	 */
	rc = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &dev->speed);
	if (rc < 0) {
		printf("%s: failed to set SPI write speed\r\n", __func__);
		goto fail_configure;
	}

	rc = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &dev->speed);
	if (rc < 0) {
		printf("%s: failed to set SPI read speed\r\n", __func__);
		goto fail_configure;
	}

	dev->fd = fd;

	return 0;

fail_configure:
	close(fd);
fail_open:
	return rc;
}

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
int spi_transfer(struct SpiDevice *dev, uint8_t *write_buf, uint8_t *read_buf, uint32_t buf_len) {
	struct spi_ioc_transfer transfer;
	int rc;

	memset(&transfer, 0, sizeof(transfer));
	transfer.tx_buf = (uint32_t)write_buf;
	transfer.rx_buf = (uint32_t)read_buf;
	transfer.len = buf_len;
	transfer.speed_hz = dev->speed;
	transfer.bits_per_word = dev->bpw;
	transfer.cs_change = 1;

	rc = ioctl(dev->fd, SPI_IOC_MESSAGE(1), &transfer);
	if (rc < 0) {
		printf("%s: failed to start SPI transfer\r\n", __func__);
	}

	return rc;
}

/*
 * Stop the SPI device.
 *
 * @param dev points to the SPI device to be stopped to
 *
 * @return - 0 if the stop procedure succeeded
 *         - negative if the stop procedure failed
 */
void spi_stop(struct SpiDevice *dev) {
	close(dev->fd);
}
