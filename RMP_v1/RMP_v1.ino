#include <LiquidCrystal.h>

// Initialize 16-pin parallel LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int buzzerPin = 8;  
const int btnNext = 6;    // Left button pin
const int btnSelect = 7;  // Right button pin

// Musical Note Frequencies
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880

int currentMenuSelection = 0; // 0 = Tetris, 1 = Mario
bool isPlaying = false;

// --- Melody Matrices ---
int tetrisMelody[] = {
  NOTE_E5, 4, NOTE_B4, 8, NOTE_C5, 8, NOTE_D5, 4, NOTE_C5, 8, NOTE_B4, 8,
  NOTE_A4, 4, NOTE_A4, 8, NOTE_C5, 8, NOTE_E5, 4, NOTE_D5, 8, NOTE_C5, 8,
  NOTE_B4, 4, NOTE_B4, 8, NOTE_C5, 8, NOTE_D5, 4, NOTE_E5, 4,
  NOTE_C5, 4, NOTE_A4, 4, NOTE_A4, 4, 0, 4
};

int marioMelody[] = {
  NOTE_E5, 8, NOTE_E5, 8, 0, 8, NOTE_E5, 8, 0, 8, NOTE_C5, 8, NOTE_E5, 4,
  NOTE_G5, 4, 0, 4, NOTE_G4, 4, 0, 4,
  NOTE_C5, 4, 0, 8, NOTE_G4, 8, 0, 4, NOTE_E4, 4,
  0, 8, NOTE_A4, 4, NOTE_B4, 4, NOTE_AS4, 8, NOTE_A4, 4
};

void setup() {
  lcd.begin(16, 2);
  
  pinMode(buzzerPin, OUTPUT);
  pinMode(btnNext, INPUT_PULLUP);
  pinMode(btnSelect, INPUT_PULLUP);
  
  // --- 1. IBM Boot Splash Screen Sequence ---
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RETRO PLAYER v1 ");
  lcd.setCursor(0, 1);
  lcd.print("By Andres       ");
  
  tone(buzzerPin, 892);  
  delay(300);            
  noTone(buzzerPin);
  
  delay(1600);           
  
  // --- 2. Initialize Main Interface ---
  drawMenu();
}

void loop() {
  if (digitalRead(btnNext) == LOW) {
    // Play a sharp, quick click sound when skipping tracks
    tone(buzzerPin, 1500); 
    delay(40);
    noTone(buzzerPin);

    isPlaying = false; 
    currentMenuSelection = (currentMenuSelection + 1) % 2; 
    drawMenu();
    delay(300); 
  }

  if (digitalRead(btnSelect) == LOW) {
    isPlaying = !isPlaying;
    drawMenu();
    
    if (isPlaying) {
      delay(400); // Debounce to allow button release before song loops check
      if (currentMenuSelection == 0) {
        playSong(tetrisMelody, sizeof(tetrisMelody) / sizeof(tetrisMelody[0]));
      } else {
        playSong(marioMelody, sizeof(marioMelody) / sizeof(marioMelody[0]));
      }
    } else {
      // Play a short low pitch tone when stopping a song manually
      tone(buzzerPin, 400);
      delay(60);
      noTone(buzzerPin);
      delay(300);
    }
  }
}

void drawMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  
  if (currentMenuSelection == 0) {
    lcd.print(isPlaying ? "Playing:Tetris  " : "Select: Tetris  ");
  } else {
    lcd.print(isPlaying ? "Playing: Mario  " : "Select: Mario   ");
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
