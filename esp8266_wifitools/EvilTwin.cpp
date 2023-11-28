#include "EvilTwin.h"
#include "ESP8266WiFi.h"
#include "DNSServer.h"
#include "ESP8266WebServer.h"

#include "wifi.h"

String EvilTwin::ssidT = "";
String EvilTwin::pass = "";
String EvilTwin::passTesting = "";

void EvilTwin::start(const char* ap) {
    EvilTwin::ssidT = ap;
    wifi::stopAP();
    wifi::startEvilTwin(ap);
}

void EvilTwin::stop() {
    wifi::stopAP();
    wifi::startAP();
}

String EvilTwin::getpass() {
    return EvilTwin::pass;
}

bool EvilTwin::isRunning() {
    return WiFi.softAPSSID() == EvilTwin::ssidT;
}

void EvilTwin::update(){
}
