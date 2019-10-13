/************************************************************************************************/
/*      _____                                        |       ________  ______         ____
 *      |    \                   |                   |       |         |     \       /    \
 *      |     |   o              |                   |       |         |      \     |
 *      |     |         _____    |   /   |       |   |       |         |       |    |
 *      |____/    |   |/     \   |  /    |       |   |       |______   |       |     \____
 *      |         |   |      |   | /     |       |   |       |         |       |          \
 *      |         |   |      |   |/\     |       |   |       |         |       |           |
 *      |         |   |      |   |  \     \_____/    |       |         |      /            |
 *      |         |   |      |   |   \         /     |       |_______  |_____/      \_____/ 
 *   \________________________________________/      |________________________________________/
 */
#define VERSION "0.9.4dev"

#include <ArduinoJson.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <ESPmDNS.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
#endif

#include <PubSubClient.h>

#include "effects.h"

#include "config.h"

#include <ArduinoOTA.h>

#ifdef ENABLE_E131
  #include <ESPAsyncE131.h>

  #if (UNIVERSE_START < 1) || (UNIVERSE_START > 63999)
    #error "UNIVERSE_START has to be 1 - 63999, check config.h"
  #endif
#endif

#if defined(LED_BUILTIN) || defined(FORCE_USE_LED_BUILTIN)
  #define USE_LED_BUILTIN 1
#endif

#if defined(BRIGHTNESS_ENCODER_DT) || defined(SPEED_ENCODER_DT)
  #include <RotaryEncoder.h>
#endif

#ifdef ARDUINO_ESP8266_NODEMCU
  #define HW_PLATFORM "NodeMCU"
#elif defined(ARDUINO_ESP8266_WEMOS_D1MINI)
  #define HW_PLATFORM "D1 mini"
#elif defined(ESP32)
  #define HW_PLATFORM "ESP32"
#else
  #define HW_PLATFORM "other"
#endif

//#define VERSION_FULL VERSION " " HW_PLATFORM
#define VERSION_FULL VERSION " " ARDUINO_VARIANT

#ifdef ESP32
  #define LED_ON HIGH
  #define LED_OFF LOW
#else
  #define LED_ON LOW
  #define LED_OFF HIGH
#endif

int OTAport = 8266;

const CRGB colorList[] = {{255,0,0}, {0,255,0}, {0,0,255}, {0,255,127}, {191,255,0},
                        {255,255,255}, {255,255,36}, {255,191,0}, {255,127,0}, {255,163,72},
                        {255,36,36}, {255,72,118}, {255,0,127}, {255,0,255}, {191,0,255},
                        {145,36,255}, {127,0,255}, {91,36,255}, {63,0,255}, {109,109,255},
                        {72,72,255}, {0,63,255}, {36,91,255}, {109,145,255}, {0,127,255},
                        {72,163,255}, {0,191,255}, {72,209,255}, {36,255,255}, {109,255,255},
                        {255,109,109}, {163,72,255}};

enum eEffects {
  Confetti = 0, Glitter  , Juggle       , Sinelon     , Solid         ,
  Christmas   , CandyCane, HollyJolly   , Valentine   , LoveyDay      ,
  StPatty     ,  Easter  , Usa          , Independence, GoBlue        ,
  Hail        , Touchdown, Halloween    , Punkin      , Thanksgiving  ,
  TurkeyDay   , Bpm      , CyclonRainbow, Dots        , Fire          ,
  Lightning   , PoliceAll, PoliceOne    , Rainbow     , GlitterRainbow,
  Ripple      , Twinkle,
  #ifdef ENABLE_E131
    E131,
  #endif
  Max
};

// Overload the ControlType++ operator
inline eEffects& operator++(eEffects& eDOW, int)  // <--- note -- must be a reference
{
	const int i = static_cast<int>(eDOW)+1;
	eDOW = static_cast<eEffects>((i) % eEffects::Max);
	return eDOW;
}

const char effectList[][20] = { "Confetti",
                                "Glitter",
                                "Juggle",
                                "Sinelon",
                                "Solid",
                                "Christmas",
                                "Candy Cane",
                                "Holly Jolly",
                                "Valentine",
                                "Lovey Day",
                                "St Patty",
                                "Easter",
                                "USA",
                                "Independence",
                                "Go Blue",
                                "Hail",
                                "Touchdown",
                                "Halloween",
                                "Punkin",
                                "Thanksgiving",
                                "Turkey Day",
                                "BPM",
                                "Cyclon Rainbow",
                                "Dots",
                                "Fire",
                                "Lightning",
                                "Police All",
                                "Police One",
                                "Rainbow",
                                "Glitter Rainbow",
                                "Ripple",
                                "Twinkle"
                                #ifdef ENABLE_E131
                                , "E131"
                                #endif
                              };


/****************************** MQTT TOPICS (change these topics as you wish)  ***************************************/
#ifdef USE_WHITE_BALANCE_FOR_SPEED
  #define SPEEDTOPIC "white_value"
  #define WHITE_VALUE "true"
#else
  #define SPEEDTOPIC "speed"
  #define WHITE_VALUE "false"
#endif
#define mqttcommand "cmnd/" DEVICE_NAME
#define mqttstate "stat/" DEVICE_NAME
#define LWTTOPIC "LWT/" DEVICE_NAME

#ifdef USE_DISCOVERY
  #define DISCOVERY_TOPIC "homeassistant/light/" DEVICE_NAME "/config"
  #define DISCOVERY_BASE "{ \"unique_id\": \"PinkyLED_" DEVICE_NAME "\", " \
                           "\"device\":{" \
                            "\"identifiers\":\"" DEVICE_NAME "\", " \
                            "\"model\": \"generic\", " \
                            "\"manufacturer\": \"Pinkywafer\", " \
                            "\"name\": \"" DEVICE_NAME "\", " \
                            "\"sw_version\": \"" VERSION_FULL "\"" \
                           "}, " \
                           "\"name\": \"" DEVICE_NAME "\", " \
                           "\"platform\": \"mqtt\", " \
                           "\"schema\": \"json\", " \
                           "\"state_topic\": \"" mqttstate "\", " \
                           "\"command_topic\": \"" mqttcommand "\", " \
                           "\"white_value\": \"" WHITE_VALUE "\", " \
                           "\"optimistic\": \"false\", " \
                           "\"availability_topic\": \"" LWTTOPIC "\", " \
                           "\"payload_available\": \"Online\", " \
                           "\"payload_not_available\": \"Offline\", " \
                           "\"rgb\": \"true\", " \
                           "\"flash_time_short\": \"1\", " \
                           "\"flash_time_long\": \"5\", " \
                           "\"brightness\": \"true\", " \
                           "\"effect\": \"true\", " \
                           "\"effect_list\": [\"Confetti\", \"Glitter\", \"Juggle\", \"Sinelon\", \"Solid\", " \
                                             "\"Christmas\", \"Candy Cane\", \"Holly Jolly\", \"Valentine\", \"Lovey Day\", " \
                                             "\"St Patty\", \"Easter\", \"USA\", \"Independence\", \"Go Blue\", " \
                                             "\"Hail\", \"Touchdown\", \"Halloween\", \"Punkin\", \"Thanksgiving\", " \
                                             "\"Turkey Day\", \"BPM\", \"Cyclon Rainbow\", \"Dots\", \"Fire\", " \
                                             "\"Lightning\", \"Police All\", \"Police One\", \"Rainbow\", \"Glitter Rainbow\", " \
                                             "\"Ripple\", \"Twinkle\""
  #ifdef ENABLE_E131
    #define DISCOVERY_E131 ",\"E131\""
  #else
    #define DISCOVERY_E131 ""
  #endif
  #define DISCOVERY_PAYLOAD DISCOVERY_BASE DISCOVERY_E131 "] }"

#endif
#ifdef ENABLE_E131
  #if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3003002)
    #warning "Requires FastLED 3.3.2 or later; check github for latest code."
  #endif
#endif

char setRed = 0;
char setGreen = 0;
char setBlue = 150;
String setPower = "OFF";
eEffects setEffect = eEffects::Solid;
eEffects prevEffect = setEffect;
String setBrightness = "150";
int brightness = 150;
String setAnimationSpeed;
int animationSpeed = 240;
String setColorTemp;
unsigned long flashTime = 0;
CRGB leds[NUM_LEDS];
uint8_t Rcolor;
uint8_t Gcolor;
uint8_t Bcolor;
bool startupMQTTconnect = true;

#ifdef BRIGHTNESS_ENCODER_DT
  volatile boolean brightnessEncoderChanged = false;
  RotaryEncoder brightnessEncoder(BRIGHTNESS_ENCODER_DT, BRIGHTNESS_ENCODER_CLK);
    void ICACHE_RAM_ATTR brightnessEncoderCallback(){
      brightnessEncoder.tick();
      brightnessEncoderChanged = true;
    }
#endif

#ifdef SPEED_ENCODER_DT
  volatile boolean animationSpeedEncoderChanged = false;
  RotaryEncoder animationSpeedEncoder(SPEED_ENCODER_DT, SPEED_ENCODER_CLK);
  void ICACHE_RAM_ATTR animationSpeedEncoderCallback() {
      animationSpeedEncoder.tick();
      animationSpeedEncoderChanged = true;
  }
#endif

/*****************For TWINKLE********/
int twinklecounter = 0;

/**************FOR DOTS**************/
uint8_t fadeval = 224;                                        // Trail behind the LED's. Lower => faster fade.
uint8_t bpm = 30;


/********BPM**********/
uint8_t gHue = 0;


WiFiClient espClient;            // this needs to be unique for each controller
PubSubClient client(espClient);  // this needs to be unique for each controller

long lastReconnectAttempt = 0;

#ifdef ENABLE_E131
  uint16_t universesRequired;
  ESPAsyncE131* e131;
#endif


// Define effects
EffectConfetti       effectConfetti(leds, Rcolor, Gcolor, Bcolor);
EffectGlitter        effectGlitter(leds, Rcolor, Gcolor, Bcolor);
EffectJuggle         effectJuggle(leds, Rcolor, Gcolor, Bcolor);
EffectSinelon        effectSinelon(leds, Rcolor, Gcolor, Bcolor);
EffectSolid          effectSolid(leds, Rcolor, Gcolor, Bcolor);
EffectChristmas      effectChristmas(leds, gHue);
EffectCandyCane      effectCandyCane(leds);
EffectHollyJolly     effectHollyJolly(leds);
EffectValentine      effectValentine(leds, gHue);
EffectLoveyDay       effectLoveyDay(leds);
EffectStPatty        effectStPatty(leds, gHue);
EffectEaster         effectEaster(leds);
EffectUsa            effectUsa(leds, gHue);
EffectIndependence   effectIndependence(leds);
EffectGoBlue         effectGoBlue(leds, gHue);
EffectHail           effectHail(leds);
EffectTouchdown      effectTouchdown(leds);
EffectHalloween      effectHalloween(leds, gHue);
EffectTurkeyDay      effectTurkeyDay(leds, gHue);
EffectPunkin         effectPunkin(leds);
EffectThanksgiving   effectThanksgiving(leds);
EffectBpm            effectBpm(leds, gHue);
EffectCyclonRainbow  effectCyclonRainbow(leds);
EffectDots           effectDots(leds, bpm, fadeval);
EffectLightning      effectLightning(leds);
EffectPoliceAll      effectPoliceAll(leds);
EffectPoliceOne      effectPoliceOne(leds);
EffectRainbow        effectRainbow(leds);
EffectGlitterRainbow effectGlitterRainbow(leds);
EffectRipple         effectRipple(leds);
EffectTwinkle        effectTwinkle(leds);
EffectFire           effectFire(leds);

Effect* effectArray[] = {
                          &effectConfetti,
                          &effectGlitter,
                          &effectJuggle,
                          &effectSinelon,
                          &effectSolid,
                          &effectChristmas,
                          &effectCandyCane,
                          &effectHollyJolly,
                          &effectValentine,
                          &effectLoveyDay,
                          &effectStPatty,
                          &effectEaster,
                          &effectUsa,
                          &effectIndependence,
                          &effectGoBlue,
                          &effectHail,
                          &effectTouchdown,
                          &effectHalloween,
                          &effectPunkin,
                          &effectThanksgiving,
                          &effectTurkeyDay,
                          &effectBpm,
                          &effectCyclonRainbow,
                          &effectDots,
                          &effectFire,
                          &effectLightning,
                          &effectPoliceAll,
                          &effectPoliceOne,
                          &effectRainbow,
                          &effectGlitterRainbow,
                          &effectRipple,
                          &effectTwinkle
};


void callback(char* topic, byte* payload, unsigned int length) {
  char message[length + 1];
  Serial.print("MQTT message received ");
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  if (String(topic) == mqttstate){
    Serial.println("State message - Unsubscribing from state topic");
    client.unsubscribe(mqttstate);
  }
  message[length] = '\0';
  Serial.println(message);

  StaticJsonBuffer<250> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(message);

  if (!root.success()) {
    Serial.println("parseObject() failed");
  } else {
    #ifdef DEBUG
      Serial.println("JSON message parsed succesfully");
    #endif
    if (root.containsKey("state")) {
      const char* power = root["state"];
      setPower = power;
      #ifdef DEBUG
        Serial.print("Power set: ");
        Serial.println(setPower);
      #endif
    }

    if (root.containsKey("color")) {
      setRed = root["color"]["r"];
      setGreen = root["color"]["g"];
      setBlue = root["color"]["b"];
      #ifdef DEBUG
        Serial.print("Colour Set: Red:");
        Serial.print(setRed, DEC);
        Serial.print("  Green: ");
        Serial.print(setGreen, DEC);
        Serial.print("  Blue: ");
        Serial.println(setBlue, DEC);
      #endif
    }

    if (root.containsKey("brightness")) {
      brightness = root["brightness"];
      #ifdef DEBUG
        Serial.print("Brightness Set: ");
        Serial.println(brightness);
      #endif
      #ifdef BRIGHTNESS_ENCODER_DT
        #ifdef BRIGHTNESS_ENCODER_LESS_STEPS
          brightnessEncoder.setPosition(brightness / 15);
        #else
          brightnessEncoder.setPosition(brightness / 5);
        #endif
      #endif
    }

    if (root.containsKey("effect")) {
      const char* tmpNewEffect = root["effect"];
      String newEffect = String(tmpNewEffect);

      if ( newEffect == "Confetti")
        setEffect = eEffects::Confetti;
      else if ( newEffect == "Glitter" )
        setEffect = eEffects::Glitter;
      else if ( newEffect == "Juggle" )
        setEffect = eEffects::Juggle;
      else if ( newEffect == "Sinelon" )
        setEffect = eEffects::Sinelon;
      else if ( newEffect == "Solid" )
        setEffect = eEffects::Solid;
      else if ( newEffect == "Christmas" )
        setEffect = eEffects::Christmas;
      else if ( newEffect == "Candy Cane" )
        setEffect = eEffects::CandyCane;
      else if ( newEffect == "Holly Jolly" )
        setEffect = eEffects::HollyJolly;
      else if ( newEffect == "Valentine" )
        setEffect = eEffects::Valentine;
      else if ( newEffect == "Lovey Day" )
        setEffect = eEffects::LoveyDay;
      else if ( newEffect == "St Patty" )
        setEffect = eEffects::StPatty;
      else if ( newEffect == "Easter" )
        setEffect = eEffects::Easter;
      else if ( newEffect == "Usa" )
        setEffect = eEffects::Usa;
      else if ( newEffect == "Independence" )
        setEffect = eEffects::Independence;
      else if ( newEffect == "Go Blue" )
        setEffect = eEffects::GoBlue;
      else if ( newEffect == "Hail" )
        setEffect = eEffects::Hail;
      else if ( newEffect == "Touchdown" )
        setEffect = eEffects::Touchdown;
      else if ( newEffect == "Halloween" )
        setEffect = eEffects::Halloween;
      else if ( newEffect == "Punkin" )
        setEffect = eEffects::Punkin;
      else if ( newEffect == "Thanksgiving" )
        setEffect = eEffects::Thanksgiving;
      else if ( newEffect == "Turkey Day" )
        setEffect = eEffects::TurkeyDay;
      else if ( newEffect == "Bpm" )
        setEffect = eEffects::Bpm;
      else if ( newEffect == "Cyclon Rainbow" )
        setEffect = eEffects::CyclonRainbow;
      else if ( newEffect == "Dots" )
        setEffect = eEffects::Dots;
      else if ( newEffect == "Fire" )
        setEffect = eEffects::Fire;
      else if ( newEffect == "Lightning" )
        setEffect = eEffects::Lightning;
      else if ( newEffect == "Police All" )
        setEffect = eEffects::PoliceAll;
      else if ( newEffect == "Police One" )
        setEffect = eEffects::PoliceOne;
      else if ( newEffect == "Rainbow" )
        setEffect = eEffects::Rainbow;
      else if ( newEffect == "Glitter Rainbow" )
        setEffect = eEffects::GlitterRainbow;
      else if ( newEffect == "Ripple" )
        setEffect = eEffects::Ripple;
      else if ( newEffect == "Twinkle" )
        setEffect = eEffects::Twinkle;
      #ifdef ENABLE_E131
      else if ( newEffect == "E131" )
        setEffect = eEffects::E131;
      #endif
      #ifdef DEBUG
      else
      {
        Serial.print("Invalid effect: ");
        Serial.println(newEffect);
      }
      #endif
      
      #ifdef DEBUG
        Serial.print("Effect Set: ");
        Serial.println(effectList[setEffect]);
      #endif
      #ifdef ENABLE_E131
        if (setEffect == eEffects::E131) {
          FastLED.clear(true);
          #ifdef DEBUG
            Serial.println("LEDs Cleared.  Ready for E1.31");
          #endif
        }
      #endif
      if ((setEffect == eEffects::Twinkle) || (setEffect == eEffects::Lightning)) {
        twinklecounter = 0;
        #ifdef DEBUG
          Serial.println("Twinkle Counter reset");
        #endif
      }
    }

    if (root.containsKey(SPEEDTOPIC)) {
      animationSpeed = root[SPEEDTOPIC];
      #ifdef DEBUG
        Serial.print("Speed Set: ");
        Serial.println(animationSpeed);
      #endif
      #ifdef SPEED_ENCODER_DT
        #ifdef SPEED_ENCODER_LESS_STEPS
          animationSpeedEncoder.setPosition(animationSpeed / 15);
        #else
          animationSpeedEncoder.setPosition(animationSpeed / 5);
        #endif
      #endif
    }

    if (root.containsKey("flash")) {
      flashTime = (int)root["flash"] * 1000;
      #ifdef DEBUG
        Serial.print("Flash Set: ");
        Serial.println(flashTime);
      #endif
    }else{
      flashTime = 0;
      #ifdef DEBUG
        Serial.println("Flash Set: Off");
      #endif
    }
  }
  #ifdef DEBUG 
    Serial.println("Prepare to publish state..."); 
  #endif

  publishState();
  #ifdef DEBUG 
    Serial.println("MQTT command complete"); 
  #endif
}

void setup_wifi() {
  #ifdef ESP32
  WiFi.setSleep(false);
  #else
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  #endif

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);

  #ifdef ESP32
    WiFi.setHostname(DEVICE_NAME);
  #else
    WiFi.hostname(DEVICE_NAME);
  #endif

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while ((WiFi.status() != WL_CONNECTED     ) && 
         (WiFi.status() != WL_NO_SSID_AVAIL ) && 
         (WiFi.status() != WL_CONNECT_FAILED)    ) 
  {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    fill_solid(leds, NUM_LEDS, CRGB(255, 191, 0));
    FastLED.show();
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    delay(750);
    #ifdef DEBUG 
      Serial.println("WiFi Setup complete"); 
      Serial.print("Hostname: ");
      #ifdef ESP32
        Serial.println(WiFi.getHostname());
      #else
        Serial.println(WiFi.hostname());
      #endif
    #endif
  } else if (WiFi.status() == WL_NO_SSID_AVAIL) {
    Serial.println("");
    Serial.println("WiFi connection failed.  SSID not available");
  } else if (WiFi.status() == WL_CONNECT_FAILED) {
    Serial.println("");
    Serial.println("WiFi connection failed.  Unable to connect");
  }
}


void publishState() {
  StaticJsonBuffer<250> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  #ifdef DEBUG
    Serial.println("Initialising JSON State Message... ");
  #endif

  root["state"] = setPower;
  JsonObject& color = root.createNestedObject("color");
  color["r"] = setRed;
  color["g"] = setGreen;
  color["b"] = setBlue;
  root["brightness"] = brightness;
  root["effect"] = effectList[setEffect];
  root[SPEEDTOPIC] = animationSpeed;
  if (flashTime > 0){
    root["flash"] = flashTime / 1000;
  }
  uint8_t buffer[root.measureLength() + 1];
  root.printTo((char*)buffer, sizeof(buffer));
  #ifdef DEBUG
    Serial.println("Done");
  #endif
  client.beginPublish(mqttstate,sizeof(buffer)-1,true);
  client.write(buffer,sizeof(buffer)-1);
  client.endPublish();
  #ifdef DEBUG
    Serial.println("State Sent");
  #endif
}


void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(250);  //for CYCLON
  }
}


boolean reconnect() {
  Serial.println("Reconnect start");
  if (client.connect(DEVICE_NAME, MQTT_USERNAME, MQTT_PASSWORD, LWTTOPIC, 0, true, "Offline"))
  { 
    Serial.println("connected");
    client.publish(LWTTOPIC, "Online", true);
    #ifdef USE_DISCOVERY
      byte b[] = DISCOVERY_PAYLOAD;
      client.beginPublish(DISCOVERY_TOPIC,sizeof(b)-1,true);
      client.write(b,sizeof(b)-1);
      client.endPublish();
      #ifdef DEBUG
        Serial.println("Discovery message sent");
      #endif
    #endif
    client.subscribe(mqttcommand);
    client.subscribe(MQTT_GROUP_TOPIC);
    #ifdef DEBUG
      Serial.println("Subscribed to MQTT topics");
    #endif
    if (startupMQTTconnect) {
      client.subscribe(mqttstate);
      startupMQTTconnect = false;
      #ifdef DEBUG
        Serial.println("Subscribed to MQTT State topic");
      #endif
      fill_solid(leds, NUM_LEDS, CRGB(0, 255, 0));
      FastLED.show();
      delay(500);
      FastLED.clear (true); //Turns off startup LEDs after connection is made
      FastLED.show();
      delay(250);
    } else {
      publishState();
      #ifdef DEBUG
        Serial.println("State published");
      #endif
    }
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 5 seconds");
    if (startupMQTTconnect) {
      startupMQTTconnect = false;
      FastLED.clear(true);
      FastLED.show();
      delay(250);
    }
  }
  Serial.println("Reconnect end");
  return client.connected();
}

#ifdef POWER_BUTTON_PIN
  void handlePowerButton() {
    static unsigned int powerDebounce = 0;
    static int lastPowerButtonState = HIGH;
    if (millis() - powerDebounce >= 50) { //avoid delay for debounce
      powerDebounce = millis();
      int powerButtonState = digitalRead(POWER_BUTTON_PIN);
      if (powerButtonState != lastPowerButtonState) { // button state has changed
        if (powerButtonState == LOW) { // button is pressed
          if (setPower == "ON") { //if on, turn off
            setPower = "OFF";
          } else { //if off turn on
            setPower = "ON";
          }
          publishState();
          Serial.println("Button press - Set Power: " + setPower);
        }
      }
      lastPowerButtonState = powerButtonState;
    }
  }
#endif

#ifdef COLOR_BUTTON_PIN
  void handleColorButton() {
    static int nextColor = 0;
    static unsigned int colorDebounce = 0;
    static int lastColorButtonState = HIGH;
    if (millis() - colorDebounce >= 50) { //avoid delay for debounce
      colorDebounce = millis();
      int colorButtonState = digitalRead(COLOR_BUTTON_PIN);
      if (colorButtonState != lastColorButtonState) { // button state has changed
        if (colorButtonState == LOW) { // button is pressed
          setRed = colorList[nextColor].red;
          setGreen = colorList[nextColor].green;
          setBlue = colorList[nextColor].blue;
          if (nextColor + 1 >= (sizeof(colorList) / 3)){
            nextColor = 0;
          } else {
            nextColor++;
          }
          publishState();
        }
      }
      lastColorButtonState = colorButtonState;
    }
  }
#endif

#ifdef EFFECT_BUTTON_PIN
  void handleEffectButton() {
    static unsigned int effectDebounce = 0;
    static int lastEffectButtonState = HIGH;
    if (millis() - effectDebounce >= 50) { //avoid delay for debounce
      effectDebounce = millis();
      int effectButtonState = digitalRead(EFFECT_BUTTON_PIN);
      if (effectButtonState != lastEffectButtonState) { // button state has changed
        if (effectButtonState == LOW) { // button is pressed
          setEffect++;

          #ifdef ENABLE_E131
          // Do not select E1.31 with button
          if ( setEffect == eEffects::E131 ) {
            setEffect++;
          }
          #endif

          if (setEffect == eEffects::Twinkle) {
            twinklecounter = 0;
          }

          if (setEffect == eEffects::Lightning) {
            twinklecounter = 0;
          }

          Serial.println("Button press - Set Effect: " + String(effectList[setEffect]));
          publishState();
        }
      }
      lastEffectButtonState = effectButtonState;
    }
  }
#endif

#ifdef BRIGHTNESS_ENCODER_DT
  void brightnessFromEncoder(){
    #ifdef BRIGHTNESS_ENCODER_LESS_STEPS
      int encoderSpeed = 15;
    #else
      int encoderSpeed = 5;
    #endif
    int newPos = brightnessEncoder.getPosition() * encoderSpeed;
    if (newPos != brightness) {
      if (newPos < 0) {
          brightnessEncoder.setPosition(0);
          newPos = 0;
      } else if (newPos > 255) {
          brightnessEncoder.setPosition(255 / encoderSpeed);
          newPos = 255;
      }
      brightness = newPos;
      publishState();
    } 
  }
#endif

#ifdef SPEED_ENCODER_DT
  void animationSpeedFromEncoder(){
    #ifdef SPEED_ENCODER_LESS_STEPS
      int encoderSpeed = 15;
    #else
      int encoderSpeed = 5;
    #endif
    int newPos = animationSpeedEncoder.getPosition() * encoderSpeed;
    if (newPos != animationSpeed) {
      if (newPos < 0) {
          animationSpeedEncoder.setPosition(0);
          newPos = 0;
      } else if (newPos > 255) {
          animationSpeedEncoder.setPosition(255 / encoderSpeed);
          newPos = 255;
      }
      animationSpeed = newPos;
      publishState();
    } 
  }
#endif


// Setup
void setup()
{
  // Init serial
  Serial.begin(115200);
  // Init digital IO
  #ifdef USE_LED_BUILTIN
    pinMode(LED_BUILTIN, OUTPUT);            // Setup builtin LED
    #warning "Builtin LED used"
  #endif
  #ifdef POWER_BUTTON_PIN
    pinMode(POWER_BUTTON_PIN, INPUT_PULLUP);   // Setup power button
  #endif
  #ifdef COLOR_BUTTON_PIN
    pinMode(COLOR_BUTTON_PIN, INPUT_PULLUP);   // Setup color button
    #endif
  #ifdef EFFECT_BUTTON_PIN
    pinMode(EFFECT_BUTTON_PIN, INPUT_PULLUP);  // Setup effect button
  #endif
  #ifdef BRIGHTNESS_ENCODER_DT
    attachInterrupt(digitalPinToInterrupt(BRIGHTNESS_ENCODER_DT), brightnessEncoderCallback, CHANGE);
    attachInterrupt(digitalPinToInterrupt(BRIGHTNESS_ENCODER_CLK), brightnessEncoderCallback, CHANGE);
    #ifdef BRIGHTNESS_ENCODER_DT
      #ifdef BRIGHTNESS_ENCODER_LESS_STEPS
        brightnessEncoder.setPosition(brightness / 15);
      #else
        brightnessEncoder.setPosition(brightness / 5);
      #endif
    #endif
  #endif
  #ifdef SPEED_ENCODER_DT
    attachInterrupt(digitalPinToInterrupt(SPEED_ENCODER_DT), animationSpeedEncoderCallback, CHANGE);
    attachInterrupt(digitalPinToInterrupt(SPEED_ENCODER_CLK), animationSpeedEncoderCallback, CHANGE);
    #ifdef SPEED_ENCODER_DT
      #ifdef SPEED_ENCODER_LESS_STEPS
        animationSpeedEncoder.setPosition(animationSpeed / 15);
      #else
        animationSpeedEncoder.setPosition(animationSpeed / 5);
      #endif
    #endif
  #endif
  #ifdef DEBUG
    Serial.println("GPIO Setup complete");
  #endif

  // Init FastLED
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(12, 10000); //experimental for power management. Feel free to try in your own setup.
  FastLED.setBrightness(brightness);
  #ifdef DEBUG
    Serial.println("FastLED initialised");
  #endif

  // Startup LED Lights
  fill_solid(leds, NUM_LEDS, CRGB::Red); 
  FastLED.show();
  #ifdef DEBUG
    Serial.println("Initial setup complete - LEDs on RED");
  #endif

  setup_wifi();

  client.setServer(MQTT_BROKER, MQTT_PORT); 
  client.setCallback(callback);
  #ifdef DEBUG
    Serial.println("MQTT Initialised");
  #endif


  // Init OTA
  ArduinoOTA.setPort(OTAport);
  ArduinoOTA.setHostname(DEVICE_NAME);
  ArduinoOTA.setPassword((const char *)OTA_PASSWORD);
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  #ifdef DEBUG
    Serial.println("OTA setup complete");
  #endif

  #ifdef ENABLE_E131
    if ( (NUM_LEDS % 170) > 0 ){
      universesRequired = (NUM_LEDS / 170);
    } else {
      universesRequired = (NUM_LEDS / 170) + 1;
    }
    e131 = new ESPAsyncE131(universesRequired);
    e131->begin(E131_UNICAST);
    #ifdef DEBUG 
      Serial.printf("E131 buffer initialised with %u Buffers\n", universesRequired); 
    #endif
  #endif

  Serial.println("Effect's list:");
  for (int i=0; i<32; i++)
  {
    Serial.print(i);
    Serial.print(" - ");
    Serial.println(effectArray[i]->GetEffectName());
  }


}


void loop()
{
  if ( WiFi.status() == WL_CONNECTED )
  {
    // Handle MQTT connection
    if (!client.connected())
    {
      long now = millis();
      if (now - lastReconnectAttempt > 5000)
      {
        Serial.print("Attempting MQTT connection...");
        lastReconnectAttempt = now;
        if (reconnect())
        {
          lastReconnectAttempt = 0;
          Serial.println("MQTT Connection attempt complete");
        }
      }
    }
    else
    {
      // MQTT client connected
      client.loop();
    }
  }

  // Handle OTA
  ArduinoOTA.handle();

  // Handle buttons
  #ifdef POWER_BUTTON_PIN
    handlePowerButton();
  #endif
  #ifdef COLOR_BUTTON_PIN
    handleColorButton();
  #endif
  #ifdef EFFECT_BUTTON_PIN
    handleEffectButton();
  #endif
  #ifdef BRIGHTNESS_ENCODER_DT
    if (brightnessEncoderChanged) {
      brightnessFromEncoder();
    }
  #endif
  #ifdef SPEED_ENCODER_DT
    if (animationSpeedEncoderChanged) {
      animationSpeedFromEncoder();
    }
  #endif

#ifdef ENABLE_E131
  if (setEffect == eEffects::E131 && setPower == "ON") {
    #ifdef USE_LED_BUILTIN
      digitalWrite(LED_BUILTIN, LED_ON);
    #endif
    if (!e131->isEmpty()) {
      e131_packet_t packet;
      e131->pull(&packet);     // Pull packet from ring buffer
      
      uint16_t universe = htons(packet.universe);
      uint16_t universeLast = universe + universesRequired - 1;
      uint16_t maxChannels = htons(packet.property_value_count) - 1;

      if ( universe >= UNIVERSE_START ) 
      {
        // Calculate LED range to update
        uint16_t firstLed = ((universe - UNIVERSE_START) * 170);
        uint16_t lastLed  = firstLed + (maxChannels / 3);  // -1

        #ifdef DEBUG
          Serial.printf("Universe %u / %u Channels | Packet#: %u / Errors: %u / FirstLed: %3u/ LastLed: %3u / CH1: %3u / CH2: %3u / CH3: %3u\n",
                    universe,                               // The Universe for this packet
                    maxChannels,                            // Start code is ignored, we're interested in dimmer data
                    e131->stats.num_packets,                // Packet counter
                    e131->stats.packet_errors,              // Packet error counter
                    firstLed,                               // First LED to update
                    lastLed-1,                              // Last LED to update
                    packet.property_values[1],              // Dimmer data for Channel 1
                    packet.property_values[2],              // Dimmer data for Channel 2
                    packet.property_values[3]);             // Dimmer data for Channel 3
        #endif

        int j = 1;
        for (int i = firstLed; i < min(lastLed,(uint16_t)NUM_LEDS); i++)
        {
          // Calculate channel
          leds[i].setRGB(packet.property_values[j], packet.property_values[j + 1], packet.property_values[j + 2]);
          j += 3;
        }

        FastLED.setBrightness(255);
        FastLED.show();
      }
    }
  } else
#endif
  {
    // Set colorpallet RGB values received to the effect that requires that
    Rcolor = setRed;
    Gcolor = setGreen;
    Bcolor = setBlue;

    static bool flashOff = false;

    if (setPower == "OFF") {
      #ifdef USE_LED_BUILTIN
        digitalWrite(LED_BUILTIN, LED_OFF);
      #endif
      for ( int i = 0; i < NUM_LEDS; i++) {
        leds[i].fadeToBlackBy( 8 );   //FADE OFF LEDS
      }
    } else {
      #ifdef USE_LED_BUILTIN
        digitalWrite(LED_BUILTIN, LED_ON);
      #endif
      static unsigned int flashDelay = 0;
      if (flashTime > 0) {
        if(millis()  - flashDelay >= flashTime) {
          flashDelay = millis();
          flashOff = !flashOff;
        }
      } else {
        flashOff = false;
      }

      switch (setEffect) {
        case eEffects::Confetti:                   // random colored speckles that blink in and fade smoothly
          effectConfetti.loop();
          break;

        case eEffects::Glitter:                    // glitter effect
          effectGlitter.loop();
          break;

        case eEffects::Juggle:                     // eight colored dots, weaving in and out of sync with each other
          effectJuggle.loop();
          break;

        case eEffects::Sinelon:
          effectSinelon.loop();
          break;

        case eEffects::Solid:                      // Fill entire strand with solid color
          effectSolid.loop();
          break;

        case eEffects::Christmas:                  // colored stripes pulsing in Shades of GREEN and RED
          effectChristmas.loop();
          break;

        case eEffects::CandyCane:
          effectCandyCane.loop();
          break;

        case eEffects::HollyJolly:
          effectHollyJolly.loop();
          break;

        case eEffects::Valentine:                  // colored stripes pulsing in Shades of PINK and RED
          effectValentine.loop();
          break;

        case eEffects::LoveyDay:                   // Valentine's Day colors (TWO COLOR SOLID)
          effectLoveyDay.loop();
          break;

        case eEffects::StPatty:                    // colored stripes pulsing in Shades of GREEN
          effectStPatty.loop();
          break;

        case eEffects::Easter:
          effectEaster.loop();
          break;

        case eEffects::Usa:                        // colored stripes pulsing in Shades of Red White & Blue
          effectUsa.loop();
          break;

        case eEffects::Independence:               // colored stripes of Red White & Blue
          effectIndependence.loop();
          break;

        case eEffects::GoBlue:
          effectGoBlue.loop();
          break;

        case eEffects::Hail:
          effectHail.loop();
          break;

        case eEffects::Touchdown:                  // Maize and Blue with POLICE ALL animation
          effectTouchdown.loop();
          break;

        case eEffects::Halloween:                  // colored stripes pulsing in Shades of Purple and Orange
          effectHalloween.loop();
          break;

        case eEffects::Punkin:
          effectPunkin.loop();
          break;

        case eEffects::Thanksgiving:               // colored stripes pulsing in Shades of Red and ORANGE and Green
          effectThanksgiving.loop();
          break;

        case eEffects::TurkeyDay:                  // colored stripes pulsing in Shades of Brown and ORANGE
          effectTurkeyDay.loop();
          break;

        case eEffects::Bpm:                        // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
          effectBpm.loop();
          break;

        case eEffects::CyclonRainbow:
          effectCyclonRainbow.loop();
          break;

        case eEffects::Dots:
          effectDots.loop();
          break;

        case eEffects::Fire:
          // Resets strip if previous animation was running
          if ( prevEffect != eEffects::Fire )
          {
            effectFire.resetStripe();
          }
          effectFire.loop();
          break;

        case eEffects::Lightning:
          // Resets strip if previous animation was running
          if ( prevEffect != eEffects::Lightning )
          {
            effectLightning.resetStripe();
          }
          effectLightning.loop();
          break;

        case eEffects::PoliceAll:                  // POLICE LIGHTS (TWO COLOR SOLID)
          effectPoliceAll.loop();
          break;

        case eEffects::PoliceOne:                  // POLICE LIGHTS (TWO COLOR SINGLE LED)
          effectPoliceOne.loop();
          break;

        case eEffects::Rainbow:                    // FastLED's built-in rainbow generator
          effectRainbow.loop();
          break;

        case eEffects::GlitterRainbow:             // FastLED's built-in rainbow generator with Glitter
          effectGlitterRainbow.loop();
          break;

        case eEffects::Ripple:
          effectRipple.loop();
          break;

        case eEffects::Twinkle:
          // Resets strip if previous animation was running
          if ( prevEffect != eEffects::Twinkle )
          {
            effectTwinkle.resetStripe();
          }
          effectTwinkle.loop();
          break;


        default:
          break;
      }
      prevEffect = setEffect;

      random16_add_entropy( random8());

      EVERY_N_MILLISECONDS(10) {
      
        gHue++;

      }

    }

    if (flashOff) {
      FastLED.clear(true);
    } else {
      FastLED.setBrightness(brightness);  //EXECUTE EFFECT COLOR
      FastLED.show();
    }

    // Set FastLED delay
    if (setPower == "OFF") 
    {
      FastLED.delay(10);
    } 
    else 
    {
      if ((animationSpeed > 0) && (animationSpeed < 255))   //Sets animation speed based on receieved value
      {
        FastLED.delay((255 - (animationSpeed - 1)) * 6);
      }
      else if (animationSpeed == 0) 
      {
          FastLED.delay(1600);
      }
    }

    #ifdef DEBUG_FPS
      Serial.print("FPS: ");
      Serial.println(FastLED.getFPS());
    #endif
  }


}