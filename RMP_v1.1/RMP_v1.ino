#include <LiquidCrystal.h>

// Initialize 16-pin parallel LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int buzzerPin = 8;
const int btnNext = 6;    // Left button pin
const int btnSelect = 7;  // Right button pin

// Test
#define C8  4186  // ~4000Hz range
#define B7  3951
#define G7  3136
#define C7  2093
#define C6  1047
#define C5  523
#define C4  262   // Middle C
#define C3  131
#define A2  110   // ~100Hz range
#define L1  80    // Low 1.0
#define L2  70
#define L3  50
#define L4  40    // Low 4.0
#define L5  20
#define L6  10
#define L7  5
#define L8  1     // Low 8.0 (1hz)
// Tetris
#define C5  523
#define CS5 554
#define D5  587
#define DS5 622
#define E5  659
#define F5  698
#define FS5 740
#define G5  784
#define GS5 831
#define A5  880
#define AS5 932
#define B5  988
#define C6  1047
// SMB
#define C4 262
#define CS4 277
#define D4 294
#define DS4 311
#define E4 330
#define F4 349
#define FS4 370
#define G4 392
#define GS4 415
#define A4 440
#define AS4 466
#define B4 494
#define C5 523
#define CS5 554
#define D5 587
#define DS5 622
#define E5 659
#define F5 698
#define G5 784
#define A5 880

int currentMenuSelection = 0;  // 0 = Tetris, 1 = Mario
bool isPlaying = false;

int tetrisMelody[] = {
  E5, 4, B4, 8, C5, 8, D5, 4, C5, 8, B4, 8,
  A4, 4, A4, 8, C5, 8, E5, 4, D5, 8, C5, 8,
  B4, 4, B4, 8, C5, 8, D5, 4, E5, 4,
  C5, 4, A4, 4, A4, 4, 0, 4
};

int marioMelody[] = {
  E5,8, E5,8, 0,8, E5,8, 0,8, C5,8, E5,4, // Intro
  G5,4, 0,4, G4,4, 0,4, 
  C5,4, G4,8, 0,4, E4,4,                // Section 1 
  A4,4, B4,4, AS4,8, A4,4,
  G4,4, E5,4, G5,4, A5,4, F5,8, G5,8,
  0,4, E5,4, C5,8, D5,8, B4,4,
  C5,4, G4,8, 0,4, E4,4,                // Section 2 
  A4,4, B4,4, AS4,8, A4,4,
  G4,4, E5,4, G5,4, A5,4, F5,8, G5,8,
  0,4, E5,4, C5,8, D5,8, B4,4,
  0,4, G5,8, FS5,8, F5,8, DS5,4, E5,8,   // Bridge
  0,8, GS4,8, A4,8, C5,8, 0,8, A4,8, C5,8, D5,8,
  0,4, G5,8, FS5,8, F5,8, DS5,4, E5,8,
  0,8, C6,4, C6,8, C6,4, 
  0,4, G5,8, FS5,8, F5,8, DS5,4, E5,8,
  0,8, GS4,8, A4,8, C5,8, 0,8, A4,8, C5,8, D5,8,
  0,4, DS5,4, 0,8, D5,4, C5,2
};

int testMelody[] = {
  C8,2, B7,2, G7,2, C7,2, C6,2, C5,2,
  C4,2, C3,2, A2,2, L1,2, L2,2, L3,2, L4,2
};

// Splash screen
void setup() {
  lcd.begin(16, 2);

  pinMode(buzzerPin, OUTPUT);
  pinMode(btnNext, INPUT_PULLUP);
  pinMode(btnSelect, INPUT_PULLUP);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RMP v1.1        ");
  lcd.setCursor(0, 1);
  lcd.print("by officialrez  ");

  tone(buzzerPin, 892);
  delay(400);
  noTone(buzzerPin);

  delay(2000);

  // Main interface
  drawMenu();
}

void loop() {
  if (digitalRead(btnNext) == LOW) {
    // Play a sharp, quick click sound when skipping tracks
    tone(buzzerPin, 1500);
    delay(40);
    noTone(buzzerPin);

    isPlaying = false;
    currentMenuSelection = (currentMenuSelection + 1) % 3;
    drawMenu();
    delay(300);
  }

  if (digitalRead(btnSelect) == LOW) {
    isPlaying = !isPlaying;
    drawMenu();

    if (isPlaying) {
      delay(400);  // Debounce to allow button release before song loops check
      if (currentMenuSelection == 0) {
        playSong(tetrisMelody, sizeof(tetrisMelody) / sizeof(tetrisMelody[0]));
      } else if (currentMenuSelection == 1) {
        playSong(marioMelody, sizeof(marioMelody) / sizeof(marioMelody[0]));
      } else {
        playSong(testMelody, sizeof(testMelody) / sizeof(testMelody[0]));
      }
    } 
  }
}

void drawMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);

  if (currentMenuSelection == 0) {
    lcd.print(isPlaying ? "Playing: Tetris " : "Select: Tetris  ");
  } else if (currentMenuSelection == 1) {
    lcd.print(isPlaying ? "Playing: SMB    " : "Select: SMB     ");
  } else {
    lcd.print(isPlaying ? "Playing: Test   " : "Select: Test    ");
  }

  lcd.setCursor(0, 1);
  if (isPlaying) {
    lcd.print("< Next    Stop >");
  } else {
    lcd.print("< Next    Play >");
  }
}

void playSong(int melody[], int totalNotes) {
  for (int thisNote = 0; thisNote < totalNotes; thisNote += 2) {

    if (digitalRead(btnNext) == LOW || digitalRead(btnSelect) == LOW) {
      isPlaying = false;
      noTone(buzzerPin);
      drawMenu();
      delay(400);
      return;
    }

    int noteDuration = 1000 / melody[thisNote + 1];
    int currentPitch = melody[thisNote];

    if (currentPitch == 0) {
      noTone(buzzerPin);
    } else {
      tone(buzzerPin, currentPitch, noteDuration);
    }

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzerPin);
  }

  isPlaying = false;
  drawMenu();
}
