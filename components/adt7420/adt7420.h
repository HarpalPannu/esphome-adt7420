#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"

namespace esphome {
namespace adt7420 {

class Adt7420 : public i2c::I2CDevice, public PollingComponent {
 public:
  void set_temperature(sensor::Sensor *temperature) {
    this->temperature_adt7420 = temperature;
  }
  void set_resolution(bool is_16_bit) {
    this->is_16_bit_ = is_16_bit;
  }
  void setup() override;
  void update() override;
  float get_setup_priority() const override;

 protected:
  sensor::Sensor *temperature_adt7420{nullptr};
  bool is_16_bit_{true};
  float convert_raw_temp(uint16_t raw_value);
  void process_reading();
};

}  // namespace adt7420
}  // namespace esphome