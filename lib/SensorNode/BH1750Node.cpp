#include "BH1750Node.hpp"

BH1750Node::BH1750Node(const char *id,  const char *name, const int i2cAddress) : SensorNode(id, name, "BH1750"), _bh1750(i2cAddress) {
  Wire.begin();
  if(!_bh1750.begin())
    Homie.getLogger() << F("Could not find a valid BH1750 sensor, check wiring!");

  _luxFactor = new HomieSetting<double>("bh1750_lux_factor", "The temperature offset in degrees [-10.0 .. 10.0] Default = 1");
  _luxFactor->setDefaultValue(1.0f).setValidator([](float candidate) {return (candidate >= -10.0f) && (candidate <= 10.0f);});

  _luxOffset = new HomieSetting<double>("bh1750_lux_offset", "The temperature factor in degrees [-10.0 .. 10.0] Default = 0");
  _luxOffset->setDefaultValue(0.0f).setValidator([](float candidate) {return (candidate >= -10.0f) && (candidate <= 10.0f);});
}

BH1750Node::~BH1750Node(){}

void 
BH1750Node::setup() {
  Homie.getLogger() << F("Calling Node Setup... Node Name: ") << getName() << endl;
  
  advertise(LUX_TOPIC).setName("BH1750 luxometer")
                      .setDatatype("float")
                      .setFormat("0:65000")
                      .setUnit("lux");
}

void
BH1750Node::onReadyToOperate() {
    Homie.getLogger() << F("Calling Ready To Operate... Node Name: ") << getName() << endl;
    takeMeasurement();
    sendMeasurement();
}

void
BH1750Node::sendMeasurement(){
  if(Homie.isConnected()) {
    setProperty(LUX_TOPIC).send(String(_lux, 3).c_str());
    Homie.getLogger() << F("Sending Measurement: Lux: ") << _lux << F(" lux") << endl;
  }
}

void
BH1750Node::takeMeasurement(){
   if (_bh1750.measurementReady()) {
    _lux = _bh1750.readLightLevel() * _luxFactor->get() + _luxOffset->get();
    if (_lux == -2) {
      _lux = NAN;
    }
  }
}