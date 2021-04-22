#ifndef BH1750Node_hpp
#define BH1750Node_hpp

#include <Wire.h>
#include <BH1750.h>
#include "SensorNode.hpp"

class BH1750Node : public SensorNode {
  public:
    explicit BH1750Node(const char *id,  const char *name, const int i2cAddress = 0x23);
    ~BH1750Node();

  protected:
    virtual void setup() override;
    virtual void onReadyToOperate() override;
    virtual void sendMeasurement() override;
    virtual void takeMeasurement() override;

  private:
    const char* LUX_TOPIC = "lux";

    HomieSetting<double> *_luxFactor;
    HomieSetting<double> *_luxOffset;
    
    BH1750 _bh1750;

    float _lux = NAN;
};

#endif //BH1750Node_hpp
//http://wiki.sunfounder.cc/index.php?title=GY-302_BH1750_Digital_Light_Intensity_Module
//https://github.com/claws/BH1750.git#master