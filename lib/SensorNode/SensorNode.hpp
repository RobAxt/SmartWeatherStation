#ifndef SensorNode_hpp
#define SensorNode_hpp

#include <Homie.hpp>

class SensorNode : public HomieNode {
  public:
    static const int MEASURE_INTERVAL =  1 * 1000UL;  // every second
    static const int SEND_INTERVAL    = 60 * 1000UL;  // every minute

    explicit SensorNode(const char *id, const char *name, const char *type,  const int measureInterval = MEASURE_INTERVAL, const int sendInterval = SEND_INTERVAL);
    virtual ~SensorNode();

  protected:
    virtual void sendMeasurement() = 0;
    virtual void takeMeasurement() = 0;
    virtual void loop() override;

  private:
    unsigned long _measureInterval;
    unsigned long _sendInterval;
    unsigned long _lastMeasureTime;
    unsigned long _lastSendTime;    
};

#endif //SensorNode_hpp