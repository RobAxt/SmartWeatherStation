#include "BMP180Node.hpp"

BMP180Node::BMP180Node(const char *id,  const char *name, const int i2cAddress) : SensorNode(id, name, "BMP180"), _bmp180() {
  _bmp180.begin(BMP085_ULTRALOWPOWER);

  _temperatureFactor = new HomieSetting<double>("bmp180_temperature_factor", "The temperature offset in degrees [-10.0 .. 10.0] Default = 1");
  _temperatureFactor->setDefaultValue(1.0f).setValidator([](float candidate) {return (candidate >= -10.0f) && (candidate <= 10.0f);});

  _temperatureOffset = new HomieSetting<double>("bmp180_temperature_offset", "The temperature factor in degrees [-10.0 .. 10.0] Default = 0");
  _temperatureOffset->setDefaultValue(0.0f).setValidator([](float candidate) {return (candidate >= -10.0f) && (candidate <= 10.0f);});

  _pressureFactor = new HomieSetting<double>("bmp180_pressure_factor", "The pressure factor [-10.0 .. 10.0] Default = 1");
  _pressureFactor->setDefaultValue(1.0f).setValidator([](float candidate) {return (candidate >= -10.0f) && (candidate <= 10.0f);});

  _pressureOffset = new HomieSetting<double>("bmp180_pressure_offset", "The pressure offset [-10.0 .. 10.0] Default = 0");
  _pressureOffset->setDefaultValue(0.0f).setValidator([](float candidate) {return (candidate >= -10.0f) && (candidate <= 10.0f);});
}

BMP180Node::~BMP180Node() {}

void 
BMP180Node::setup() {
  Homie.getLogger() << F("Calling Node Setup... Node Name: ") << getName() << endl;
  
  advertise(TMP_TOPIC).setName("BMP180 Temperature")
                      .setDatatype("float")
                      .setFormat("-40:85")
                      .setUnit("°C");
  advertise(PRES_TOPIC).setName("BMP180 Pressure")
                      .setDatatype("float")
                      .setFormat("300:1100")
                      .setUnit("hPa");
}

void
BMP180Node::onReadyToOperate() {
    Homie.getLogger() << F("Calling Ready To Operate... Node Name: ") << getName() << endl;
    takeMeasurement();
    sendMeasurement();
}

void
BMP180Node::sendMeasurement(){
  if(Homie.isConnected()) {
    setProperty(TMP_TOPIC).send(String(_temperature, 3).c_str());
    setProperty(PRES_TOPIC).send(String(_pressure, 3).c_str());
    Homie.getLogger() << F("Sending Measurement: Temp.: ") << _temperature << F(" °C - Press.: ") << _pressure << F(" hPa") << endl;
  }
}

void
BMP180Node::takeMeasurement(){
  _temperature = _bmp180.readTemperature() * _temperatureFactor->get() + _temperatureOffset->get(); 
  _pressure = _bmp180.readPressure() * _pressureFactor->get() / 100 + _pressureOffset->get(); 
}