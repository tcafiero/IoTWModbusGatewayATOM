#include "contract.h"
// Include the header for the ModbusClient RTU style
#include "ModbusClientRTU.h"

//struct contract_struct contract={.baudrate=921600, .root="IoThingsWare", .plant="unina", .unit="DipAgrAirHeritage1", .num_servers=1,
#if 0
struct contract_struct contract = {.baudrate = 9600, .root = "IoThingsWare", .plant = "Bestico", .unit = "IndonesiaT1", .num_servers = 10,

  .server = {
    {.address = 1, .name = "SQ-212-SS", .num_sensors = 2, .sensors_address = 0, .sensor = {{.name = "PPFD"}, {.name = "ePPFD"}}},
    {.address = 2, .name = "WeatherStation", .num_sensors = 2, .sensors_address = 0, .sensor = {{.name = "WindSpeed"}, {.name = "WindDirection"}}},
    {.address = 3, .name = "Fake", .num_sensors = 1, .sensors_address = 0, .sensor = {{.name = "counter"}}, .num_actuators = 1, .actuators_address = 8, .actuator = {{.name = "threshold"}}},
    {.address = 4, .name = "SCD30", .num_sensors = 3, .sensors_address = 0, .sensor = {{.name = "Temperature"}, {.name = "Humidity"}, {.name = "CO2"}}},
    {.address = 5, .name = "SHT20", .num_sensors = 2, .sensors_address = 0, .sensor = {{.name = "Temperature"}, {.name = "Humidity"}}},
    {.address = 6, .name = "pH", .num_sensors = 1, .sensors_address = 0, .sensor = {{.name = "pH"}}},
    {.address = 7, .name = "DS18B20", .num_sensors = 1, .sensors_address = 0, .sensor = {{.name = "Temperature"}}},
    {.address = 8, .name = "O2", .num_sensors = 1, .sensors_address = 0, .sensor = {{.name = "O2"}}},
    {.address = 9, .name = "Turbidity", .num_sensors = 1, .sensors_address = 0, .sensor = {{.name = "Turbidity"}}},
    {.address = 10, .name = "Spectrometer", .num_sensors = 9, .sensors_address = 0, .sensor = {{.name = "Lux"}, {.name = "F1"}, {.name = "F2"}, {.name = "F3"}, {.name = "F4"}, {.name = "F5"}, {.name = "F6"}, {.name = "F7"}, {.name = "F8"}}}
  }
#endif
struct contract_struct contract = {.baudrate = 9600, .root = "IoThingsWare", .plant = "Bestico", .unit = "IndonesiaT1", .num_servers = 5,
  .server = {
    {.address = 0x20, .function = READ_INPUT_REGISTER, .name = "XY-MD02", .num_sensors = 2, .sensors_address = 1, .sensor = {{.name = "Temperature", .factor = 0.1}, {.name = "RH", .factor=0.1}}},
    {.address = 0x21, .function = READ_HOLD_REGISTER, .name = "SQ-522", .num_sensors = 1, .sensors_address = 40, .sensor = {{.name = "PPFD", .factor = 0.1}}},
    {.address = 0x22, .function = READ_HOLD_REGISTER, .name = "S-Temp&VWC", .num_sensors = 2, .sensors_address = 0, .sensor = {{.name = "Temperature", .factor = 0.01}, {.name = "VWC", .factor=0.01}}},
    {.address = 0x23, .function = READ_HOLD_REGISTER, .name = "SenseCAP-CO2", .num_sensors = 1, .sensors_address = 0, .sensor = {{.name = "CO2", .factor = 1.0}}},
    {.address = 0x0B, .function = READ_HOLD_REGISTER, .name = "S-YM-01", .num_sensors = 2, .sensors_address = 0, .sensor = {{.name = "Temperature", .factor = 0.01}, {.name = "Wetness", .factor = 0.01}}},
    {.address = 0x0B, .function = READ_HOLD_REGISTER, .name = "S-YM-01", .num_sensors = 2, .sensors_address = 0, .sensor = {{.name = "Temperature", .factor = 0.01}, {.name = "Wetness", .factor = 0.01}}}
}};
//END WARNING
