/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/cademissner/Desktop/IoT/Lab20-TrafficLights/src/Lab20-TrafficLights.ino"
void setLEDs(char* redN, char* yellowN, char* greenN, char* redE, char* yellowE, char* greenE);
void setup();
void loop();
#line 1 "/Users/cademissner/Desktop/IoT/Lab20-TrafficLights/src/Lab20-TrafficLights.ino"
SYSTEM_THREAD(ENABLED);

#include "MQTT.h"

void callback(char* topic, byte* payload, unsigned int length);
MQTT client("lab.thewcl.com", 1883, callback);

enum State {NORTH, EAST};
State currentState = NORTH;
State prevState = NORTH;

void callback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;

  if (String(p).equals("true")) {
    prevState = currentState;
    if (String(topic).equals("cade/eastsensor")) {
      currentState = EAST;
    } else if (String(topic).equals("cade/northsensor")) {
      currentState = NORTH;
    }
  }
}

void setLEDs(char* redN, char* yellowN, char* greenN, char* redE, char* yellowE, char* greenE) {
  client.publish("cade/northred", redN);
  client.publish("cade/northyellow", yellowN);
  client.publish("cade/northgreen", greenN);
  client.publish("cade/eastred", redE);
  client.publish("cade/eastyellow", yellowE);
  client.publish("cade/eastgreen", greenE);
}

void setup() {
}

void loop() {
  if (client.isConnected()) {
    client.loop();
  } else {
    client.connect(System.deviceID());
    client.subscribe("cade/eastsensor");
    client.subscribe("cade/northsensor");
  }

  switch (currentState) {
    case NORTH:
    if (prevState != NORTH) {
      setLEDs("true", "false", "false", "false", "true", "false");
      delay(5000);
      setLEDs("false", "false", "true", "true", "false", "false");
      prevState = NORTH;
    }
    break;

    case EAST:
    if (prevState != EAST) {
      setLEDs("false", "true", "false", "true", "false", "false");
      delay(5000);
      setLEDs("true", "false", "false", "false", "false", "true");
      prevState = EAST;
    }
    break;
  }
}