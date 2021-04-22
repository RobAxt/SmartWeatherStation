#include "SHT30Node.hpp"

SHT30Node::SHT30Node(const char *id,  const char *name, const int i2cAddress) : SensorNode(id, name, "SHT30"), _sht30(i2cAddress, SHT3x::PrevValue){
  Wire.begin();
  _sht30.Begin();
  
  _temperatureFactor = new HomieSetting<double>("sht30_temperature_factor", "The temperature offset in degrees [-10.0 .. 10.0] Default = 1");
  _temperatureFactor->setDefaultValue(1.0f).setValidator([](float candidate) {return (candidate >= -10.0f) && (candidate <= 10.0f);});

  _temperatureOffset = new HomieSetting<double>("sht30_temperature_offset", "The temperature factor in degrees [-10.0 .. 10.0] Default = 0");
  _temperatureOffset->setDefaultValue(0.0f).setValidator([](float candidate) {return (candidate >= -10.0f) && (candidate <= 10.0f);});

  _humidityFactor = new HomieSetting<double>("sht30_humidity_factor", "The humidity factor [-10.0 .. 10.0] Default = 1");
  _humidityFactor->setDefaultValue(1.0f).setValidator([](float candidate) {return (candidate >= -10.0f) && (candidate <= 10.0f);});

  _humidityOffset = new HomieSetting<double>("sht30_humidity_offset", "The humidity offset [-10.0 .. 10.0] Default = 0");
  _humidityOffset->setDefaultValue(0.0f).setValidator([](float candidate) {return (candidate >= -10.0f) && (candidate <= 10.0f);});
}

SHT30Node::~SHT30Node(){}

void 
SHT30Node::setup() {
  Homie.getLogger() << F("Calling Node Setup... Node Name: ") << getName() << endl;
  
  advertise(TMP_TOPIC).setName("SHT30 Temperature")
                      .setDatatype("float")
                      .setFormat("-40:85")
                      .setUnit("°C");
  advertise(HUM_TOPIC).setName("SHT30 Relative Humidity")
                      .setDatatype("float")
                      .setFormat("0:100")
                      .setUnit("%");
  
  SHT3x::CalibrationFactors TemperatureCalibration;
  TemperatureCalibration.Factor = _temperatureFactor->get();
  TemperatureCalibration.Shift = _temperatureOffset->get();
  _sht30.SetTemperatureCalibrationFactors(TemperatureCalibration);
  
  SHT3x::CalibrationFactors RelHumidityCalibration;
  RelHumidityCalibration.Factor = _humidityFactor->get();
  RelHumidityCalibration.Shift  = _humidityOffset->get();
  _sht30.SetRelHumidityCalibrationFactors(RelHumidityCalibration);
}

void
SHT30Node::onReadyToOperate() {
    Homie.getLogger() << F("Calling Ready To Operate... Node Name: ") << getName() << endl;
    takeMeasurement();
    sendMeasurement();
}

void
SHT30Node::sendMeasurement(){
  if(Homie.isConnected()) {
    setProperty(TMP_TOPIC).send(String(_temperature, 3).c_str());
    setProperty(HUM_TOPIC).send(String(_humidity, 3).c_str());
    Homie.getLogger() << F("Sending Measurement: Temp.: ") << _temperature << F(" °C - Hum.: ") << _humidity << F(" %") << endl;
  }
}

void
SHT30Node::takeMeasurement(){
  _sht30.UpdateData();
  if(!_sht30.GetError()){
    _temperature = _sht30.GetTemperature(SHT3x::Cel);
    _humidity = _sht30.GetRelHumidity();  
  } else
    Homie.getLogger() << F("Fail to Retrieve Measurement: Reason ") << _sht30.GetError() << endl;
}