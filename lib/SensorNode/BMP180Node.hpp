#ifndef BMP180_hpp
#define BMP180_hpp

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP085.h>
#include "SensorNode.hpp"

class BMP180Node : public SensorNode {
  public:
    explicit BMP180Node(const char *id,  const char *name, const int i2cAddress = 0x77);
    ~BMP180Node();

  protected:
    virtual void setup() override;
    virtual void onReadyToOperate() override;
    virtual void sendMeasurement() override;
    virtual void takeMeasurement() override;

  private:
    const char* TMP_TOPIC  = "temperature";
    const char* PRES_TOPIC = "pressure";

    HomieSetting<double> *_temperatureFactor;
    HomieSetting<double> *_temperatureOffset;
    HomieSetting<double> *_pressureFactor;
    HomieSetting<double> *_pressureOffset;

    Adafruit_BMP085 _bmp180;

    float _temperature = NAN;
    float _pressure = NAN;
};

#endif //BMP180_hpp
//https://forum.mysensors.org/topic/1296/bmp180-breakout-board-on-low-voltage/2
//https://github.com/adafruit/Adafruit-BMP085-Library/blob/HEAD/Adafruit_BMP085.h