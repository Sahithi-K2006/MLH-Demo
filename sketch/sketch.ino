// Gemma Emoji Matrix - MCU side (Arduino UNO Q)
// Python sends an emotion number over the Bridge, we draw that face
// on the 8 x 13 LED matrix.  0=HAPPY 1=SAD 2=ANGRY 3=LOVE 4=SURPRISED 5=NEUTRAL
// License: MIT

#include <Arduino_RouterBridge.h>
#include <Arduino_LED_Matrix.h>

Arduino_LED_Matrix matrix;

const uint8_t FRAMES[6][104] = {
  { // HAPPY
    0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,1,0,0,0,0,0,1,0,0,0,
    0,0,0,1,0,0,0,0,0,1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,1,0,0,0,0,0,0,0,1,0,0,
    0,0,0,1,0,0,0,0,0,1,0,0,0,
    0,0,0,0,1,1,1,1,1,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0
  },
  { // SAD
    0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,1,0,0,0,0,0,1,0,0,0,
    0,0,0,1,0,0,0,0,0,1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,1,1,1,1,1,0,0,0,0,
    0,0,0,1,0,0,0,0,0,1,0,0,0,
    0,0,1,0,0,0,0,0,0,0,1,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0
  },
  { // ANGRY
    0,0,1,0,0,0,0,0,0,0,1,0,0,
    0,0,0,1,0,0,0,0,0,1,0,0,0,
    0,0,0,1,1,0,0,0,1,1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,1,1,1,1,1,0,0,0,0,
    0,0,0,1,0,0,0,0,0,1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0
  },
  { // LOVE
    0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,1,1,0,0,0,1,1,0,0,0,
    0,0,1,1,1,1,0,1,1,1,1,0,0,
    0,0,1,1,1,1,1,1,1,1,1,0,0,
    0,0,0,1,1,1,1,1,1,1,0,0,0,
    0,0,0,0,1,1,1,1,1,0,0,0,0,
    0,0,0,0,0,1,1,1,0,0,0,0,0,
    0,0,0,0,0,0,1,0,0,0,0,0,0
  },
  { // SURPRISED
    0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,1,0,0,0,0,0,1,0,0,0,
    0,0,0,1,0,0,0,0,0,1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,1,1,1,0,0,0,0,0,
    0,0,0,0,1,0,0,0,1,0,0,0,0,
    0,0,0,0,0,1,1,1,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0
  },
  { // NEUTRAL
    0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,1,0,0,0,0,0,1,0,0,0,
    0,0,0,1,0,0,0,0,0,1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,1,1,1,1,1,1,1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0
  }
};

uint8_t frame[104];               // the picture we send to the matrix
volatile int pendingEmotion = 5;  // start with NEUTRAL
volatile bool hasNew = true;

// Python calls this through the Bridge. Only save the number and return
// fast; the drawing happens in loop().
void show_emotion(int id) {
  if (id < 0 || id > 5) id = 5;
  pendingEmotion = id;
  hasNew = true;
}

void setup() {
  matrix.begin();
  matrix.setGrayscaleBits(1);     // simple ON/OFF pixels
  Bridge.begin();
  Bridge.provide("show_emotion", show_emotion);
}

void loop() {
  if (hasNew) {
    hasNew = false;
    int id = pendingEmotion;
    for (int i = 0; i < 104; i++) frame[i] = FRAMES[id][i];
    matrix.draw(frame);
  }
  delay(10);
}
