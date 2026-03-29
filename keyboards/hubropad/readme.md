# Hubropad

![Hubropad](https://i.imgur.com/PLACEHOLDER.jpg)

A 2×4 macropad (8 keys) built around an STM32F103C8T6 MCU, designed to sit behind an integrated USB hub. The firmware includes a custom USB enumeration driver that controls a D+ pull-up via GPIO to ensure the keyboard only enumerates after the hub is online.

* Keyboard Maintainer: hzb
* Hardware Supported: Hubropad (STM32F103C8T6, 12 MHz crystal)

Make example for this keyboard (after setting up your build environment):

    make hubropad:default

Flashing example for this keyboard:

    make hubropad:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 2 ways:

* **Bootmagic reset**: Hold down the top-left key (KEY1) and plug in the keyboard.
* **Physical reset button**: Press the reset button on the PCB (if present).

## Hardware Features

### Key Matrix

8 keys in a 2×4 direct-pin matrix (one GPIO per key, active-low with internal pull-up):

| | Col 0 | Col 1 | Col 2 | Col 3 |
|---|---|---|---|---|
| **Row 0** | KEY1 (PB11) | KEY2 (PB12) | KEY3 (PA2) | KEY4 (PA3) |
| **Row 1** | KEY5 (PB10) | KEY6 (PB2) | KEY7 (PB1) | KEY8 (PB0) |

### Lighting

* **RGB Underglow**: 6× WS2812B LEDs on PA1 (front-facing, `rgblight` feature)
* **Key backlights**: White LEDs driven by MOSFET on PA8, PWM-dimmable via TIM1_CH1
* **Debug LED**: Red LED on PB15, lights up after successful initialization

### USB Hub Enumeration Driver

The keyboard connects to the host through an integrated USB hub on the same PCB. Because the hub and the keyboard share the same power rail, the keyboard MCU boots simultaneously with the hub controller. The hub needs time to initialize before it can relay downstream device connections to the host.

To handle this, the firmware implements a controlled USB enumeration sequence:

1. **Boot (GPIO init):** PB3 is configured as output-low in `board.h` via `VAL_GPIOBCRL`/`VAL_GPIOBODR` overrides. This keeps the 1.5 kΩ D+ pull-up de-asserted so the hub does not see a downstream device yet.

2. **Pre-init delay (`keyboard_pre_init_kb`):** QMK calls `keyboard_pre_init_kb()` *before* `protocol_pre_init()` (which starts the ChibiOS USB driver). A configurable delay (`USB_HUB_ENUMERATION_DELAY_MS`, default 500 ms) gives the hub time to complete its own enumeration with the host.

3. **USB connect (`usb_lld_connect_bus` macro):** When ChibiOS calls `usbConnectBus()`, the overridden `usb_lld_connect_bus` macro drives PB3 high, asserting the D+ pull-up. The hub detects the new downstream device and the host enumerates the keyboard normally.

4. **USB disconnect (`usb_lld_disconnect_bus` macro):** On suspend or disconnect, the macro drives PB3 low, cleanly removing the keyboard from the hub.

#### Tuning the Delay

If the keyboard fails to enumerate on some hosts/hubs, increase the delay in `config.h`:

```c
#define USB_HUB_ENUMERATION_DELAY_MS 1000
```

### Clock Configuration

The board uses a 12 MHz HSE crystal. The PLL is configured as 12 MHz × 6 = 72 MHz (system clock), with USB at 72 / 1.5 = 48 MHz. The relevant overrides are in `mcuconf.h` and `board.h`.

## How to build?

### 1. Install dependencies
sudo apt install git python3-pip gcc-arm-none-eabi binutils-arm-none-eabi make

### 2. Clone with submodules (or init them in your existing checkout)
git submodule update --init --recursive

### 3. Install QMK CLI
pip3 install --user qmk

### 4. Set up QMK environment (first time only)
qmk setup --home /home/llatva/git/qmk_firmware_hubropad

### 5. Build
make hubropad:default

### 6. Flash (put board in bootloader mode first, then)
make hubropad:default:flash
