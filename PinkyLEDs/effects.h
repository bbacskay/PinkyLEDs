#pragma once

#ifdef ESP8266
  /*
    Spending too much time in the ISR or disabling the interrupts for too long
    time could cause WDT reset.

    For info see:
    https://github.com/espressif/ESP8266_NONOS_SDK/issues/90
    and
    https://github.com/FastLED/FastLED/wiki/Interrupt-problems
  */
  //#define FASTLED_ALLOW_INTERRUPTS 0
  #define FASTLED_INTERRUPT_RETRY_COUNT 1

  #ifdef ARDUINO_ESP8266_NODEMCU
    #define FASTLED_ESP8266_RAW_PIN_ORDER
  #endif

#endif

#include <FastLED.h> 

#include "config.h"

#ifdef ENABLE_E131
  #include <ESPAsyncE131.h>

  #if (UNIVERSE_START < 1) || (UNIVERSE_START > 63999)
    #error "UNIVERSE_START has to be 1 - 63999, check config.h"
  #endif
#endif


class Effect
{
public:
  Effect(CRGB leds[], String);
  virtual ~Effect();
  String GetEffectName() const;
  virtual void loop();
  virtual void resetStripe();

protected:
  int antipodal_index(int) const;
  CRGB * m_Leds;
  const String m_Name;

  private:

};


class EffectConfetti : public Effect
{
public:
  EffectConfetti(CRGB leds[], uint8_t&, uint8_t&, uint8_t&);
  virtual ~EffectConfetti();
  void loop();

private:

  uint8_t& Rcolor;
  uint8_t& Gcolor;
  uint8_t& Bcolor;

};


class EffectGlitter : public Effect
{
public:
  EffectGlitter(CRGB leds[], uint8_t&, uint8_t&, uint8_t&);
  virtual ~EffectGlitter();
  void loop();


private:
  void addGlitterColor( fract8 chanceOfGlitter, const int Rcolor, const int Gcolor, const int Bcolor) const;

  //static const String m_Name;

  uint8_t& Rcolor;
  uint8_t& Gcolor;
  uint8_t& Bcolor;
  
};


class EffectJuggle : public Effect 
{
public:
  EffectJuggle(CRGB leds[], uint8_t&, uint8_t&, uint8_t&);
  virtual ~EffectJuggle();
  //String GetEffectName() const override;
  void loop();

private:
  //static const String m_Name;

  uint8_t& Rcolor;
  uint8_t& Gcolor;
  uint8_t& Bcolor;

};


class EffectSinelon : public Effect
{
public:
  EffectSinelon(CRGB leds[], uint8_t&, uint8_t&, uint8_t&);
  virtual ~EffectSinelon();
  void loop();

private:
  //static const String m_Name;

  uint8_t& Rcolor;
  uint8_t& Gcolor;
  uint8_t& Bcolor;

};


class EffectSolid : public Effect
{
public:
  EffectSolid(CRGB leds[], uint8_t&, uint8_t&, uint8_t&);
  virtual ~EffectSolid();
  void loop();

private:
  //static const String m_Name;

  uint8_t& Rcolor;
  uint8_t& Gcolor;
  uint8_t& Bcolor;

};


class EffectChristmas : public Effect
{
public:
  EffectChristmas(CRGB leds[], uint8_t&);
  virtual ~EffectChristmas();
  void loop();

private:
  static const String m_Name;

  uint8_t& gHue;

};


class EffectCandyCane : public Effect
{
public:
  EffectCandyCane(CRGB leds[]);
  virtual ~EffectCandyCane();
  void loop();

private:
  void setupPalette( CRGB A, CRGB AB, CRGB B, CRGB BA);

  //static const String m_Name;

  CRGBPalette16 currentPalettestriped;
  uint8_t startIndex;

};


class EffectHollyJolly : public Effect
{
public:
  EffectHollyJolly(CRGB leds[]);
  virtual ~EffectHollyJolly();
  void loop();

private:
  void setupPalette( CRGB A, CRGB AB, CRGB B, CRGB BA);

  static const String m_Name;

  CRGBPalette16 HJPalettestriped;
  uint8_t startIndex;

};

class EffectLoveyDay : public Effect
{
public:
  EffectLoveyDay(CRGB leds[]);
  virtual ~EffectLoveyDay();
  void loop();

private:
  static const String m_Name;

  int idex;    // -LED INDEX (0 to NUM_LEDS-1)
  uint8_t thishueLovey;
  int thissat;     // -FX LOOPS DELAY VAR

};

class EffectStPatty : public Effect
{
public:
  EffectStPatty(CRGB leds[], uint8_t&);
  virtual ~EffectStPatty();
  void loop();

private:

  uint8_t& gHue;

};

class EffectEaster : public Effect
{
public:
  EffectEaster(CRGB leds[]);
  virtual ~EffectEaster();
  void loop();

private:
  uint16_t dist;                // A random number for our noise generator.
  uint16_t scale;               // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
  uint8_t maxChanges;           // Value for blending between palettes.
  CRGBPalette16 targetPalette;
  CRGBPalette16 currentPalette;


};

class EffectUsa : public Effect
{
public:
  EffectUsa(CRGB leds[], uint8_t&);
  virtual ~EffectUsa();
  void loop();

private:
  static const String m_Name;

  uint8_t& gHue;

};

class EffectValentine : public Effect
{
public:
  EffectValentine(CRGB leds[], uint8_t&);
  virtual ~EffectValentine();
  void loop();

private:
  static const String m_Name;

  uint8_t& gHue;

};

class EffectIndependence : public Effect
{
public:
  EffectIndependence(CRGB leds[]);
  virtual ~EffectIndependence();
  void loop();

private:
  void setupPalette( CRGB A, CRGB AB, CRGB B, CRGB BA);

  static const String m_Name;

  CRGBPalette16 IndPalettestriped;
  uint8_t startIndex;

};

class EffectGoBlue : public Effect
{
public:
  EffectGoBlue(CRGB leds[], uint8_t&);
  virtual ~EffectGoBlue();
  void loop();

private:
  static const String m_Name;
  
  uint8_t& gHue;

};


class EffectHail : public Effect
{
public:
  EffectHail(CRGB leds[]);
  virtual ~EffectHail();
  void loop();

private:
  void setupPalette( CRGB A, CRGB AB, CRGB B, CRGB BA);

  static const String m_Name;

  CRGBPalette16 hailPalettestriped;
  uint8_t startIndex;

};


class EffectTouchdown : public Effect
{
public:
  EffectTouchdown(CRGB leds[]);
  virtual ~EffectTouchdown();
  void loop();

private:
  static const String m_Name;

  int idex;    // -LED INDEX (0 to NUM_LEDS-1)
  uint8_t thishuetouch;
  int thissat;     // -FX LOOPS DELAY VAR

};


class EffectHalloween : public Effect
{
public:
  EffectHalloween(CRGB leds[], uint8_t&);
  virtual ~EffectHalloween();
  void loop();

private:
  static const String m_Name;

  uint8_t& gHue;

};


class EffectTurkeyDay : public Effect
{
public:
  EffectTurkeyDay(CRGB leds[], uint8_t&);
  virtual ~EffectTurkeyDay();
  void loop();

private:
  static const String m_Name;

  uint8_t& gHue;

};


class EffectPunkin : public Effect
{
public:
  EffectPunkin(CRGB leds[]);
  virtual ~EffectPunkin();
  void loop();

private:
  void setupPalette( CRGB A, CRGB AB, CRGB B, CRGB BA);

  static const String m_Name;

  CRGBPalette16 PunkinPalettestriped;
  uint8_t startIndex;

};


class EffectThanksgiving : public Effect
{
public:
  EffectThanksgiving(CRGB leds[]);
  virtual ~EffectThanksgiving();
  void loop();

private:
  void setupPalette( CRGB A, CRGB AB, CRGB B, CRGB BA);

  static const String m_Name;

  CRGBPalette16 ThxPalettestriped;
  uint8_t startIndex;

};


class EffectBpm : public Effect
{
public:
  EffectBpm(CRGB leds[], uint8_t&);
  virtual ~EffectBpm();
  void loop();

private:
  static const String m_Name;

  uint8_t& gHue;

};


class EffectCyclonRainbow : public Effect
{
public:
  EffectCyclonRainbow(CRGB leds[]);
  virtual ~EffectCyclonRainbow();
  void loop();


private:
  static const String m_Name;

  uint8_t cyclonHue;
  int     cyclonPos;
  bool    forwards;
};


class EffectDots : public Effect
{
public:
  EffectDots(CRGB leds[]);
  virtual ~EffectDots();
  void loop();

private:
  static const String m_Name;

  //uint8_t& bpm;
  //uint8_t& fadeval;
  const uint8_t bpm = 30;
  const uint8_t fadeval = 224;    // Trail behind the LED's. Lower => faster fade.
};


class EffectLightning : public Effect
{
public:
  EffectLightning(CRGB leds[]);
  virtual ~EffectLightning();
  void loop();


private:
  static const String m_Name;

  uint16_t ledstart;                  // Starting location of a flash
  uint16_t ledlen;
  unsigned int lightningFlashTime;
  unsigned int lightningFlashDelay;
  unsigned int dimmer;
  uint8_t flashes;                   // the upper limit of flashes per strike
  uint8_t frequency;                 // controls the interval between strikes

};

class EffectPoliceAll : public Effect
{
public:
  EffectPoliceAll(CRGB leds[]);
  virtual ~EffectPoliceAll();
  void loop();

private:
  static const String m_Name;

  int     idex;               // -LED INDEX (0 to NUM_LEDS-1)
  uint8_t thishuepolice;
  int     thissat;            // -FX LOOPS DELAY VAR

};

class EffectPoliceOne : public Effect
{
public:
  EffectPoliceOne(CRGB leds[]);
  virtual ~EffectPoliceOne();
  void loop();

private:
  static const String m_Name;

  int     idex;               // -LED INDEX (0 to NUM_LEDS-1)
  uint8_t thishuepolice;
  int     thissat;            // -FX LOOPS DELAY VAR

};


class EffectRainbow : public Effect
{
public:
  EffectRainbow(CRGB leds[]);
  virtual ~EffectRainbow();
  void loop();

private:
  static const String m_Name;

  uint8_t thishue;            // Starting hue value.
  uint8_t deltahue;

};


class EffectGlitterRainbow : public Effect
{
public:
  EffectGlitterRainbow(CRGB leds[]);
  virtual ~EffectGlitterRainbow();
  void loop();

private:
  void addGlitter( fract8 );

  static const String m_Name;

  uint8_t thishue;            // Starting hue value.
  uint8_t deltahue;

};


class EffectTwinkle : public Effect
{
public:
  EffectTwinkle(CRGB leds[]);
  virtual ~EffectTwinkle();
  void loop();

private:
  static const String m_Name;

  const uint8_t density;

};


/********FIRE**********/
#define COOLING  55
#define SPARKING 120

class EffectFire : public Effect
{
public:
  EffectFire(CRGB leds[]);
  virtual ~EffectFire();
  //String GetEffectName() const override;
  void resetStripe() override;
  void loop();

private:

  bool gReverseDirection;
  uint8_t heat[NUM_LEDS];  // Array of temperature readings at each simulation cell
  CRGBPalette16 gPal;

};


#define MAX_STEPS 16                                           // Case statement wouldn't allow a variable.
class EffectRipple : public Effect
{
public:
  EffectRipple(CRGB leds[]);
  virtual ~EffectRipple();
  void loop();

private:

  uint8_t colour;                                               // Ripple colour is randomized.
  int center;                                                   // Center of the current ripple.
  int step;                                                     // -1 is the initializing step.
  uint8_t bgcol;                                                // Background colour rotates.
  uint8_t myfade;                                               // Starting brightness.


};

class EffectE131 : public Effect
{
  public:
    EffectE131(CRGB leds[]);
    virtual ~EffectE131();
    void resetStripe() override;
    void loop();
    void setup();

  private:
    uint16_t universesRequired;
    ESPAsyncE131* e131;
};
