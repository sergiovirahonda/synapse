# Synapse

A professional-grade drone transmitter (TX) controller built for Arduino, designed to interface with USB joysticks via a USB Host Shield and transmit flight commands wirelessly to DIY drones.

## Overview

Synapse is the control interface for your DIY drone project. It reads input from a USB joystick (specifically the Thrustmaster T16000M), processes the control signals, and transmits them via nRF24L01 radio module to the drone receiver. This project pairs with the [Cortex](https://github.com/sergiovirahonda/cortex) drone flight controller.

## Features

- 🎮 **USB Joystick Support**: Native support for Thrustmaster T16000M via USB Host Shield
- 📡 **Wireless Communication**: Reliable nRF24L01 radio transmission with optimized settings
- 📊 **Telemetry**: Receives live telemetry from the drone (PWM, roll, pitch) via ACK payloads
- 📺 **OLED Display**: 128×64 I2C OLED (SSD1306Ascii) for status and telemetry HUD—no framebuffer, RAM-friendly
- 🏗️ **Clean Architecture**: Modular design with adapters and models for maintainability
- ⚡ **Real-time Control**: Low-latency command transmission at 100Hz
- 🔧 **PlatformIO Integration**: Modern build system with dependency management

## Hardware Requirements

### Core Components
- **Arduino Nano** (ATmega328)
- **USB Host Shield** (for joystick connectivity)
- **nRF24L01** radio module
- **128×64 I2C OLED** (SSD1306, e.g. 0.96" at 0x3C)
- **Thrustmaster T16000M** joystick (or compatible HID device)

### Pin Connections

| Component | Pin | Arduino Pin |
|-----------|-----|-------------|
| nRF24L01 CE | - | D7 |
| nRF24L01 CSN | - | D8 |
| nRF24L01 MOSI | - | D11 (SPI) |
| nRF24L01 MISO | - | D12 (SPI) |
| nRF24L01 SCK | - | D13 (SPI) |
| OLED SDA (I2C) | - | A4 |
| OLED SCL (I2C) | - | A5 |

## Software Architecture

```
synapse/
├── src/
│   ├── main.cpp                 # Main control loop, OLED HUD
│   ├── adapters/
│   │   ├── joystick_adapter.*   # USB HID joystick parser
│   │   └── radio_adapter.*      # nRF24L01 TX + telemetry RX (ACK payload)
│   └── models/
│       ├── drone_command.*      # DroneCommand + DronePacket, TelemetryData + TelemetryPacket
│       └── joystick_input.*     # Joystick input model
└── platformio.ini               # Build configuration
```

### Key Components

- **T16000MParser**: Custom HID parser that extracts pitch, roll, yaw, throttle, and trim from joystick input
- **RadioAdapter**: nRF24L01 init, `send(DroneCommand)`, and `receiveTelemetry(TelemetryData)` via ACK payloads
- **DroneCommand** / **DronePacket**: Flight control and trim; packed wire format for TX
- **TelemetryData** / **TelemetryPacket**: Live telemetry (PWM, roll, pitch) from drone; packed wire format for RX
- **SSD1306AsciiWire**: Lightweight I2C OLED driver (no framebuffer) for status and telemetry display

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
   - `SSD1306Ascii` - I2C OLED driver (no framebuffer, RAM-friendly)

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

The radio adapter is configured for range, reliability, and telemetry:

- **Data Rate**: 250KBPS (longest range)
- **Power Level**: PA_HIGH
- **Channel**: 108 (above 2.48GHz to avoid WiFi interference)
- **Auto-ACK**: Enabled (required for telemetry via ACK payload)
- **Retries**: 2 retries, 3×250µs delay

### Joystick Mapping

The T16000M joystick is mapped as follows:

- **Pitch**: Forward/Backward stick movement (normalized to -100 to +100)
- **Roll**: Left/Right stick movement (normalized to -100 to +100)
- **Yaw**: Twist axis (normalized to -100 to +100)
- **Throttle**: Throttle slider (normalized to 0 to 100%)

### Command Format (TX → Drone)

Commands are transmitted as a packed struct:
```cpp
struct DronePacket {
    int16_t pitch;    // -100 to +100
    int16_t roll;     // -100 to +100
    int16_t yaw;      // -100 to +100
    int16_t throttle; // 0 to 100
    int16_t rollTrim; int16_t pitchTrim; int16_t yawTrim;
    uint8_t trimReset;
} __attribute__((packed));
```

### Telemetry (Drone → TX)

Telemetry is sent back from the drone in the **ACK payload** of each command packet. The TX calls `receiveTelemetry(telemetryData)` after `send()`; if the drone replied with an ACK payload, the data is written into the given `TelemetryData` object.

**Telemetry wire format** (packed):
```cpp
struct TelemetryPacket {
    int16_t pwm;   // Motor PWM
    int16_t roll;  // Roll (e.g. x100 for degrees)
    int16_t pitch; // Pitch (e.g. x100 for degrees)
} __attribute__((packed));
```

The Cortex receiver must send a `TelemetryPacket` (or compatible struct) as the ACK payload when it acknowledges a command. The TX display shows **Ack** (telemetry received), **PWM**, **Roll**, and **Pitch** in the TELEMETRY section of the OLED HUD.

## Usage

1. **Power on** the Arduino with USB Host Shield and OLED attached
2. **Connect** the Thrustmaster T16000M joystick to the USB Host Shield
3. **Verify** initialization on the OLED: "Radio OK", "USB Host OK", "All systems ready"
4. **Control** the drone by moving the joystick
5. **Monitor** the OLED HUD: SYSTEM READY (throttle, sent status) and TELEMETRY (Ack, PWM, Roll, Pitch). Telemetry updates when the drone responds with an ACK payload

## Troubleshooting

### Radio Not Connecting
- Verify nRF24L01 wiring (CE, CSN, SPI pins)
- Check that radio address matches receiver
- Ensure power supply provides stable 3.3V

### Joystick Not Detected
- Verify USB Host Shield is properly connected
- Check USB Host Shield power (may need external 5V)
- Ensure joystick is in HID mode

### OLED Not Initializing / Blank
- Use **SSD1306Ascii** (not Adafruit SSD1306); the Adafruit library needs a 1KB framebuffer and will fail on Nano with USB Host + RF24
- Check I2C wiring (SDA→A4, SCL→A5) and address (0x3C typical)

### Commands Not Received
- Verify radio address matches on both TX and RX
- Check radio channel and data rate settings
- Ensure receiver is powered and in listening mode

### Telemetry Not Updating (Ack always 0)
- Cortex must send a `TelemetryPacket` (or same layout) as the ACK payload when it ACKs a command
- Ensure ACK payload is enabled on the receiver and payload size ≥ sizeof(TelemetryPacket)
- Radio address and pipe must match so ACKs are received

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
