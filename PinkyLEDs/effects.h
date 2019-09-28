#pragma once

#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h> 

#include "config.h"

class Effect
{
public:
  Effect(CRGB leds[]);
  virtual ~Effect();
  virtual void loop();
  void resetStrip() const;

protected:
  int antipodal_index(int) const;
  CRGB * m_Leds;

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

  uint8_t& Rcolor;
  uint8_t& Gcolor;
  uint8_t& Bcolor;
  
};


class EffectJuggle : public Effect 
{
public:
  EffectJuggle(CRGB leds[], uint8_t&, uint8_t&, uint8_t&);
  virtual ~EffectJuggle();
  void loop();

private:
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

class EffectUsa : public Effect
{
public:
  EffectUsa(CRGB leds[], uint8_t&);
  virtual ~EffectUsa();
  void loop();

private:
  uint8_t& gHue;

};

class EffectValentine : public Effect
{
public:
  EffectValentine(CRGB leds[], uint8_t&);
  virtual ~EffectValentine();
  void loop();

private:
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
  uint8_t& gHue;

};


class EffectTurkeyDay : public Effect
{
public:
  EffectTurkeyDay(CRGB leds[], uint8_t&);
  virtual ~EffectTurkeyDay();
  void loop();

private:
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
  uint8_t& gHue;

};


class EffectCyclonRainbow : public Effect
{
public:
  EffectCyclonRainbow(CRGB leds[]);
  virtual ~EffectCyclonRainbow();
  void loop();


private:
  uint8_t cyclonHue;
  int     cyclonPos;
  bool    forwards;
};


class EffectDots : public Effect
{
public:
  EffectDots(CRGB leds[], uint8_t&, uint8_t&);
  virtual ~EffectDots();
  void loop();

private:
  uint8_t& bpm;
  uint8_t& fadeval;

};


class EffectLightning : public Effect
{
public:
  EffectLightning(CRGB leds[]);
  virtual ~EffectLightning();
  void loop();


private:
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
  void loop();

private:
  bool gReverseDirection;
  uint8_t heat[NUM_LEDS];  // Array of temperature readings at each simulation cell
  CRGBPalette16 gPal;

};