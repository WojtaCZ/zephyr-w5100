# About
This repository contains a Wiznet W5100 driver for Zephyr RTOS. It's heavilly based on the existing W5500 driver with just minor changes in memory adressing, SPI frame structure and register adressing.

# Usage
1. Copy the `eth_w5100_priv.h`, `eth_w5100.c` and `Kconfig.w5100` files into the directory `[zephyr root]/drivers/ethernet`
2. In the same directory, add the line `source "drivers/ethernet/Kconfig.w5100"` into the `Kconfig` file
3. In the same directory, add the line `zephyr_library_sources_ifdef(CONFIG_ETH_W5100		eth_w5100.c)` into the `CMakeLists.txt` file
4. Copy the `wiznet,w5100.yaml` file into the directory `[zephyr root]/dts/bindings/ethernet`

Now the driver should be fully usable in your project. Example of binding the driver in the dts file of your board (nRF5340 is used in this example).

```
&spi4 {
    compatible = "nordic,nrf-spim";
    status = "okay";
	pinctrl-0 = <&spi4_default>;
	pinctrl-1 = <&spi4_sleep>;
	pinctrl-names = "default", "sleep";
    cs-gpios = <&gpio0 11 GPIO_ACTIVE_LOW>;
    w5100: w5100@0 {
        compatible = "wiznet,w5100";
        reg = <0x0>;
        spi-max-frequency = <1000000>;
        int-gpios = <&gpio0 19 GPIO_ACTIVE_LOW>;
        reset-gpios = <&gpio0 20 GPIO_ACTIVE_LOW>;
    };
};
```