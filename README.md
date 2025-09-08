# 🚀 WIZnet-EVB-Pico-micropython

## ⚡ Quick Start (Release UF2 Recommended)

### Step 1: Download Firmware
Download the **`build-<BOARD>` asset** for your board from **Releases** v1.xx.x-WIZnet of this repository.  
📋 **Examples:**
- `build-W55RP20_EVB_PICO/firmware.uf2`
- `build-W6300_EVB_PICO/firmware.uf2`

### Step 2: Flash Firmware
1. Hold down the board's **BOOTSEL** button and connect via USB
2. The `RPI-RP2` drive will mount automatically
3. Drag & drop the downloaded **`firmware.uf2`** file into the `RPI-RP2` drive
4. The board will automatically reboot after the copy is complete

> 💡 **Note:** If you built from source, use `ports/rp2/build-<BOARD>/firmware.uf2` with the same procedure.

> **MicroPython firmware with built-in WIZnet Ethernet support for WIZnet EVB Pico boards**

## 📋 Supported Boards

| Board Series | Pico | Pico2 |
|--------------|------|-------|
| **W5100S-EVB** | ✅ W5100S-EVB-Pico | ✅ W5100S-EVB-Pico2 |
| **W5500-EVB** | ✅ W5500-EVB-Pico | ✅ W5500-EVB-Pico2 |
| **W55RP20-EVB** | ✅ W55RP20-EVB-Pico | - |
| **W6100-EVB** | ✅ W6100-EVB-Pico | ✅ W6100-EVB-Pico2 |
| **W6300-EVB** | ✅ W6300-EVB-Pico | ✅ W6300-EVB-Pico2 |  

---

### Quick Start (Release UF2 Recommended)
1. Download the **`build-<BOARD>` asset** for your board from GitHub **Releases**.  
   Example: `build-W55RP20_EVB_PICO/firmware.uf2`, `build-W6300_EVB_PICO/firmware.uf2`
2. Hold down the board’s **BOOTSEL** button and connect via USB → the `RPI-RP2` drive will mount.
3. Drag & drop the downloaded **`firmware.uf2`** file into the `RPI-RP2` drive.
4. The board will automatically reboot after the copy is complete.

*(If you built from source, use `ports/rp2/build-<BOARD>/firmware.uf2` with the same procedure.)*

---

## 🛠️ Build from Source

### Requirements
| Component | Details |
|-----------|---------|
| **Host OS** | Ubuntu / Debian / WSL2 (Windows users: WSL recommended) |
| **Packages** | `git`, `make`, `cmake`, `gcc-arm-none-eabi`, `libnewlib-arm-none-eabi`, `build-essential` |
| **Cable** | USB cable suitable for your board (Pico/Pico2: microUSB or USB-C) |

### 1️⃣ Clone Repository & Switch Branch
```bash
# 1) Clone repo
git clone --recurse-submodules https://github.com/WIZnet-ioNIC/WIZnet-EVB-Pico-micropython.git
cd WIZnet-EVB-Pico-micropython

# 2) Checkout branch
git checkout WIZnet-EVB-Pico-micropython

# 3) Resync submodules
git submodule update --init --recursive
```

---

### 2️⃣ Build mpy-cross (Required)
Used for pre-compiling MicroPython scripts into `.mpy` files.
```bash
make -C mpy-cross
```

> 💡 **Tip:** If you get errors, check that submodules like `lib/pico-sdk` and `lib/tinyusb` were fully cloned.

### 3️⃣ Build for Selected Board

#### Example A: W55RP20-EVB-Pico
```bash
# Run inside ports/rp2 directory
make BOARD=W55RP20_EVB_PICO -j
```

#### Example B: W6300-EVB-Pico2
```bash
# Run inside ports/rp2 directory
make BOARD=W6300_EVB_PICO2 -j
```

**📁 Output Location:**
```
ports/rp2/build-<BOARD>/firmware.uf2
```
**Example:** `ports/rp2/build-W55RP20_EVB_PICO/firmware.uf2`

---

## 🧪 Verify Operation (REPL)

Once the board reboots, connect to the serial port REPL and test with the examples below.

### 🔹 W5100S/W5500/W6100 series (WIZNET6K)
```python
import network

# Auto init (simple)
nic = network.WIZNET6K()   # Driver name: 6K
nic.active(True)
nic.ifconfig("dhcp")
print('ifconfig:', nic.ifconfig())
```

### 🔹 W55RP20 (WIZNET_PIO_SPI + WIZNET6K)
```python
import network

spi = WIZNET_PIO_SPI(baudrate=10_000_000, sck=Pin(21), cs=Pin(20), mosi=Pin(23), miso=Pin(22))
nic = network.WIZNET6K(spi, Pin(20), Pin(25))  # spi, cs, reset pin
nic.active(True)
nic.ifconfig("dhcp")
print('ifconfig:', nic.ifconfig())
```

### 🔹 W6300 series (WIZNET_PIO_SPI + WIZNET6K)
```python
import network

spi = WIZNET_PIO_SPI(
    baudrate=10_000_000,
    sck=Pin(17), cs=Pin(16),
    io0=Pin(18), io1=Pin(19), io2=Pin(20), io3=Pin(21)  # QUAD_QSPI_MODE
)
nic = network.WIZNET6K(spi, Pin(16), Pin(22))  # spi, cs, reset pin
nic.active(True)
nic.ifconfig("dhcp")
print('ifconfig:', nic.ifconfig())
```

---

## 🔧 Upload Scripts with Thonny (Optional)

1. **Configure Thonny:** Set Interpreter to **MicroPython (Raspberry Pi Pico)**
2. **Upload Scripts:** Once REPL is connected, save your `main.py` / `boot.py` onto the board for auto execution

---

## 📄 License

- This repository is based on **MicroPython** (MIT license)
- For each board/driver, please check the license file inside its directory
