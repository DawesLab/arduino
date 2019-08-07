#include <Audio.h>
#include <SD.h>
#include <Bounce.h>
#include <FastLED.h>

// How many leds are in the strip?
#define NUM_LEDS 6

// Data pin that led data will be written out over
#define DATA_PIN 20

#define FRAMES_PER_SECOND  120

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

long randNumber;
const int buttonPin1 = 2;
Bounce pushbutton1 = Bounce(buttonPin1, 20);
const int buttonPin2 = 3;
Bounce pushbutton2 = Bounce(buttonPin2, 20);
const int buttonPin3 = 4;
Bounce pushbutton3 = Bounce(buttonPin3, 20);
const int buttonPin4 = 5;
Bounce pushbutton4 = Bounce(buttonPin4, 20);
const int buttonPin5 = 16;
Bounce pushbutton5 = Bounce(buttonPin5, 20);
const int buttonPin6 = 17;
Bounce pushbutton6 = Bounce(buttonPin6, 20);

AudioPlaySdWav           playSdWav1;
AudioOutputI2S           i2s1;
AudioConnection          patchCord1(playSdWav1, 0, i2s1, 0);
AudioConnection          patchCord2(playSdWav1, 1, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;


void setup() {
  delay(2000);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
  pinMode(buttonPin5, INPUT_PULLUP);
  pinMode(buttonPin6, INPUT_PULLUP);
  
  Serial.begin(9600);
  AudioMemory(8);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  SPI.setMOSI(7);
  SPI.setSCK(14);
  if (!(SD.begin(10))) {
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  while (!Serial && millis() < 2500) ; // wait 2.5 sec for Arduino Serial Monitor
  Serial.println("begin random player");
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, clearlights };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  

void loop() {
  pushbutton1.update();
  if (pushbutton1.fallingEdge()) {
    Serial.println("925.wav");
    playSdWav1.play("925.wav");
    gCurrentPatternNumber = 5;
  }
  pushbutton2.update();
  if (pushbutton2.fallingEdge()) {
    Serial.println("GTW");
    playSdWav1.play("gtw.wav");
    gCurrentPatternNumber = 4;
  }
  pushbutton3.update();
  if (pushbutton3.fallingEdge()) {
    Serial.println("I like it");
    playSdWav1.play("ilikeit.wav");
    gCurrentPatternNumber = 3;
  }
  pushbutton4.update();
  if (pushbutton4.fallingEdge()) {
    Serial.println("Oops");
    playSdWav1.play("oops.wav");
    gCurrentPatternNumber = 2;
  }
  pushbutton5.update();
  if (pushbutton5.fallingEdge()) {
    Serial.println("Queens Everywhere");
    playSdWav1.play("queens.wav");
    gCurrentPatternNumber = 1;
  }
  pushbutton6.update();
  if (pushbutton6.fallingEdge()) {
    Serial.println("Tik Tok");
    playSdWav1.play("tiktok.wav");
    gCurrentPatternNumber = 0;
  }
  if ( playSdWav1.isPlaying() == 0 ) {
    delay(200);
    if (playSdWav1.isPlaying() == 0 ) {
      FastLED.clear();
    }
  }
  if ( playSdWav1.isPlaying() ) {
//    gPatterns[gCurrentPatternNumber]();
//    // send the 'leds' array out to the actual LED strip
//    FastLED.show();  
//    // insert a delay to keep the framerate modest
//    FastLED.delay(1000/FRAMES_PER_SECOND);
  } else {
    gCurrentPatternNumber = 6;
  }
  gPatterns[gCurrentPatternNumber]();
  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND);
  EVERY_N_MILLISECONDS( 20 ) { gHue++; }
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void clearlights() {
  fill_solid( leds, NUM_LEDS, CRGB( 1, 1, 1) );
}
