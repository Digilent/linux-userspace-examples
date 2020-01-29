# Linux Userspace Examples
This repository provides linux application demos for common communication protocols: SPI, I2C, UART.
The Vivado 2019.1 projects are posted on [Vivado configurations for Linux Userspace examples](https://github.com/Digilent/Zybo-Z7-20-PMOD-Comm-hw) repository (separate branch for each example).
The Petalinux 2019.1 projects are posted on [Petalinux project for Linux Userspace examples](https://github.com/Digilent/Zybo-Z7-20-PMOD-Comm-ow) repository (separate branch for each example).

## SPI Demo
It is implemented using spidev linux spi driver.
It demonstrates a simple SPI communication with [PmodACL2](https://store.digilentinc.com/pmod-acl2-3-axis-mems-accelerometer/).
The demo configures the PmodACL2 device and repeatedly retrieves acceleration values for the three axis.

### SPI Demo Vivado project
The demo is using AXI Quad SPI IP in the Vivado project, having its lines configured to the Pmod connector where PmodACL2 is plugged.
The Vivado project for the SPI demo is posted [here](https://github.com/Digilent/Zybo-Z7-20-PMOD-Comm-hw/tree/spi_example). It is configured for PmodACL2 plugged on JB Pmod connector.

### SPI Demo Petalinux configuration
In order to use the spidev driver, the following node must exist in the device_user.dtsi:
```
&axi_quad_spi_0 {
	spidev@0 {
		reg = <0>;
		compatible = "spidev";
		spi-max-frequency = <25000000>;
	};
```
Petalinux project for the SPI demo is posted [here](https://github.com/Digilent/Zybo-Z7-20-PMOD-Comm-os/tree/spi_example).

## I2C Demo
It is implemented using standard linux I2C driver.
It demonstrates a simple I2C communication with [PmodTMP3](https://store.digilentinc.com/pmod-tmp3-digital-temperature-sensor/).
The demo configures the PmodTMP3 device and repeatedly retrieves the ambient temperature.

### I2C Demo Vivado project
The demo is using AXI IIC IP in the Vivado project, having its lines configured to the Pmod connector where PmodTMP3 is plugged.
The Vivado project for the I2C demo is posted [here](https://github.com/Digilent/Zybo-Z7-20-PMOD-Comm-hw/tree/i2c_example). It is configured for PmodTMP3 plugged on JB Pmod connector.

### I2C Demo Petalinux configuration
Petalinux project for the I2C demo is posted [here](https://github.com/Digilent/Zybo-Z7-20-PMOD-Comm-os/tree/i2c_example).

## UART Demo
It is implemented using standard linux TTY driver. It demonstrates a simple UART communication.
It uses PmodUSB-UART to interface the UART lines with the PC.  
The demo repeatedly echoes over UART data received over UART. Open a terminal and type some characters, they will be echoed back.

### UART Demo Vivado project
Insert AXI UartLite IP in the Vivado project, configuring its lines to the Pmod connector where you connect PmodUSB-UART.
The Vivado project for the UART demo is posted [here](https://github.com/Digilent/Zybo-Z7-20-PMOD-Comm-hw/tree/uart_example). It is configured for PmodUSB-UART plugged on JB Pmod connector.

### UART Demo Petalinux configuration
Petalinux project for the UART demo is posted [here](https://github.com/Digilent/Zybo-Z7-20-PMOD-Comm-os/tree/uart_example).
