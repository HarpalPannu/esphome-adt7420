import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    CONF_ID,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
    DEVICE_CLASS_TEMPERATURE,
    CONF_TEMPERATURE
)

DEPENDENCIES = ['i2c']
CODEOWNERS = ['Pannu']

# Define namespace and component class
adt7420_ns = cg.esphome_ns.namespace('adt7420')
Adt7420 = adt7420_ns.class_(
    'Adt7420', 
    i2c.I2CDevice,
    cg.PollingComponent
)


CONF_RESOLUTION = "resolution"

# Define configuration schema
CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Adt7420),
            cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_RESOLUTION, default="16bit"): cv.one_of(
                "13bit", "16bit"
            ),
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x48))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    if CONF_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATURE])
        cg.add(var.set_temperature(sens))
    cg.add(var.set_resolution(config[CONF_RESOLUTION] == "16bit"))