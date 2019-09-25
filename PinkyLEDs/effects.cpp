#include "effects.h"

#define TOP_INDEX (NUM_LEDS / 2)

//#pragma region class_Effect
Effect::Effect(CRGB leds[])
  : m_Leds(leds)
{
  
}

Effect::~Effect()
{

}

// Function to calculate antipodial index
int Effect::antipodal_index(int i) const
{
  int iN = i + TOP_INDEX;
  if (i >= TOP_INDEX) {
    iN = ( i + TOP_INDEX ) % NUM_LEDS;
  }
  return iN;
}

// Loop methode, needs to be called in every loop cycle
void Effect::loop()
{

}

void Effect::resetStrip() const
{
  FastLED.clear();
  FastLED.show();
}



// Constructor of EffectConfetti class
EffectConfetti::EffectConfetti(CRGB leds[], uint8_t& Rcolor, uint8_t& Gcolor, uint8_t& Bcolor)
	: Effect(leds), Rcolor(Rcolor), Gcolor(Gcolor), Bcolor(Bcolor)
{
	Serial.println("EffectConfetti constructor called");
}

// Destructor of EffectConfetti class
EffectConfetti::~EffectConfetti()
{

}

void EffectConfetti::loop()
{
  fadeToBlackBy( m_Leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  m_Leds[pos] += CRGB(Rcolor + random8(64), Gcolor, Bcolor);
}


// Constructor of EffectGlitter class
EffectGlitter::EffectGlitter(CRGB leds[], uint8_t& Rcolor, uint8_t& Gcolor, uint8_t& Bcolor)
	: Effect(leds), Rcolor(Rcolor), Gcolor(Gcolor), Bcolor(Bcolor)
{
	Serial.println("EffectGlitter constructor called");
}

// Destructor of EffectGlitter class
EffectGlitter::~EffectGlitter()
{

}

void EffectGlitter::addGlitterColor( fract8 chanceOfGlitter, const int Rcolor, const int Gcolor, const int Bcolor) const
{
  if ( random8() < chanceOfGlitter) {
    m_Leds[ random16(NUM_LEDS) ] += CRGB(Rcolor, Gcolor, Bcolor);
  }
}

void EffectGlitter::loop()
{
  fadeToBlackBy( m_Leds, NUM_LEDS, 20);
  addGlitterColor(80, Rcolor, Gcolor, Bcolor);
}

// Constructor of EffectJuggle class
EffectJuggle::EffectJuggle(CRGB leds[], uint8_t& Rcolor, uint8_t& Gcolor, uint8_t& Bcolor)
	: Effect(leds), Rcolor(Rcolor), Gcolor(Gcolor), Bcolor(Bcolor)
{
	Serial.println("class_EffectJuggle constructor called");
}

// Destructor of EffectJuggle class
EffectJuggle::~EffectJuggle()
{

}

void EffectJuggle::loop()
{
  fadeToBlackBy( m_Leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for ( int i = 0; i < 8; i++) {
    m_Leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CRGB(Rcolor, Gcolor, Bcolor);
    dothue += 32;
  }
}


// Constructor of EffectSolid class
EffectSolid::EffectSolid(CRGB leds[], uint8_t& Rcolor, uint8_t& Gcolor, uint8_t& Bcolor)
	: Effect(leds), Rcolor(Rcolor), Gcolor(Gcolor), Bcolor(Bcolor)
{
	Serial.println("class_EffectSolid constructor called");
}

// Destructor of EffectSolid class
EffectSolid::~EffectSolid()
{

}

void EffectSolid::loop()
{
  fill_solid(m_Leds, NUM_LEDS, CRGB(Rcolor, Gcolor, Bcolor));
}


// Constructor of EffectSinelon class
EffectSinelon::EffectSinelon(CRGB leds[], uint8_t& Rcolor, uint8_t& Gcolor, uint8_t& Bcolor)
	: Effect(leds), Rcolor(Rcolor), Gcolor(Gcolor), Bcolor(Bcolor)
{
	Serial.println("class_EffectSinelon constructor called");
}

// Destructor of EffectSinelon class
EffectSinelon::~EffectSinelon()
{

}

void EffectSinelon::loop()
{
  fadeToBlackBy( m_Leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS - 1);
  m_Leds[pos] += CRGB(Rcolor, Gcolor, Bcolor);
}


//#pragma region class_EffectChristmas
// Constructor of EffectChristmas class
EffectChristmas::EffectChristmas(CRGB leds[], uint8_t& gHue )
	: Effect(leds), gHue(gHue)
{
	Serial.println("EffectChristmas constructor called");
}

EffectChristmas::~EffectChristmas()
{

}

//palette for Christmas
DEFINE_GRADIENT_PALETTE( bhw2_xmas_gp ) {
  0,   0, 12,  0,
  40,   0, 55,  0,
  66,   1, 117,  2,
  77,   1, 84,  1,
  81,   0, 55,  0,
  119,   0, 12,  0,
  153,  42,  0,  0,
  181, 121,  0,  0,
  204, 255, 12,  8,
  224, 121,  0,  0,
  244,  42,  0,  0,
  255,  42,  0,  0
};

void EffectChristmas::loop()
{
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = bhw2_xmas_gp;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) {
    m_Leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

//#pragma endregion class_EffectChristmas


// Constructor of EffectCandyCane class
EffectCandyCane::EffectCandyCane(CRGB leds[])
	: Effect(leds), startIndex(0)
{
  // Init palette
  setupPalette( CRGB::Red, CRGB::Red, CRGB::White, CRGB::White );

  Serial.println("EffectCandyCane constructor called");
}

EffectCandyCane::~EffectCandyCane()
{
  
}

void EffectCandyCane::setupPalette( CRGB A, CRGB AB, CRGB B, CRGB BA) {
  currentPalettestriped = CRGBPalette16(
                        A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B
                      );
}

void EffectCandyCane::loop()
{
  startIndex += 1;   // higher = faster motion

  fill_palette( m_Leds, NUM_LEDS,
                startIndex, 16,  // higher = narrower stripes
                currentPalettestriped, 255, LINEARBLEND);

}


// Constructor of EffectHollyJolly class
EffectHollyJolly::EffectHollyJolly(CRGB leds[])
	: Effect(leds), startIndex(0)
{
  // Init palette
  setupPalette( CRGB::Red, CRGB::Red, CRGB::Green, CRGB::Green );

  Serial.println("EffectHollyJolly constructor called");
}

EffectHollyJolly::~EffectHollyJolly()
{
  
}

void EffectHollyJolly::setupPalette( CRGB A, CRGB AB, CRGB B, CRGB BA) {
  HJPalettestriped = CRGBPalette16(
                        A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B
                      );
}

void EffectHollyJolly::loop()
{
  startIndex += 1;   // higher = faster motion

  fill_palette( m_Leds, NUM_LEDS,
                startIndex, 16,  // higher = narrower stripes
                HJPalettestriped, 255, LINEARBLEND);

}


// Constructor of EffectLoveyDay class
EffectLoveyDay::EffectLoveyDay(CRGB leds[])
	: Effect(leds), idex(0), thishueLovey(0), thissat(255)
{

  Serial.println("EffectLoveyDay constructor called");
}

EffectLoveyDay::~EffectLoveyDay()
{
  
}


void EffectLoveyDay::loop()
{
  idex++;
  if (idex >= NUM_LEDS) {
    idex = 0;
  }
  int idexR = idex;
  int idexB = antipodal_index(idexR);
  int thathue = (thishueLovey + 244) % 255;
  m_Leds[idexR] = CHSV(thishueLovey, thissat, 255);
  m_Leds[idexB] = CHSV(thathue, thissat, 255);
}


//#pragma region class_EffectStPatty
// Constructor of EffectStPatty class
EffectStPatty::EffectStPatty(CRGB leds[], uint8_t& gHue)
	: Effect(leds), gHue(gHue)
{
	Serial.println("EffectStPatty constructor called");
}

EffectStPatty::~EffectStPatty()
{
  
}

// Palette for St Patty
DEFINE_GRADIENT_PALETTE( bhw2_greenman_gp ) {
  0,   1, 22,  1,
  130,   1, 168,  2,
  255,   1, 22,  1
};


void EffectStPatty::loop()
{
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = bhw2_greenman_gp;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) {
    m_Leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
};

//#pragma endregion class_EffectStPatty


// Constructor of EffectValentine class
EffectValentine::EffectValentine(CRGB leds[], uint8_t& gHue)
	: Effect(leds), gHue(gHue)
{
	Serial.println("EffectValentine constructor called");
}

EffectValentine::~EffectValentine()
{
  
}


//palette for Valentine
DEFINE_GRADIENT_PALETTE( bhw2_redrosey_gp ) {
  0, 103,  1, 10,
  33, 109,  1, 12,
  76, 159,  5, 48,
  119, 175, 55, 103,
  127, 175, 55, 103,
  178, 159,  5, 48,
  221, 109,  1, 12,
  255, 103,  1, 10
};

void EffectValentine::loop()
{
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = bhw2_redrosey_gp;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) {
    m_Leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}


// Constructor of EffectTurkeyDay class
EffectTurkeyDay::EffectTurkeyDay(CRGB leds[], uint8_t& gHue)
	: Effect(leds), gHue(gHue)
{
	Serial.println("EffectTurkeyDay constructor called");
}

EffectTurkeyDay::~EffectTurkeyDay()
{
  
}

//palette for Turkey Day
DEFINE_GRADIENT_PALETTE( bhw2_thanks_gp ) {
  0,   9,  5,  1,
  48,  25,  9,  1,
  76, 137, 27,  1,
  96,  98, 42,  1,
  124, 144, 79,  1,
  153,  98, 42,  1,
  178, 137, 27,  1,
  211,  23,  9,  1,
  255,   9,  5,  1
};

void EffectTurkeyDay::loop()
{
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = bhw2_thanks_gp;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) {
    m_Leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}


// Constructor of EffectThanksgiving class
EffectThanksgiving::EffectThanksgiving(CRGB leds[])
	: Effect(leds), startIndex(0)
{
  // Init palette
  setupPalette( CRGB::OrangeRed, CRGB::Olive, CRGB::Maroon, CRGB::Maroon);

  Serial.println("EffectThanksgiving constructor called");
}

EffectThanksgiving::~EffectThanksgiving()
{
  
}

void EffectThanksgiving::setupPalette( CRGB A, CRGB AB, CRGB B, CRGB BA) {
  ThxPalettestriped = CRGBPalette16(
                        A, A, A, A, A, A, A, AB, AB, AB, B, B, B, B, B, B
                      );
}

void EffectThanksgiving::loop()
{
  startIndex += 1;   // higher = faster motion

  fill_palette( m_Leds, NUM_LEDS,
                startIndex, 16,  // higher = narrower stripes
                ThxPalettestriped, 255, LINEARBLEND);

}


// Constructor of EffectUsa class
EffectUsa::EffectUsa(CRGB leds[], uint8_t& gHue)
	: Effect(leds), gHue(gHue)
{
	Serial.println("EffectUsa constructor called");
}

EffectUsa::~EffectUsa()
{
  
}

//palette for USA
DEFINE_GRADIENT_PALETTE( bhw3_41_gp ) {
  0,   0,  0, 45,
  71,   7, 12, 255,
  76,  75, 91, 255,
  76, 255, 255, 255,
  81, 255, 255, 255,
  178, 255, 255, 255,
  179, 255, 55, 45,
  196, 255,  0,  0,
  255,  42,  0,  0
};

void EffectUsa::loop()
{
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = bhw3_41_gp;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) {
    m_Leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}


// Constructor of EffectIndependence class
EffectIndependence::EffectIndependence(CRGB leds[])
	: Effect(leds), startIndex(0)
{
  // Init palette
  setupPalette( CRGB::FireBrick, CRGB::Cornsilk, CRGB::MediumBlue, CRGB::MediumBlue );

  Serial.println("EffectIndependence constructor called");
}

EffectIndependence::~EffectIndependence()
{
  
}

void EffectIndependence::setupPalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
{
  IndPalettestriped = CRGBPalette16(
                        A, A, A, A, A, AB, AB, AB, AB, AB, B, B, B, B, B, B
                      );
}

void EffectIndependence::loop()
{
  startIndex += 1;   // higher = faster motion 

  fill_palette( m_Leds, NUM_LEDS,
                startIndex, 16,  // higher = narrower stripes
                IndPalettestriped, 255, LINEARBLEND);
}


// Constructor of EffectGoBlue class
EffectGoBlue::EffectGoBlue(CRGB leds[], uint8_t& gHue)
	: Effect(leds), gHue(gHue)
{

  Serial.println("EffectGoBlue constructor called");
}

EffectGoBlue::~EffectGoBlue()
{
  
}

//palette for Go Blue
DEFINE_GRADIENT_PALETTE( Pills_3_gp ) {
  0,   4, 12, 122,
  127,  55, 58, 50,
  255, 192, 147, 11
};

void EffectGoBlue::loop()
{
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = Pills_3_gp;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) { //9948
    m_Leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}


// Constructor of EffectHail class
EffectHail::EffectHail(CRGB leds[])
	: Effect(leds), startIndex(0)
{
  // Init palette
  setupPalette( CRGB::Blue, CRGB::Blue, CRGB::Yellow, CRGB::Yellow );

  Serial.println("EffectHail constructor called");
}

EffectHail::~EffectHail()
{
  
}

void EffectHail::setupPalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
{
  hailPalettestriped = CRGBPalette16(
                        A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B
                      );
}

void EffectHail::loop()
{
  startIndex += 1;                // higher = faster motion

  fill_palette( m_Leds, NUM_LEDS,
                startIndex, 16,   // higher = narrower stripes
                hailPalettestriped, 255, LINEARBLEND);
}


// Constructor of EffectTouchdown class
EffectTouchdown::EffectTouchdown(CRGB leds[])
	: Effect(leds), idex(0), thishuetouch(0), thissat(255)
{

  Serial.println("EffectTouchdown constructor called");
}

EffectTouchdown::~EffectTouchdown()
{
  
}

void EffectTouchdown::loop()
{
  idex++;
  if (idex >= NUM_LEDS) {
    idex = 0;
  }
  int idexY = idex;
  int idexB = antipodal_index(idexY);
  int thathue = (thishuetouch + 96) % 255;
  m_Leds[idexY] = CHSV(thishuetouch, thissat, 255);
  m_Leds[idexB] = CHSV(thathue, thissat, 255);
}


// Constructor of EffectHalloween class
EffectHalloween::EffectHalloween(CRGB leds[], uint8_t& gHue)
	: Effect(leds), gHue(gHue)
{

  Serial.println("EffectHalloween constructor called");
}

EffectHalloween::~EffectHalloween()
{
  
}

//palette for Halloween
DEFINE_GRADIENT_PALETTE( Orange_to_Purple_gp ) {
  0, 208, 50,  1,
  127, 146, 27, 45,
  255,  97, 12, 178
};

void EffectHalloween::loop()
{
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = Orange_to_Purple_gp;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) 
  {
    m_Leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}


// Constructor of EffectPunkin class
EffectPunkin::EffectPunkin(CRGB leds[])
	: Effect(leds), startIndex(0)
{
  // Init palette
  setupPalette( CRGB::DarkOrange, CRGB::DarkOrange, CRGB::Indigo, CRGB::Indigo );

  Serial.println("EffectPunkin constructor called");
}

EffectPunkin::~EffectPunkin()
{
  
}

void EffectPunkin::setupPalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
{
  PunkinPalettestriped = CRGBPalette16(
                        A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B
                      );
}

void EffectPunkin::loop()
{
  startIndex += 1;  // higher = faster motion

  fill_palette( m_Leds, NUM_LEDS,
                startIndex, 16, // higher = narrower stripes
                PunkinPalettestriped, 255, LINEARBLEND);
}


// Constructor of EffectBpm class
EffectBpm::EffectBpm(CRGB leds[], uint8_t& gHue)
	: Effect(leds), gHue(gHue)
{
	Serial.println("EffectBpm constructor called");
}

EffectBpm::~EffectBpm()
{

}

void EffectBpm::loop()
{
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) {
    m_Leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}


// Constructor of EffectCyclonRainbow class
EffectCyclonRainbow::EffectCyclonRainbow(CRGB leds[])
	: Effect(leds), cyclonHue(0), cyclonPos(0), forwards(false)
{
	Serial.println("EffectCyclonRainbow constructor called");
}

EffectCyclonRainbow::~EffectCyclonRainbow()
{

}

void EffectCyclonRainbow::loop()
{
  for(int a = 0; a < NUM_LEDS; a++) {
    m_Leds[a].nscale8(250);
  }

  if (forwards) {
    if (cyclonPos < NUM_LEDS - 1){
      m_Leds[cyclonPos] = CHSV(cyclonHue++, 255,255);
      cyclonPos++;
    } else {
      forwards = false;
    }
  } else {
    if (cyclonPos >= 0) {
      m_Leds[cyclonPos] = CHSV(cyclonHue++, 255,255);
      cyclonPos--;
    } else {
      forwards = true;
    }
  }
}


// Constructor of EffectDots class
EffectDots::EffectDots(CRGB leds[], uint8_t& bpm, uint8_t& fadeval)
	: Effect(leds), bpm(bpm), fadeval(fadeval)
{
	Serial.println("EffectDots constructor called");
}

EffectDots::~EffectDots()
{

}

void EffectDots::loop()
{
  uint8_t inner = beatsin8(bpm, NUM_LEDS / 4, NUM_LEDS / 4 * 3);
  uint8_t outer = beatsin8(bpm, 0, NUM_LEDS - 1);
  uint8_t middle = beatsin8(bpm, NUM_LEDS / 3, NUM_LEDS / 3 * 2);
  m_Leds[middle] = CRGB::Purple;
  m_Leds[inner] = CRGB::Blue;
  m_Leds[outer] = CRGB::Aqua;
  nscale8(m_Leds, NUM_LEDS, fadeval);
}


// Constructor of EffectLightning class
EffectLightning::EffectLightning(CRGB leds[])
	: Effect(leds), ledstart(0), ledlen(0), lightningFlashTime(0), lightningFlashDelay(0), dimmer(1), flashes(8), frequency(50)
{
	Serial.println("EffectLightning constructor called");
}

EffectLightning::~EffectLightning()
{

}

void EffectLightning::loop()
{
  ledstart = random16(NUM_LEDS);           // Determine starting location of flash
  ledlen = random16(NUM_LEDS - ledstart);  // Determine length of flash (not to go beyond NUM_LEDS-1)
  
  if (millis() - lightningFlashTime >= lightningFlashDelay){
    for (int lightningFlashCounter = 0; lightningFlashCounter < random8(3, flashes); lightningFlashCounter++) {
      if (lightningFlashCounter == 0) dimmer = 5;             // the brightness of the leader is scaled down by a factor of 5
      else dimmer = random8(1, 3);                            // return strokes are brighter than the leader
      fill_solid(m_Leds + ledstart, ledlen, CHSV(255, 0, 255 / dimmer));
      FastLED.show();                                         // Show a section of LED's
      delay(random8(4, 10));                                  // each flash only lasts 4-10 milliseconds
      fill_solid(m_Leds + ledstart, ledlen, CHSV(255, 0, 0)); // Clear the section of LED's
      FastLED.show();
      if (lightningFlashCounter == 0) delay (150);            // longer delay until next flash after the leader
      delay(50 + random8(100));                               // shorter delay between strokes
    }
    lightningFlashDelay = random8(frequency) * 100;           // delay between strikes
    lightningFlashTime = millis();
  }
}


// Constructor of EffectPoliceAll class
EffectPoliceAll::EffectPoliceAll(CRGB leds[])
	: Effect(leds), idex(0), thishuepolice(0), thissat(255)
{

  Serial.println("EffectPoliceAll constructor called");
}

EffectPoliceAll::~EffectPoliceAll()
{
  
}

void EffectPoliceAll::loop()
{
  idex++;
  if (idex >= NUM_LEDS) {
    idex = 0;
  }
  int idexR = idex;
  int idexB = antipodal_index(idexR);
  int thathue = (thishuepolice + 160) % 255;
  m_Leds[idexR] = CHSV(thishuepolice, thissat, 255);
  m_Leds[idexB] = CHSV(thathue, thissat, 255);
}


// Constructor of EffectPoliceOne class
EffectPoliceOne::EffectPoliceOne(CRGB leds[])
	: Effect(leds), idex(0), thishuepolice(0), thissat(255)
{

  Serial.println("EffectPoliceOne constructor called");
}

EffectPoliceOne::~EffectPoliceOne()
{
  
}

void EffectPoliceOne::loop()
{
  idex++;
  if (idex >= NUM_LEDS) {
    idex = 0;
  }
  int idexR = idex;
  int idexB = antipodal_index(idexR);
  int thathue = (thishuepolice + 160) % 255;
  for (int i = 0; i < NUM_LEDS; i++ ) {
    if (i == idexR) {
      m_Leds[i] = CHSV(thishuepolice, thissat, 255);
    }
    else if (i == idexB) {
      m_Leds[i] = CHSV(thathue, thissat, 255);
    }
    else {
      m_Leds[i] = CHSV(0, 0, 0);
    }
  }
}


// Constructor of EffectRainbow class
EffectRainbow::EffectRainbow(CRGB leds[])
	: Effect(leds), thishue(0), deltahue(10)
{

  Serial.println("EffectRainbow constructor called");
}

EffectRainbow::~EffectRainbow()
{
  
}

void EffectRainbow::loop()
{
  thishue++;
  fill_rainbow(m_Leds, NUM_LEDS, thishue, deltahue);
}


// Constructor of EffectGlitterRainbow class
EffectGlitterRainbow::EffectGlitterRainbow(CRGB leds[])
	: Effect(leds), thishue(0), deltahue(10)
{

  Serial.println("EffectGlitterRainbow constructor called");
}

EffectGlitterRainbow::~EffectGlitterRainbow()
{
  
}

void EffectGlitterRainbow::addGlitter( fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    m_Leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void EffectGlitterRainbow::loop()
{
  thishue++;
  fill_rainbow(m_Leds, NUM_LEDS, thishue, deltahue);
  addGlitter(80);
}


// Constructor of EffectTwinkle class
EffectTwinkle::EffectTwinkle(CRGB leds[])
	: Effect(leds), density(80)
{

  Serial.println("EffectTwinkle constructor called");
}

EffectTwinkle::~EffectTwinkle()
{
  
}

void EffectTwinkle::loop()
{
  const CRGB lightcolor(8, 7, 1);
  for ( int i = 0; i < NUM_LEDS; i++) {
    if ( !m_Leds[i]) continue;            // skip black pixels
    if ( m_Leds[i].r & 1) {               // is red odd?
      m_Leds[i] -= lightcolor;            // darken if red is odd
    } else {
      m_Leds[i] += lightcolor;            // brighten if red is even
    }
  }
  if ( random8() < density) {
    int j = random16(NUM_LEDS);
    if ( !m_Leds[j] ) m_Leds[j] = lightcolor;
  }
}


