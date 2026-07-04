# ESPHome ADT7420 External Component

This repository contains an [ESPHome](https://esphome.io/) external component for the **ADT7420** high-accuracy, 16-bit digital I2C temperature sensor by Analog Devices.


## Installation & Configuration

To use this component in your ESPHome configuration, add this repository as an `external_component`:

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/HarpalPannu/esphome-adt7420
    components: [adt7420]

i2c:
  sda: 33
  scl: 32
  scan: True

sensor:
  - platform: adt7420
    temperature:
      name: "Temperature Sensor"
    address: 0x48           # I2C address (0x48 - 0x4B depending on A1/A0 pins)
    resolution: 16bit       # Select '16bit' or '13bit' (default is '16bit')
    update_interval: 15s
```

## Hardware Address Mapping

| A1 Pin | A0 Pin | I2C Address (7-bit) |
|--------|--------|---------------------|
| GND    | GND    | `0x48` (Default)    |
| GND    | VDD    | `0x49`              |
| VDD    | GND    | `0x4A`              |
| VDD    | VDD    | `0x4B`              |
