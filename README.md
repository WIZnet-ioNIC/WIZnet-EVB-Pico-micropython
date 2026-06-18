# WIZnet-EVB-Pico-micropython

> Example codes and usage for WIZnet EVB Pico boards are available at
> [WIZnet-ioNIC/WIZnet-ioNIC-micropython-examples](https://github.com/WIZnet-ioNIC/WIZnet-EVB-Pico-micropython-examples.git)

**MicroPython firmware with built-in WIZnet Ethernet support for WIZnet EVB Pico boards**

## Branches

| Branch | Description |
|--------|-------------|
| [fix-wiznet-timeout](https://github.com/WIZnet-ioNIC/WIZnet-EVB-Pico-micropython/tree/fix-wiznet-timeout) | **Recommended** — required for MQTT |
| [WIZnet-EVB-Pico-micropython](https://github.com/WIZnet-ioNIC/WIZnet-EVB-Pico-micropython/tree/WIZnet-EVB-Pico-micropython) | Base PR branch |

Upstream MicroPython PR: [micropython/micropython#18035](https://github.com/micropython/micropython/pull/18035)

---

## Supported Boards

| Board Series | Pico | Pico2 |
|--------------|------|-------|
| **W5100S-EVB** | ✅ W5100S-EVB-Pico | ✅ W5100S-EVB-Pico2 |
| **W5500-EVB** | ✅ W5500-EVB-Pico | ✅ W5500-EVB-Pico2 |
| **W55RP20-EVB** | ✅ W55RP20-EVB-Pico | - |
| **W6100-EVB** | ✅ W6100-EVB-Pico | ✅ W6100-EVB-Pico2 |
| **W6300-EVB** | ✅ W6300-EVB-Pico | ✅ W6300-EVB-Pico2 |

---

## Quick Start (Release UF2 Recommended)

### Step 1: Download Firmware

Download the **`build-<BOARD>` asset** for your board from the [**Releases page**](https://github.com/WIZnet-ioNIC/WIZnet-EVB-Pico-micropython/releases).

**Examples:**
- `build-W55RP20_EVB_PICO/firmware.uf2`
- `build-W6300_EVB_PICO/firmware.uf2`

### Step 2: Flash Firmware

1. Hold down the board's **BOOTSEL** button and connect via USB
2. The `RPI-RP2` drive will mount automatically
3. Drag & drop the downloaded **`firmware.uf2`** file into the `RPI-RP2` drive
4. The board will automatically reboot after the copy is complete

> **Note:** If you built from source, use `ports/rp2/build-<BOARD>/firmware.uf2` with the same procedure.

---

## Build from Source

### Requirements

| Component | Details |
|-----------|---------|
| **Host OS** | Ubuntu / Debian / WSL2 |
| **Packages** | `git`, `make`, `cmake`, `gcc-arm-none-eabi`, `libnewlib-arm-none-eabi`, `build-essential` |
| **Cable** | USB cable suitable for your board (Pico/Pico2: microUSB or USB-C) |

### 1. Clone Repository

```bash
git clone --recurse-submodules https://github.com/WIZnet-ioNIC/WIZnet-EVB-Pico-micropython.git
cd WIZnet-EVB-Pico-micropython
git checkout fix-wiznet-timeout
git submodule update --init --recursive
```

### 2. Build mpy-cross (Required)

```bash
make -C mpy-cross
```

> **Tip:** If you get errors, check that submodules like `lib/pico-sdk` and `lib/tinyusb` were fully cloned.

### 3. Build for Selected Board

```bash
cd ports/rp2

# Example: W55RP20-EVB-Pico
make BOARD=W55RP20_EVB_PICO -j

# Example: W6300-EVB-Pico2
make BOARD=W6300_EVB_PICO2 -j
```

**Output:** `ports/rp2/build-<BOARD>/firmware.uf2`

---

## Verify Operation (REPL)

### W5100S / W5500 / W6100 series (WIZNET6K)

```python
import network

nic = network.WIZNET6K()
nic.active(True)
nic.ifconfig("dhcp")
print('ifconfig:', nic.ifconfig())
```

### W55RP20 (WIZNET_PIO_SPI + WIZNET6K)

```python
import network
from machine import Pin, WIZNET_PIO_SPI

spi = WIZNET_PIO_SPI(baudrate=10_000_000, sck=Pin(21), cs=Pin(20), mosi=Pin(23), miso=Pin(22))
nic = network.WIZNET6K(spi, Pin(20), Pin(25))
nic.active(True)
nic.ifconfig("dhcp")
print('ifconfig:', nic.ifconfig())
```

### W6300 series (WIZNET_PIO_SPI + WIZNET6K)

```python
import network
from machine import Pin, WIZNET_PIO_SPI

spi = WIZNET_PIO_SPI(
    baudrate=10_000_000,
    sck=Pin(17), cs=Pin(16),
    io0=Pin(18), io1=Pin(19), io2=Pin(20), io3=Pin(21)
)
nic = network.WIZNET6K(spi, Pin(16), Pin(22))
nic.active(True)
nic.ifconfig("dhcp")
print('ifconfig:', nic.ifconfig())
```

---

## Upload Scripts with Thonny (Optional)

1. Set Interpreter to **MicroPython (Raspberry Pi Pico)**
2. Once REPL is connected, save your `main.py` / `boot.py` onto the board for auto execution

---

## License

- This repository is based on **MicroPython** (MIT license)
- For each board/driver, please check the license file inside its directory
