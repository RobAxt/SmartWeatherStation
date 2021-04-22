#include <Arduino.h>
#include <Homie.h>
#include "SyslogStream.hpp"
#include "SHT30Node.hpp"
#include "BH1750Node.hpp"
#include "BMP180Node.hpp"

#define FW_NAME "Smart Weather Station"
#define FW_VERSION "0.0.0" 

/* Magic sequence for Autodetectable Binary Upload */
const char *__FLAGGED_FW_NAME = "\xbf\x84\xe4\x13\x54" FW_NAME "\x93\x44\x6b\xa7\x75";
const char *__FLAGGED_FW_VERSION = "\x6a\x3f\x3e\x0e\xe1" FW_VERSION "\xb0\x30\x48\xd4\x1a";
/* End of magic sequence for Autodetectable Binary Upload */

SHT30Node sht30Node("sht30", "SHT30 Temp. and Humedity Sensor Node");
BMP180Node bmp180Node("bmp180", "BMP180 Temp. and Pressure Sensor Node");
BH1750Node bh1750Node("bh1750", "BH1750 Luxometer Sensor Node");

void setup() {
#if DEBUG_APP
  Serial.begin(SERIAL_SPEED);
  Serial << endl << endl;

  SyslogStream.setup();
  SyslogStream.deviceHostname("");
  SyslogStream.appName("");
  SyslogStream.setSyslogLevel(LOG_INFO);

  Homie.setLoggingPrinter(&SyslogStream);
  Homie.getLogger() << F("Build Date and Time: ") << __DATE__ << " & " << __TIME__ << endl;
#else
  Homie.disableLogging();
#endif
  Homie_setFirmware(FW_NAME, FW_VERSION); // The underscore is not a typo! See Magic bytes
  Homie.setup();
}

void loop() {
  Homie.loop();
}