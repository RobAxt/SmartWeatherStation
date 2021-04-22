#ifndef SHT30Node_hpp
#define SHT30Node_hpp

#include <Wire.h>
#include <SHT3x.h>
#include "SensorNode.hpp"

class SHT30Node : public SensorNode {
  public:
    explicit SHT30Node(const char *id,  const char *name, const int i2cAddress = 0x45);
    ~SHT30Node();

  protected:
    virtual void setup() override;
    virtual void onReadyToOperate() override;
    virtual void sendMeasurement() override;
    virtual void takeMeasurement() override;

  private:
    const char* TMP_TOPIC = "temperature";
    const char* HUM_TOPIC = "humidity";

    HomieSetting<double> *_humidityFactor;
    HomieSetting<double> *_humidityOffset;
    HomieSetting<double> *_temperatureFactor;
    HomieSetting<double> *_temperatureOffset;

    SHT3x _sht30;

    float _temperature = NAN;
    float _humidity = NAN;
};

#endif //SHT30Node_hpp
//https://www.wemos.cc/en/latest/d1_mini_shield/sht30.html
//https://github.com/Risele/SHT3x.git#master