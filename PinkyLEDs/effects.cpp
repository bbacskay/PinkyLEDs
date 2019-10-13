#include "effects.h"

#define TOP_INDEX (NUM_LEDS / 2)


/******************************************************************************
 *                                                                            *
 *                                 Effect class                               *
 *                                                                            *
 *****************************************************************************/

Effect::Effect(CRGB leds[], String name)
  : m_Leds(leds), m_Name(name)
{
  
}

Effect::~Effect()
{

}

String Effect::GetEffectName() const
{
  return m_Name;
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

void Effect::resetStripe()
{
  FastLED.clear();
  FastLED.show();
}

/******************************************************************************
 *                                                                            *
 *                            EffectConfetti class                            *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectConfetti class
EffectConfetti::EffectConfetti(CRGB leds[], uint8_t& Rcolor, uint8_t& Gcolor, uint8_t& Bcolor)
	: Effect(leds, "Confetti"), Rcolor(Rcolor), Gcolor(Gcolor), Bcolor(Bcolor)
{
	Serial.println("EffectConfetti constructor called");
}

// Destructor of EffectConfetti class
EffectConfetti::~EffectConfetti() {}

void EffectConfetti::loop()
{
  fadeToBlackBy( m_Leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  m_Leds[pos] += CRGB(Rcolor + random8(64), Gcolor, Bcolor);
}

/******************************************************************************
 *                                                                            *
 *                             EffectGlitter class                            *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectGlitter class
EffectGlitter::EffectGlitter(CRGB leds[], uint8_t& Rcolor, uint8_t& Gcolor, uint8_t& Bcolor)
	: Effect(leds, "Glitter"), Rcolor(Rcolor), Gcolor(Gcolor), Bcolor(Bcolor)
{
	Serial.println("EffectGlitter constructor called");
}

// Destructor of EffectGlitter class
EffectGlitter::~EffectGlitter() {}

//
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

/******************************************************************************
 *                                                                            *
 *                             EffectJuggle class                             *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectJuggle class
EffectJuggle::EffectJuggle(CRGB leds[], uint8_t& Rcolor, uint8_t& Gcolor, uint8_t& Bcolor)
	: Effect(leds, "Juggle"), Rcolor(Rcolor), Gcolor(Gcolor), Bcolor(Bcolor)
{
	Serial.println("class_EffectJuggle constructor called");
}

// Destructor of EffectJuggle class
EffectJuggle::~EffectJuggle() {}

// Juggle's main loop
void EffectJuggle::loop()
{
  fadeToBlackBy( m_Leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for ( int i = 0; i < 8; i++) {
    m_Leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CRGB(Rcolor, Gcolor, Bcolor);
    dothue += 32;
  }
}


/******************************************************************************
 *                                                                            *
 *                            EffectSinelon class                             *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectSinelon class
EffectSinelon::EffectSinelon(CRGB leds[], uint8_t& Rcolor, uint8_t& Gcolor, uint8_t& Bcolor)
	: Effect(leds, "Sinelon"), Rcolor(Rcolor), Gcolor(Gcolor), Bcolor(Bcolor)
{
	Serial.println("class_EffectSinelon constructor called");
}

// Destructor of EffectSinelon class
EffectSinelon::~EffectSinelon() {}

// Sinelon's main loop
void EffectSinelon::loop()
{
  fadeToBlackBy( m_Leds, NUM_LEDS, 20);
  uint16_t pos = beatsin16(13, 0, NUM_LEDS - 1);
  m_Leds[pos] += CRGB(Rcolor, Gcolor, Bcolor);
}


/******************************************************************************
 *                                                                            *
 *                             EffectSolid class                              *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectSolid class
EffectSolid::EffectSolid(CRGB leds[], uint8_t& Rcolor, uint8_t& Gcolor, uint8_t& Bcolor)
	: Effect(leds, "Solid"), Rcolor(Rcolor), Gcolor(Gcolor), Bcolor(Bcolor)
{
	Serial.println("class_EffectSolid constructor called");
}

// Destructor of EffectSolid class
EffectSolid::~EffectSolid() {}

// Solid's main loop
void EffectSolid::loop()
{
  fill_solid(m_Leds, NUM_LEDS, CRGB(Rcolor, Gcolor, Bcolor));
}


/******************************************************************************
 *                                                                            *
 *                           EffectChristmas class                            *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectChristmas class
EffectChristmas::EffectChristmas(CRGB leds[], uint8_t& gHue )
	: Effect(leds, "Christmas"), gHue(gHue)
{
	Serial.println("EffectChristmas constructor called");
}

// Destructor of EffectChristmas class
EffectChristmas::~EffectChristmas() {}

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

// Christmas's main loop
void EffectChristmas::loop()
{
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = bhw2_xmas_gp;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) {
    m_Leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}


/******************************************************************************
 *                                                                            *
 *                           EffectCandyCane class                            *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectCandyCane class
EffectCandyCane::EffectCandyCane(CRGB leds[])
	: Effect(leds, "Candy Cane"), startIndex(0)
{
  // Init palette
  setupPalette( CRGB::Red, CRGB::Red, CRGB::White, CRGB::White );

  Serial.println("EffectCandyCane constructor called");
}

// Destructor of EffectCandyCane class
EffectCandyCane::~EffectCandyCane() {}

//
void EffectCandyCane::setupPalette( CRGB A, CRGB AB, CRGB B, CRGB BA) {
  currentPalettestriped = CRGBPalette16(
                        A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B
                      );
}

// Candy Cane's main loop
void EffectCandyCane::loop()
{
  startIndex += 1;   // higher = faster motion

  fill_palette( m_Leds, NUM_LEDS,
                startIndex, 16,  // higher = narrower stripes
                currentPalettestriped, 255, LINEARBLEND);

}


/******************************************************************************
 *                                                                            *
 *                          EffectHollyJolly class                            *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectHollyJolly class
EffectHollyJolly::EffectHollyJolly(CRGB leds[])
	: Effect(leds, "Holly Jolly"), startIndex(0)
{
  // Init palette
  setupPalette( CRGB::Red, CRGB::Red, CRGB::Green, CRGB::Green );

  Serial.println("EffectHollyJolly constructor called");
}

// Destructor of EffectHollyJolly class
EffectHollyJolly::~EffectHollyJolly() {}

//
void EffectHollyJolly::setupPalette( CRGB A, CRGB AB, CRGB B, CRGB BA) {
  HJPalettestriped = CRGBPalette16(
                        A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B
                      );
}

// Holly Jolly's main loop
void EffectHollyJolly::loop()
{
  startIndex += 1;   // higher = faster motion

  fill_palette( m_Leds, NUM_LEDS,
                startIndex, 16,  // higher = narrower stripes
                HJPalettestriped, 255, LINEARBLEND);

}


/******************************************************************************
 *                                                                            *
 *                           EffectLoveyDay class                             *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectLoveyDay class
EffectLoveyDay::EffectLoveyDay(CRGB leds[])
	: Effect(leds, "Lovey Day"), idex(0), thishueLovey(0), thissat(255)
{

  Serial.println("EffectLoveyDay constructor called");
}

// Destructor of EffectLoveyDay class
EffectLoveyDay::~EffectLoveyDay() {}

// Lovey Day's main loop
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


/******************************************************************************
 *                                                                            *
 *                            EffectStPatty class                             *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectStPatty class
EffectStPatty::EffectStPatty(CRGB leds[], uint8_t& gHue)
	: Effect(leds, "St Patty"), gHue(gHue)
{
	Serial.println("EffectStPatty constructor called");
}

// Destructor of EffectStPatty class
EffectStPatty::~EffectStPatty() {}

// Palette for St Patty
DEFINE_GRADIENT_PALETTE( bhw2_greenman_gp ) {
  0,   1, 22,  1,
  130,   1, 168,  2,
  255,   1, 22,  1
};

// St Patty's main loop
void EffectStPatty::loop()
{
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = bhw2_greenman_gp;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) {
    m_Leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
};


/******************************************************************************
 *                                                                            *
 *                             EffectEaster class                             *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectEaster class
EffectEaster::EffectEaster(CRGB leds[])
	: Effect(leds, "Easter"), scale(30), maxChanges(48), targetPalette(OceanColors_p), currentPalette(CRGB::Black)
{
	Serial.println("EffectEaster constructor called");
}

// Destructor of EffectEaster class
EffectEaster::~EffectEaster() {}

// Easter's main loop
void EffectEaster::loop()
{
  EVERY_N_MILLISECONDS(10) {
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // FOR NOISE ANIMATION

    for (int i = 0; i < NUM_LEDS; i++) {                                     // Just ONE loop to fill up the LED array as all of the pixels change.
      uint8_t index = inoise8(i * scale, dist + i * scale) % 255;            // Get a value from the noise function. I'm using both x and y axis.
      m_Leds[i] = ColorFromPalette(currentPalette, index, 255, LINEARBLEND); // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    }
    dist += beatsin8(10, 1, 4);                                              // Moving along the distance (that random number we started out with). Vary it a bit with a sine wave.
  }

  EVERY_N_SECONDS(5) {
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 192, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)));
  }
}

/******************************************************************************
 *                                                                            *
 *                           EffectValentine class                            *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectValentine class
EffectValentine::EffectValentine(CRGB leds[], uint8_t& gHue)
	: Effect(leds, "Valentine"), gHue(gHue)
{
	Serial.println("EffectValentine constructor called");
}

// Destructor of EffectStPatty class
EffectValentine::~EffectValentine() {}

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

// Valentine's main loop
void EffectValentine::loop()
{
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = bhw2_redrosey_gp;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) {
    m_Leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}


/******************************************************************************
 *                                                                            *
 *                           EffectTurkeyDay class                            *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectTurkeyDay class
EffectTurkeyDay::EffectTurkeyDay(CRGB leds[], uint8_t& gHue)
	: Effect(leds, "Turkey Day"), gHue(gHue)
{
	Serial.println("EffectTurkeyDay constructor called");
}

// Destructor of EffectTurkeyDay class
EffectTurkeyDay::~EffectTurkeyDay() {}

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

// Turkey Day's main loop
void EffectTurkeyDay::loop()
{
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = bhw2_thanks_gp;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) {
    m_Leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}


/******************************************************************************
 *                                                                            *
 *                          EffectThanksgiving class                          *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectThanksgiving class
EffectThanksgiving::EffectThanksgiving(CRGB leds[])
	: Effect(leds, "Thanksgiving"), startIndex(0)
{
  // Init palette
  setupPalette( CRGB::OrangeRed, CRGB::Olive, CRGB::Maroon, CRGB::Maroon);

  Serial.println("EffectThanksgiving constructor called");
}

// Destructor of EffectThanksgiving class
EffectThanksgiving::~EffectThanksgiving() {}


void EffectThanksgiving::setupPalette( CRGB A, CRGB AB, CRGB B, CRGB BA) {
  ThxPalettestriped = CRGBPalette16(
                        A, A, A, A, A, A, A, AB, AB, AB, B, B, B, B, B, B
                      );
}

// Thanksgiving's main loop
void EffectThanksgiving::loop()
{
  startIndex += 1;   // higher = faster motion

  fill_palette( m_Leds, NUM_LEDS,
                startIndex, 16,  // higher = narrower stripes
                ThxPalettestriped, 255, LINEARBLEND);

}


/******************************************************************************
 *                                                                            *
 *                               EffectUsa class                              *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectUsa class
EffectUsa::EffectUsa(CRGB leds[], uint8_t& gHue)
	: Effect(leds, "USA"), gHue(gHue)
{
	Serial.println("EffectUsa constructor called");
}

// Destructor of EffectUsa class
EffectUsa::~EffectUsa() {}

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

// Usa's main loop
void EffectUsa::loop()
{
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = bhw3_41_gp;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) {
    m_Leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}


/******************************************************************************
 *                                                                            *
 *                          EffectIndependence class                          *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectIndependence class
EffectIndependence::EffectIndependence(CRGB leds[])
	: Effect(leds, "Independence"), startIndex(0)
{
  // Init palette
  setupPalette( CRGB::FireBrick, CRGB::Cornsilk, CRGB::MediumBlue, CRGB::MediumBlue );

  Serial.println("EffectIndependence constructor called");
}

// Destructor of EffectIndependence class
EffectIndependence::~EffectIndependence() {}

void EffectIndependence::setupPalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
{
  IndPalettestriped = CRGBPalette16(
                        A, A, A, A, A, AB, AB, AB, AB, AB, B, B, B, B, B, B
                      );
}

// Independence's main loop
void EffectIndependence::loop()
{
  startIndex += 1;   // higher = faster motion 

  fill_palette( m_Leds, NUM_LEDS,
                startIndex, 16,  // higher = narrower stripes
                IndPalettestriped, 255, LINEARBLEND);
}


/******************************************************************************
 *                                                                            *
 *                              EffectGoBlue class                            *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectGoBlue class
EffectGoBlue::EffectGoBlue(CRGB leds[], uint8_t& gHue)
	: Effect(leds, "Go Blue"), gHue(gHue)
{

  Serial.println("EffectGoBlue constructor called");
}

// Destructor of EffectGoBlue class
EffectGoBlue::~EffectGoBlue() {}

//palette for Go Blue
DEFINE_GRADIENT_PALETTE( Pills_3_gp ) {
  0,   4, 12, 122,
  127,  55, 58, 50,
  255, 192, 147, 11
};

// Go Blue's main loop
void EffectGoBlue::loop()
{
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = Pills_3_gp;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) { //9948
    m_Leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}


/******************************************************************************
 *                                                                            *
 *                               EffectHail class                             *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectHail class
EffectHail::EffectHail(CRGB leds[])
	: Effect(leds, "Hail"), startIndex(0)
{
  // Init palette
  setupPalette( CRGB::Blue, CRGB::Blue, CRGB::Yellow, CRGB::Yellow );

  Serial.println("EffectHail constructor called");
}

// Destructor of EffectHail class
EffectHail::~EffectHail() {}


void EffectHail::setupPalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
{
  hailPalettestriped = CRGBPalette16(
                        A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B
                      );
}

// Hail's main loop
void EffectHail::loop()
{
  startIndex += 1;                // higher = faster motion

  fill_palette( m_Leds, NUM_LEDS,
                startIndex, 16,   // higher = narrower stripes
                hailPalettestriped, 255, LINEARBLEND);
}


/******************************************************************************
 *                                                                            *
 *                            EffectTouchdown class                           *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectTouchdown class
EffectTouchdown::EffectTouchdown(CRGB leds[])
	: Effect(leds, "Touchdown"), idex(0), thishuetouch(64), thissat(255)
{

  Serial.println("EffectTouchdown constructor called");
}

// Destructor of EffectTouchdown class
EffectTouchdown::~EffectTouchdown() {}

// Touchdown's main loop
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

  FastLED.delay(1);
}


/******************************************************************************
 *                                                                            *
 *                            EffectHalloween class                           *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectHalloween class
EffectHalloween::EffectHalloween(CRGB leds[], uint8_t& gHue)
	: Effect(leds, "Halloween"), gHue(gHue)
{

  Serial.println("EffectHalloween constructor called");
}

// Destructor of EffectTouchdown class
EffectHalloween::~EffectHalloween() {}

//palette for Halloween
DEFINE_GRADIENT_PALETTE( Orange_to_Purple_gp ) {
  0, 208, 50,  1,
  127, 146, 27, 45,
  255,  97, 12, 178
};

// Halloween's main loop
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


/******************************************************************************
 *                                                                            *
 *                             EffectPunkin class                             *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectPunkin class
EffectPunkin::EffectPunkin(CRGB leds[])
	: Effect(leds, "Punkin"), startIndex(0)
{
  // Init palette
  setupPalette( CRGB::DarkOrange, CRGB::DarkOrange, CRGB::Indigo, CRGB::Indigo );

  Serial.println("EffectPunkin constructor called");
}

// Destructor of EffectPunkin class
EffectPunkin::~EffectPunkin() {}


void EffectPunkin::setupPalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
{
  PunkinPalettestriped = CRGBPalette16(
                        A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B
                      );
}

// Punkin's main loop
void EffectPunkin::loop()
{
  startIndex += 1;  // higher = faster motion

  fill_palette( m_Leds, NUM_LEDS,
                startIndex, 16, // higher = narrower stripes
                PunkinPalettestriped, 255, LINEARBLEND);
}


/******************************************************************************
 *                                                                            *
 *                               EffectBpm class                              *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectBpm class
EffectBpm::EffectBpm(CRGB leds[], uint8_t& gHue)
	: Effect(leds, "BPM"), gHue(gHue)
{
	Serial.println("EffectBpm constructor called");
}

// Destructor of EffectBpm class
EffectBpm::~EffectBpm() {}

// Bpm's main loop
void EffectBpm::loop()
{
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) {
    m_Leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}


/******************************************************************************
 *                                                                            *
 *                         EffectCyclonRainbow class                          *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectCyclonRainbow class
EffectCyclonRainbow::EffectCyclonRainbow(CRGB leds[])
	: Effect(leds, "Cyclon Rainbow"), cyclonHue(0), cyclonPos(0), forwards(false)
{
	Serial.println("EffectCyclonRainbow constructor called");
}

// Destructor of EffectCyclonRainbow class
EffectCyclonRainbow::~EffectCyclonRainbow() {}

// Cyclon Rainbow's main loop
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


/******************************************************************************
 *                                                                            *
 *                              EffectDots class                              *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectDots class
EffectDots::EffectDots(CRGB leds[], uint8_t& bpm, uint8_t& fadeval)
	: Effect(leds, "Dots"), bpm(bpm), fadeval(fadeval)
{
	Serial.println("EffectDots constructor called");
}

// Destructor of EffectDots class
EffectDots::~EffectDots() {}

// Dots' main loop
void EffectDots::loop()
{
  uint16_t inner = beatsin16(bpm, NUM_LEDS / 4, NUM_LEDS / 4 * 3);
  uint16_t outer = beatsin16(bpm, 0, NUM_LEDS - 1);
  uint16_t middle = beatsin16(bpm, NUM_LEDS / 3, NUM_LEDS / 3 * 2);
  m_Leds[middle] = CRGB::Purple;
  m_Leds[inner] = CRGB::Blue;
  m_Leds[outer] = CRGB::Aqua;
  nscale8(m_Leds, NUM_LEDS, fadeval);
}


/******************************************************************************
 *                                                                            *
 *                           EffectLightning class                            *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectLightning class
EffectLightning::EffectLightning(CRGB leds[])
	: Effect(leds, "Lightning"), ledstart(0), ledlen(0), lightningFlashTime(0), lightningFlashDelay(0), dimmer(1), flashes(8), frequency(50)
{
	Serial.println("EffectLightning constructor called");
}

// Destructor of EffectLightning class
EffectLightning::~EffectLightning() {}

// Lightning's main loop
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


/******************************************************************************
 *                                                                            *
 *                           EffectPoliceAll class                            *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectPoliceAll class
EffectPoliceAll::EffectPoliceAll(CRGB leds[])
	: Effect(leds, "Police All"), idex(0), thishuepolice(0), thissat(255)
{

  Serial.println("EffectPoliceAll constructor called");
}

// Destructor of EffectPoliceAll class
EffectPoliceAll::~EffectPoliceAll()
{
  
}

// Police All's main loop
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


/******************************************************************************
 *                                                                            *
 *                           EffectPoliceOne class                            *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectPoliceOne class
EffectPoliceOne::EffectPoliceOne(CRGB leds[])
	: Effect(leds, "Police One"), idex(0), thishuepolice(0), thissat(255)
{

  Serial.println("EffectPoliceOne constructor called");
}

// Destructor of EffectPoliceOne class
EffectPoliceOne::~EffectPoliceOne() {}

// Police One's main loop
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


/******************************************************************************
 *                                                                            *
 *                             EffectRainbow class                            *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectRainbow class
EffectRainbow::EffectRainbow(CRGB leds[])
	: Effect(leds, "Rainbow"), thishue(0), deltahue(10)
{

  Serial.println("EffectRainbow constructor called");
}

// Destructor of EffectRainbow class
EffectRainbow::~EffectRainbow() {}

// Rainbow's main loop
void EffectRainbow::loop()
{
  thishue++;
  fill_rainbow(m_Leds, NUM_LEDS, thishue, deltahue);
}


/******************************************************************************
 *                                                                            *
 *                         EffectGlitterRainbow class                         *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectGlitterRainbow class
EffectGlitterRainbow::EffectGlitterRainbow(CRGB leds[])
	: Effect(leds, "Glitter Rainbow"), thishue(0), deltahue(10)
{

  Serial.println("EffectGlitterRainbow constructor called");
}

// Destructor of EffectRainbow class
EffectGlitterRainbow::~EffectGlitterRainbow() {}

void EffectGlitterRainbow::addGlitter( fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    m_Leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

// Glitter Rainbow's main loop
void EffectGlitterRainbow::loop()
{
  thishue++;
  fill_rainbow(m_Leds, NUM_LEDS, thishue, deltahue);
  addGlitter(80);
}


/******************************************************************************
 *                                                                            *
 *                             EffectTwinkle class                            *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectTwinkle class
EffectTwinkle::EffectTwinkle(CRGB leds[])
	: Effect(leds, "Twinkle"), density(80)
{

  Serial.println("EffectTwinkle constructor called");
}

// Destructor of EffectTwinkle class
EffectTwinkle::~EffectTwinkle() {}

// Twinkle's main loop
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

/******************************************************************************
 *                                                                            *
 *                              EffectFire class                              *
 *                                                                            *
 * ***************************************************************************/

// Constructor of EffectFire class
EffectFire::EffectFire(CRGB leds[])
	: Effect(leds, "Fire"), gReverseDirection(false), gPal(HeatColors_p)
{

  Serial.println("EffectFire constructor called");
}

EffectFire::~EffectFire() {}


void EffectFire::resetStripe()
{
  // Reset heat array
  for (uint16_t i=0; i<NUM_LEDS;i++)
  {
    heat[i]=0;
  }

  // Call the resetStrip function from the parent
  Effect::resetStripe();
}

// Fire2012WithPalette
void EffectFire::loop()
{

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  // Step 4.  Map from heat cells to LED colors
  for ( int j = 0; j < NUM_LEDS; j++) {
    // Scale the heat value from 0-255 down to 0-240
    // for best results with color palettes.
    uint8_t colorindex = scale8( heat[j], 240);
    CRGB color = ColorFromPalette( gPal, colorindex);
    int pixelnumber;
    if ( gReverseDirection ) {
      pixelnumber = (NUM_LEDS - 1) - j;
    } else {
      pixelnumber = j;
    }
    m_Leds[pixelnumber] = color;
  }

}

/******************************************************************************
 *                                                                            *
 *                              EffectRipple class                            *
 *                                                                            *
 *****************************************************************************/

// Constructor of EffectTwinkle class
EffectRipple::EffectRipple(CRGB leds[])
	: Effect(leds, "Ripple"), colour(0), center(0), step(-1), bgcol(0), myfade(255)
{

  Serial.println("EffectRipple constructor called");
}

// Destructor of EffectRipple class
EffectRipple::~EffectRipple() {}

// Ripple's main loop
void EffectRipple::loop()
{
  EVERY_N_MILLISECONDS(10) {
    for (int i = 0; i < NUM_LEDS; i++) m_Leds[i] = CHSV(bgcol++, 255, 15);  // Rotate background colour.
    switch (step) {
      case -1:                                                          // Initialize ripple variables.
        center = random(NUM_LEDS);
        colour = random8();
        step = 0;
        break;
      case 0:
        m_Leds[center] = CHSV(colour, 255, 255);                        // Display the first pixel of the ripple.
        step ++;
        break;
      case MAX_STEPS:                                                   // At the end of the ripples.
        step = -1;
        break;
      default:                                                             // Middle of the ripples.
        m_Leds[(center + step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, myfade / step * 2);   // Simple wrap from Marc Miller
        m_Leds[(center - step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, myfade / step * 2);
        step ++;                                                         // Next step.
        break;
    }
  }
}