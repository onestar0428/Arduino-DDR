#include "pitches.h"
#include <U8glib.h>
/*

  Bitmap.pde

  Show simple bitmap

  >>> Before compiling: Please remove comment from the constructor of the
  >>> connected graphics display (see below).

  Universal 8bit Graphics Library, https://github.com/olikraus/u8glib/

  Copyright (c) 2011, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.

    Redistributions in binary form must reproduce the above copyright notice, this
    list of conditions and the following disclaimer in the documentation and/or other
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
//////////////////////////cr
// setup u8g object, please remove comment from one of the following constructor calls
// IMPORTANT NOTE: The following list is incomplete. The complete list of supported
// devices with all constructor calls is here: https://github.com/olikraus/u8glib/wiki/device
U8GLIB_ST7920_128X64_1X u8g(10);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16


int plus = 0;
int plusTemp = 0;

int length = 0;
boolean isGameOver = false;
boolean isClearGame = false;

const uint8_t heart[] PROGMEM = {0x22, 0x77, 0x7f, 0x3e, 0x1c, 0x08,};
const uint8_t emptyHeart[] PROGMEM = {0x22, 0x55, 0x49, 0x22, 0x14, 0x08,};
const uint8_t line[] PROGMEM = {0xff,};
const uint8_t leftLine[] PROGMEM = {0x80,};
const uint8_t rightLine[] PROGMEM = {0x01,};

//const uint8_t bar[] PROGMEM = {};
const unsigned char PROGMEM bar[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                     0xff, 0xff, 0xff, 0xff,
                                    };

void draw();
void setMenuMode();
void game();
void drawGameOver(int );
void drawClearGame(int );

////////////////////jy
#define TOUCH_SENSOR1 12
#define TOUCH_SENSOR2 8
#define TOUCH_SENSOR3 7
#define TOUCH_SENSOR4 4
#define PIEZO 2

int song[ ] = {NOTE_G4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_G4, 0, NOTE_G4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_A4, 0, NOTE_A4, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_B4, 0, NOTE_G5, NOTE_G5, NOTE_F5, NOTE_D5, NOTE_E5, NOTE_C5, 0,
               NOTE_G4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_G4, 0, NOTE_G4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_A4, 0, NOTE_A4, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_F5, NOTE_D5, NOTE_C5, 0,
               NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5, NOTE_E5, 0, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_G5, 0,
               NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5, NOTE_E5, 0, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_G5, NOTE_G5, NOTE_F5, NOTE_D5, NOTE_C5, 0
              };
int ts[ ] = { 250, 250, 250, 250, 750, 250, 250, 250, 250, 250, 750, 250, 250, 250, 250, 250, 750, 250, 250, 250, 250, 250, 500, 250, 250,
              250, 250, 250, 250, 750, 250, 250, 250, 250, 250, 750, 250, 250, 250, 250, 250, 450, 50, 250, 250, 250, 250, 250, 250, 750, 250,
              250, 250, 500, 250, 250, 500, 250, 250, 400, 100, 750, 250, 250, 250, 400, 100, 250, 250, 250, 250, 250, 250, 250, 250, 500, 250, 250,
              250, 250, 500, 250, 250, 500, 250, 250, 400, 100, 750, 250, 250, 250, 400, 100, 250, 250, 250, 250, 250, 250, 250, 250, 750, 250
            };
int touch[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

int num = 0, score = 0, life = 5, flag = 1;

void setup() {
  Serial.begin(9600);
  pinMode(TOUCH_SENSOR1, INPUT);
  pinMode(PIEZO, OUTPUT);

  for (int i = 3; i >= 1; i--) {
    Serial.println(i);
    delay(1000);
  }

  draw();
}

void loop()
{
  if (num != sizeof(song) / 2 && life != 0) {
    tone(PIEZO, song[num], ts[num]);
  }

  if (num % 4 == 1) {
    for (int i = 0; i < 4; i++) {
      touch[0][i] = random(2);
      Serial.print(touch[0][i]);
    } Serial.println("");
    if(touch[0][0] ==0 && touch[0][1] == 0 && touch[0][2] ==0 && touch[0][3] == 0){
      touch[0][0] = 1;
      touch[0][3] = 1;
    }
  }

  // picture loop
  u8g.firstPage();
  do {
    if (isGameOver) {
      int scoreResult = score;
      drawGameOver(scoreResult);
    } else if(isClearGame){
      int scoreResult = score;
      drawClearGame(scoreResult);
    } else {
      draw(); //background
      game();
    }
  } while ( u8g.nextPage() );

  if (num != sizeof(song) / 2 && life != 0) {

    if (touch[3][0] + touch[3][1] + touch[3][2] + touch[3][3] != 0 && num % 4 == 0) {

      Serial.println("PUSH!!");
      if (digitalRead(TOUCH_SENSOR1) == touch[3][0] && digitalRead(TOUCH_SENSOR2) == touch[3][1] && digitalRead(TOUCH_SENSOR3) == touch[3][2] && digitalRead(TOUCH_SENSOR4) == touch[3][3]) {
        score += 100;
        Serial.print("SCORE: ");
        Serial.println(score);
        delay(ts[num++]);
      } else if (life > 0) {
        life = life - 1;
        delay(ts[num++]);
      }
      Serial.print("----------------------------------------");
      Serial.println(life);
    }
    else {
      delay(ts[num++]);
    }

    for (int i = 2; i >= 0; i--) {
      for (int j = 3; j >= 0; j--) {
        touch[i + 1][j] = touch[i][j];
        touch[i][j] = 0;
      }
    }

    //digitalWrite(TOUCH_SENSOR1, LOW);
    noTone(PIEZO);
  }
  else if(num == sizeof(song) / 2){
    noTone(PIEZO);
    flag = 0;
    isClearGame = 1;
  }
  else {
    noTone(PIEZO);
    flag = 0;
    isGameOver = 1;
  }

}

void draw(void) {
  //u8g.setFont(u8g_font_fixed_v0); -- font size
  /////////////score////////////
  u8g.setFont(u8g_font_04b_03); //없으면 string 안 뜸 ㅠㅠ
  char buf [4];
  sprintf (buf, "%04i", score);
  u8g.drawStr(107, 6, buf);

  // graphic commands to redraw the complete screen should be placed here
  //////////////목숨 다섯개//////////////
  if (life == 5) {
    u8g.drawBitmapP( 0, 0, 1, 6, heart);
    u8g.drawBitmapP( 8, 0, 1, 6, heart);
    u8g.drawBitmapP( 16, 0, 1, 6, heart);
    u8g.drawBitmapP( 24, 0, 1, 6, heart);
    u8g.drawBitmapP( 32, 0, 1, 6, heart);
  }
  else if (life == 4) {
    u8g.drawBitmapP( 0, 0, 1, 6, heart);
    u8g.drawBitmapP( 8, 0, 1, 6, heart);
    u8g.drawBitmapP( 16, 0, 1, 6, heart);
    u8g.drawBitmapP( 24, 0, 1, 6, heart);
    u8g.drawBitmapP( 32, 0, 1, 6, emptyHeart);
  }
  else if (life == 3) {
    u8g.drawBitmapP( 0, 0, 1, 6, heart);
    u8g.drawBitmapP( 8, 0, 1, 6, heart);
    u8g.drawBitmapP( 16, 0, 1, 6, heart);
    u8g.drawBitmapP( 24, 0, 1, 6, emptyHeart);
    u8g.drawBitmapP( 32, 0, 1, 6, emptyHeart);
  }
  else if (life == 2) {
    u8g.drawBitmapP( 0, 0, 1, 6, heart);
    u8g.drawBitmapP( 8, 0, 1, 6, heart);
    u8g.drawBitmapP( 16, 0, 1, 6, emptyHeart);
    u8g.drawBitmapP( 24, 0, 1, 6, emptyHeart);
    u8g.drawBitmapP( 32, 0, 1, 6, emptyHeart);
  }
  else if (life == 1) {
    u8g.drawBitmapP( 0, 0, 1, 6, heart);
    u8g.drawBitmapP( 8, 0, 1, 6, emptyHeart);
    u8g.drawBitmapP( 16, 0, 1, 6, emptyHeart);
    u8g.drawBitmapP( 24, 0, 1, 6, emptyHeart);
    u8g.drawBitmapP( 32, 0, 1, 6, emptyHeart);
  }
  else if (life == 0) {
    u8g.drawBitmapP( 0, 0, 1, 6, emptyHeart);
    u8g.drawBitmapP( 8, 0, 1, 6, emptyHeart);
    u8g.drawBitmapP( 16, 0, 1, 6, emptyHeart);
    u8g.drawBitmapP( 24, 0, 1, 6, emptyHeart);
    u8g.drawBitmapP( 32, 0, 1, 6, emptyHeart);
  }

  /////////////// 4개 칸 구분 /////////////
  for (int i = 10; i < 64; i++) {
    u8g.drawBitmapP(32, i, 1, 1, leftLine);
    u8g.drawBitmapP(64, i, 1, 1, leftLine);
    u8g.drawBitmapP(96, i, 1, 1, leftLine);
  }

    for (int i = 49; i < 52; i++) {
      //first
      for (int j = 0; j < 32; j += 8) {
        u8g.drawBitmapP(j, i, 1, 1, line);
      }
      //second
      for (int j = 32; j < 64; j += 8) {
        u8g.drawBitmapP(j, i, 1, 1, line);
      }
      //third
      for (int j = 64; j < 96; j += 8) {
        u8g.drawBitmapP(j, i, 1, 1, line);
      }
      //fourth
      for (int j = 96; j < 128; j += 8) {
        u8g.drawBitmapP(j, i, 1, 1, line);
      }
    }
}

void drawGameOver(int s) {
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr( 0, 20, "Game Over!");

  char buf[4];
  sprintf (buf, "%04d", s);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(80, 40, buf);
}

void drawClearGame(int s){
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr( 0, 20, "Game Success!");
  
  char buf[4];
  sprintf (buf, "%04d", s);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(60, 40, buf);
  if(s>=2300){
    u8g.drawStr(55, 55, "Excellent!");
  } else {
    u8g.drawStr(55, 55, "Good Game!");  
  }
}
void game() {

  for (int i = 0; i < 4; i++) {
    if (touch[i][0] == 1) {
      u8g.drawBitmapP(0, 10 + 13 * i, 4, 13, bar);
    }
  }
  for (int i = 0; i < 4; i++) {
    if (touch[i][1] == 1) {
      u8g.drawBitmapP(32, 10 + 13 * i, 4, 13, bar);
    }
  }
  for (int i = 0; i < 4; i++) {
    if (touch[i][2] == 1) {
      u8g.drawBitmapP(64, 10 + 13 * i, 4, 13, bar);
    }
  }
  for (int i = 0; i < 4; i++) {
    if (touch[i][3] == 1) {
      u8g.drawBitmapP(96, 10 + 13 * i, 4, 13, bar);
    }
  }
}
