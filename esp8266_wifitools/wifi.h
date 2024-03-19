/* This software is licensed under the MIT License: https://github.com/spacehuhntech/esp8266_deauther */

#pragma once

#include <Arduino.h>
#include <dhcpserver.h>
#include <lwip/napt.h>
#include <lwip/dns.h>

namespace wifi {
    void begin();

    String getMode();
    String getNaptStatus();

    void printStatus();
    void initNapt();

    void connectNapt(String ssid, String pass);

    void startNewAP(String path, String ssid, String password, uint8_t ch, bool hidden, bool captivePortal);
    void startEvilTwin(String apn);
    // void startAP(String path);
    void startWebServer();
    void startAP();

    void stopAP();
    void resumeAP();

    void update();
}