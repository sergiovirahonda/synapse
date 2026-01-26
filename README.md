# Synapse

A professional-grade drone transmitter (TX) controller built for Arduino, designed to interface with USB joysticks via a USB Host Shield and transmit flight commands wirelessly to DIY drones.

## Overview

Synapse is the control interface for your DIY drone project. It reads input from a USB joystick (specifically the Thrustmaster T16000M), processes the control signals, and transmits them via nRF24L01 radio module to the drone receiver. This project pairs with the [Cortex](https://github.com/sergiovirahonda/cortex) drone flight controller.

## Features

- 🎮 **USB Joystick Support**: Native support for Thrustmaster T16000M via USB Host Shield
- 📡 **Wireless Communication**: Reliable nRF24L01 radio transmission with optimized settings
- 🏗️ **Clean Architecture**: Modular design with adapters and models for maintainability
- ⚡ **Real-time Control**: Low-latency command transmission at 100Hz
- 🔧 **PlatformIO Integration**: Modern build system with dependency management

## Hardware Requirements

### Core Components
- **Arduino Nano** (ATmega328)
- **USB Host Shield** (for joystick connectivity)
- **nRF24L01** radio module
- **Thrustmaster T16000M** joystick (or compatible HID device)

### Pin Connections

| Component | Pin | Arduino Pin |
|-----------|-----|-------------|
| nRF24L01 CE | - | D7 |
| nRF24L01 CSN | - | D8 |
| nRF24L01 MOSI | - | D11 (SPI) |
| nRF24L01 MISO | - | D12 (SPI) |
| nRF24L01 SCK | - | D13 (SPI) |

## Software Architecture

```
synapse/
├── src/
│   ├── main.cpp                 # Main control loop
│   ├── adapters/
│   │   ├── joystick_adapter.*   # USB HID joystick parser
│   │   └── radio_adapter.*      # nRF24L01 communication
│   └── models/
│       ├── drone_command.*      # Command data structure
│       └── joystick_input.*     # Joystick input model
└── platformio.ini               # Build configuration
```

### Key Components

- **T16000MParser**: Custom HID parser that extracts pitch, roll, yaw, and throttle from joystick input
- **RadioAdapter**: Handles nRF24L01 initialization and packet transmission
- **DroneCommand**: Encapsulates flight control commands with proper data types

## Installation

### Prerequisites

1. **PlatformIO**: Install [PlatformIO IDE](https://platformio.org/install/ide?install=vscode) or use the CLI
2. **Arduino IDE** (optional): For manual uploads if not using PlatformIO

### Setup Steps

1. **Clone the repository**
   ```bash
   git clone https://github.com/sergiovirahonda/synapse.git
   cd synapse
   ```

2. **Install dependencies**
   PlatformIO will automatically install required libraries:
   - `RF24` - nRF24L01 radio driver
   - `USB_Host_Shield_2.0` - USB Host Shield library

3. **Configure radio address**
   Edit `src/main.cpp` to match your drone's receiver address:
   ```cpp
   byte NRF_ADDRESS[6] = "00001";  // Must match receiver address
   ```

4. **Build and upload**
   ```bash
   pio run -t upload
   ```

5. **Monitor serial output**
   ```bash
   pio device monitor
   ```
   You should see initialization messages and real-time command data.

## Configuration

### Radio Settings

The radio adapter is configured for optimal range and reliability:

- **Data Rate**: 250KBPS (longest range)
- **Power Level**: PA_LOW (adjustable for range vs. power consumption)
- **Channel**: 108 (above 2.48GHz to avoid WiFi interference)
- **Auto-ACK**: Disabled (for lower latency)

### Joystick Mapping

The T16000M joystick is mapped as follows:

- **Pitch**: Forward/Backward stick movement (normalized to -100 to +100)
- **Roll**: Left/Right stick movement (normalized to -100 to +100)
- **Yaw**: Twist axis (normalized to -100 to +100)
- **Throttle**: Throttle slider (normalized to 0 to 100%)

### Command Format

Commands are transmitted as a packed struct:
```cpp
struct DronePacket {
    int16_t pitch;    // -100 to +100
    int16_t roll;     // -100 to +100
    int16_t yaw;      // -100 to +100
    int16_t throttle; // 0 to 100
}
```

## Usage

1. **Power on** the Arduino with USB Host Shield attached
2. **Connect** the Thrustmaster T16000M joystick to the USB Host Shield
3. **Verify** initialization via serial monitor:
   - USB Host Shield initialized
   - Radio chip connected
4. **Control** the drone by moving the joystick
5. **Monitor** real-time command transmission in serial output

## Troubleshooting

### Radio Not Connecting
- Verify nRF24L01 wiring (CE, CSN, SPI pins)
- Check that radio address matches receiver
- Ensure power supply provides stable 3.3V

### Joystick Not Detected
- Verify USB Host Shield is properly connected
- Check USB Host Shield power (may need external 5V)
- Ensure joystick is in HID mode

### Commands Not Received
- Verify radio address matches on both TX and RX
- Check radio channel and data rate settings
- Ensure receiver is powered and in listening mode

## Development

### Building

```bash
# Build project
pio run

# Upload to device
pio run -t upload

# Clean build artifacts
pio run -t clean
```

### Adding Support for Other Joysticks

To support a different joystick, modify `T16000MParser::Parse()` in `joystick_adapter.cpp` to match your joystick's HID report format. The parser extracts raw values from the HID buffer and normalizes them.

## Related Projects

- **[Cortex](https://github.com/sergiovirahonda/cortex)**: The drone flight controller that receives commands from Synapse

## License

This project is open source. See repository for license details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

---

**Built with ❤️ for the DIY drone community**
