#include "adt7420.h"

#include "esphome/core/hal.h"
#include "esphome/core/log.h"
namespace esphome {
namespace adt7420 {

static const char *const TAG = "adt7420";

// Register addresses
static const uint8_t ADT7420_REG_TEMP_MSB = 0x00;
static const uint8_t ADT7420_REG_CONFIG = 0x03;
static const uint8_t ADT7420_REG_ID = 0x0B;
static const uint8_t ADT7420_REG_RESET = 0x2F;

void Adt7420::setup() {
  ESP_LOGD(TAG, "Setting Up Adt7420 Sensor");

  // Perform software reset
  if (!this->write_byte(ADT7420_REG_RESET, 0x00)) {
    ESP_LOGE(TAG, "Failed to reset sensor!");
    this->mark_failed();
    return;
  }
  // Wait at least 200 µs (using 50ms to be safe for power-up and reset completion)
  delay(50);

  // Verify ID register
  uint8_t id_val = 0;
  if (!this->read_byte(ADT7420_REG_ID, &id_val)) {
    ESP_LOGE(TAG, "Failed to read ID register!");
    this->mark_failed();
    return;
  }

  if (id_val != 0xCB) {
    ESP_LOGW(TAG, "Invalid ID register value: 0x%02X (expected 0xCB). Proceeding anyway...", id_val);
  } else {
    ESP_LOGD(TAG, "Sensor ID verified successfully: 0x%02X", id_val);
  }

  // Set configuration
  // Bit 7: Resolution (1 for 16-bit, 0 for 13-bit)
  // Bits 6-5: Operation mode (00 for continuous conversion)
  uint8_t config = 0x00;
  if (this->is_16_bit_) {
    config |= 0x80;
  }

  if (!this->write_byte(ADT7420_REG_CONFIG, config)) {
    ESP_LOGE(TAG, "Failed to write configuration register!");
    this->mark_failed();
    return;
  }
}

void Adt7420::update() {
  this->process_reading();
}

float Adt7420::get_setup_priority() const { return setup_priority::DATA; }

float Adt7420::convert_raw_temp(uint16_t raw_value) {
  if (this->is_16_bit_) {
    int16_t value_16 = static_cast<int16_t>(raw_value);
    return value_16 / 128.0f;
  } else {
    int16_t value_13 = raw_value >> 3;
    if (value_13 & 0x1000) { // Bit 12 is sign bit
      value_13 -= 8192;
    }
    return value_13 / 16.0f;
  }
}

void Adt7420::process_reading() {
  uint8_t data[2];
  if (!this->read_bytes(ADT7420_REG_TEMP_MSB, data, 2)) {
    ESP_LOGE(TAG, "Failed to read temperature data!");
    this->status_set_warning();
    return;
  }

  uint16_t raw_temp = (data[0] << 8) | data[1];
  float temperature = this->convert_raw_temp(raw_temp);

  ESP_LOGD(TAG, "Read temperature: %.4f °C", temperature);
  if (this->temperature_adt7420 != nullptr) {
    this->temperature_adt7420->publish_state(temperature);
  }
  this->status_clear_warning();
}

}  // namespace adt7420
}  // namespace esphome