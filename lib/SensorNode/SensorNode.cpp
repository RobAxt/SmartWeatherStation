#include "SensorNode.hpp"

SensorNode::SensorNode(const char *id, const char *name, const char *type, const int measureInterval, const int sendInterval) : HomieNode(id, name, type), _measureInterval(measureInterval), _sendInterval(sendInterval), _lastMeasureTime(0), _lastSendTime(0) {
  setRunLoopDisconnected(true);
}

SensorNode::~SensorNode(){}

void SensorNode::loop()
{
  unsigned long now = millis();
  if (now - _lastMeasureTime >= _measureInterval || _lastMeasureTime == 0) {
    takeMeasurement();
    _lastMeasureTime = now;
  }

  if (now - _lastSendTime >= _sendInterval || _lastSendTime == 0) {
    if(Homie.isConnected())
      sendMeasurement();
    _lastSendTime = now;
  }
}
