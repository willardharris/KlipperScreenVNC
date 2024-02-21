/* WiFi settings */
const char *SSID_NAME = "yourssid";
const char *SSID_PASSWORD = "yourpass";

const char *VNC_IP = "printerip";
const uint16_t VNC_PORT = 5901;
const char *VNC_PASSWORD = "notused"; //not used

#include "touch.h"

#include <Arduino_GFX_Library.h>

#define GFX_DEV_DEVICE WAVESHARE_ESP32_S3_TFT_4_3


Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
  5 /* DE */,
  3 /* VSYNC */,
  46 /* HSYNC */,
  7 /* PCLK */,

  1 /* R0 */,
  2 /* R1 */,
  42 /* R2 */,
  41 /* R3 */,
  40 /* R4 */,

  39 /* G0 */,
  0 /* G1 */,
  45 /* G2 */,
  48 /* G3 */,
  47 /* G4 */,
  21 /* G5 */,

  14 /* B0 */,
  38 /* B1 */,
  18 /* B2 */,
  17 /* B3 */,
  10 /* B4 */,

  0 /* hsync_polarity */, 40 /* hsync_front_porch */, 48 /* hsync_pulse_width */, 88 /* hsync_back_porch */,
  0 /* vsync_polarity */, 13 /* vsync_front_porch */, 3 /* vsync_pulse_width */, 32 /* vsync_back_porch */,
  1 /* pclk_active_neg */, 16000000 /* prefer_speed */
);

Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
  800 /* width */,
  480 /* height */,
  rgbpanel,
  0 /* rotation */,
  true /* auto_flush */
);

#include <WiFi.h>


#include "VNC_GFX.h"
#include <VNC.h>

VNC_GFX *vnc_gfx = new VNC_GFX(gfx);
arduinoVNC vnc = arduinoVNC(vnc_gfx);

void TFTnoWifi(void) {
  gfx->fillScreen(BLACK);
  gfx->setCursor(0, ((gfx->height() / 2) - (5 * 8)));
  gfx->setTextColor(RED);
  gfx->setTextSize(5);
  gfx->println("NO WIFI!");
  gfx->setTextSize(2);
  gfx->println();
}

void TFTnoVNC(void) {
  gfx->fillScreen(BLACK);
  gfx->setCursor(0, ((gfx->height() / 2) - (4 * 8)));
  gfx->setTextColor(GREEN);
  gfx->setTextSize(4);
  gfx->println("connect VNC");
  gfx->setTextSize(2);
  gfx->println();
  gfx->print(VNC_IP);
  gfx->print(":");
  gfx->println(VNC_PORT);
}

void handle_touch() {
  static bool touchHandled = false;
  static unsigned long lastTouchTime = 0;
  const unsigned long debounceTime = 100;  // 50 milliseconds debounce time

  if (touch_has_signal()) {
    if (touch_touched() && !touchHandled && (millis() - lastTouchTime > debounceTime)) {
      vnc.mouseEvent(touch_last_x, touch_last_y, 0b001);
      touchHandled = true;
      lastTouchTime = millis();
    } else if (touch_released() && touchHandled) {
      vnc.mouseEvent(touch_last_x, touch_last_y, 0b000);
      touchHandled = false;
      lastTouchTime = millis();
    }
  }
}


void setup(void) {
  Serial.begin(115200);

  Serial.println("ESP32 VNC Client");

#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif


  Serial.println("Init display");
  if (!gfx->begin()) {
    Serial.println("gfx->begin() failed!");
  }
  gfx->fillScreen(BLACK);

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif

  // Init touch device
  touch_init(gfx->width(), gfx->height(), gfx->getRotation());

  TFTnoWifi();

  Serial.println("Init WiFi");
  gfx->println("Init WiFi");

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID_NAME, SSID_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    gfx->print(".");
  }
  Serial.println(" CONNECTED");
  gfx->println(" CONNECTED");
  Serial.println("IP address: ");
  gfx->println("IP address: ");
  Serial.println(WiFi.localIP());
  gfx->println(WiFi.localIP());
  TFTnoVNC();

  Serial.println(F("[SETUP] VNC..."));

  vnc.begin(VNC_IP, VNC_PORT);
  vnc.setPassword(VNC_PASSWORD);  // optional
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    vnc.reconnect();
    TFTnoWifi();
    delay(100);
  } else {
    if (vnc.connected()) {
      handle_touch();
    }
    vnc.loop();
    if (!vnc.connected()) {
      TFTnoVNC();
      delay(5000);
    }
  }
}
