/*Snake Game with lcd and joystick and Arduino mega By Aryan Chavan. But this code can work with other arduino boards eg: uno, nano.
  #By Aryan Chavan
  This game is like the snake game in the old phones.
# COMPONENTS NEEDED TO BUILD
  1x arduino
  jumper wires
  1x breadboard  (for compact build)  // but would advise using 2 breadboards
  1x Joystick    (from the arduino kit will work)
  1x 1602 LCD    (i used the non i2c 1602 LCD)
  2x 10k Potentiometers
  1x led         (i dint hook the led to arduino but you can if wanted)
  1x 220 ohm resistor (for the led)
  1x pushbutton   (you can add more for other things)
  2x 10k ohm resistor (for the pushbutton)

# Some inportant notes
  press the pushbutton to change the speed.
  you will only be able to change speed while holding the button down and changing the speed with the potentiometer at the same time.
  the game will only start after the speed is set.
  it takes about 15 seconds to set the speed
  as per the counter and delay the you will have 30 chances to set the speed and you will have to wait till the counter goes to 30. 
  after the counter reaches 30 the game will wait fro 3 secs and then it will start.
*/
#include <LiquidCrystal.h>

//PINS
#define       JOYSTICK_X_PIN                 A0
#define       JOYSTICK_Y_PIN                 A1
#define       JOYSTICK_Z_PIN                 8

#define       NUM_KEYS                       5
#define       JOYSTICK_RELEASED              0
#define       JOYSTICK_UP_                   1
#define       JOYSTICK_DOWN_                 2
#define       JOYSTICK_LEFT_                 3
#define       JOYSTICK_RIGHT_                4
#define       ROWS                           2  // number of indicator rows
#define       levels                         4  // difficulty levels
#define       MAX_SNAKE_LENGTH              25  // maximum length of snake




LiquidCrystal lcd(6, 7, 5, 4, 3, 2);    // lcd pins assigned    // you can change the lcd pins to be however you have them hooked up

unsigned long time, timeNow;

int gameSpeed;

int pcont = A3;
int mcr = 10;

boolean skip, gameOver, gameStarted;

int olddir;
int selectedLevel;

byte key = 0;

byte oldkey = 0;

byte Field[8 * ROWS * 16];

struct partdef  {
  int row, column, dir;
  struct partdef *next;
};

typedef partdef part;

part *head, *tail;

int collected;

unsigned int pc, pr;


//----------------------------------------------

// credits
void credits() {
  for (int i = 0; i <= 1; i++) {
    lcd.clear();
    lcd.print("Welcome to the");
    lcd.setCursor(0, 2);
    lcd.print("Snake Game");
    delay(2000);
    lcd.clear();
    lcd.print("By Aryan Chavan");
    delay(2000);
    break;
  }

}


// display on the screen

void drawMatrix() {
  boolean levelz[levels][4][16] = {

    { {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},

      {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},

      {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},

      {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}
    },


    { {true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true},

      {true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true},

      {true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true},

      {true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true}
    },


    { {true, false, false, false, true, false, false, false, false, false, false, true, false, false, false, true},

      {true, false, false, false, false, false, false, false, true, false, false, false, false, false, false, true},

      {true, false, false, false, true, false, false, false, false, false, false, true, false, false, false, true},

      {true, false, false, false, false, false, false, false, true, false, false, false, false, false, false, true}
    },


    { {true, false, true, false, false, false, false, false, false, true, false, false, false, true, false, false},

      {false, false, false, false, true, false, false, true, false, false, false, true, false, false, false, true},

      {true, false, true, false, false, false, false, false, false, true, false, false, false, true, false, false},

      {false, false, false, false, true, false, false, true, false, false, false, true, false, false, false, true}
    }

  };




  byte myChar[8];
  boolean special;
  int cc = 0;

  if (!gameOver)  {
    ChangeDot(pr, pc, true);

    for (int r = 0; r < ROWS; r++) {
      for (int c = 0; c < 16; c++) {
        special = false;
        for (int i = 0; i < 8; i++)  {
          byte b = B00000;

          if ((Field[16 * (r * 8 + i) + (c * 5 + 0) / 5] & (1 << ((c * 5 + 0) % 5))) >> ((c * 5 + 0) % 5)) {
            b += B10000;
            special = true;
          }

          if ((Field[16 * (r * 8 + i) + (c * 5 + 1) / 5] & (1 << ((c * 5 + 1) % 5))) >> ((c * 5 + 1) % 5)) {
            b += B01000;
            special = true;
          }

          if ((Field[16 * (r * 8 + i) + (c * 5 + 2) / 5] & (1 << ((c * 5 + 2) % 5))) >> ((c * 5 + 2) % 5)) {
            b += B00100;
            special = true;
          }

          if ((Field[16 * (r * 8 + i) + (c * 5 + 3) / 5] & (1 << ((c * 5 + 3) % 5))) >> ((c * 5 + 3) % 5)) {
            b += B00010;
            special = true;
          }

          if ((Field[16 * (r * 8 + i) + (c * 5 + 4) / 5] & (1 << ((c * 5 + 4) % 5))) >> ((c * 5 + 4) % 5)) {
            b += B00001;
            special = true;
          }

          myChar[i] = b;
        }

        if (special)  {
          lcd.createChar(cc, myChar);
          lcd.setCursor(c, r);
          lcd.write(byte(cc));

          cc++;

        }

        else  {

          lcd.setCursor(c, r);

          if (levelz[selectedLevel][r][c]) lcd.write(255);

          else lcd.write(128);

        }

      }

    }

  }

}

//----------------------------------

void freeList()

{

  part *p, *q;

  p = tail;

  while (p != NULL)

  {

    q = p;

    p = p->next;

    free(q);

  }

  head = tail = NULL;

}

//----------------------------------

void gameOverFunction()

{

  delay(1000);

  lcd.clear();

  freeList();

  lcd.setCursor(3, 0);

  lcd.print("Game Over!");

  lcd.setCursor(4, 1);

  lcd.print("Score: ");

  lcd.print(collected);

  delay(3000);

}

//----------------------------------

void growSnake()

{

  part *p;

  p = (part*)malloc(sizeof(part));

  p->row = tail->row;

  p->column = tail->column;

  p->dir = tail->dir;

  p->next = tail;

  tail = p;

}

//----------------------------------

// a new dot on the screen

void newPoint()

{

  boolean levelz[levels][4][16] = {

    { {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},

      {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},

      {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},

      {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}
    },


    { {true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true},

      {true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true},

      {true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true},

      {true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true}
    },


    { {true, false, false, false, true, false, false, false, false, false, false, true, false, false, false, true},

      {true, false, false, false, false, false, false, false, true, false, false, false, false, false, false, true},

      {true, false, false, false, true, false, false, false, false, false, false, true, false, false, false, true},

      {true, false, false, false, false, false, false, false, true, false, false, false, false, false, false, true}
    },


    { {true, false, true, false, false, false, false, false, false, true, false, false, false, true, false, false},

      {false, false, false, false, true, false, false, true, false, false, false, true, false, false, false, true},

      {true, false, true, false, false, false, false, false, false, true, false, false, false, true, false, false},

      {false, false, false, false, true, false, false, true, false, false, false, true, false, false, false, true}
    }

  };




  part *p;

  p = tail;

  boolean newp = true;

  while (newp)

  {

    pr = random(8 * ROWS);

    pc = random(80);

    newp = false;

    if (levelz[selectedLevel][pr / 8][pc / 5]) newp = true;

    while (p->next != NULL && !newp)

    {

      if (p->row == pr && p->column == pc) newp = true;

      p = p->next;

    }

  }



  if (collected < MAX_SNAKE_LENGTH && gameStarted) growSnake();

}

//----------------------------------


// head movement

void moveHead()

{

  boolean levelz[levels][4][16] = {

    { {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},

      {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},

      {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},

      {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}
    },


    { {true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true},

      {true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true},

      {true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true},

      {true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true}
    },


    { {true, false, false, false, true, false, false, false, false, false, false, true, false, false, false, true},

      {true, false, false, false, false, false, false, false, true, false, false, false, false, false, false, true},

      {true, false, false, false, true, false, false, false, false, false, false, true, false, false, false, true},

      {true, false, false, false, false, false, false, false, true, false, false, false, false, false, false, true}
    },


    { {true, false, true, false, false, false, false, false, false, true, false, false, false, true, false, false},

      {false, false, false, false, true, false, false, true, false, false, false, true, false, false, false, true},

      {true, false, true, false, false, false, false, false, false, true, false, false, false, true, false, false},

      {false, false, false, false, true, false, false, true, false, false, false, true, false, false, false, true}
    }

  };





  switch (head->dir) // 1 step in direction

  {

    case 0: head->row--; break;

    case 1: head->row++; break;

    case 2: head->column++; break;

    case 3: head->column--; break;

    default : break;

  }

  if (head->column >= 80) head->column = 0;

  if (head->column < 0) head->column = 79;

  if (head->row >= (8 * ROWS)) head->row = 0;

  if (head->row < 0) head->row = (8 * ROWS - 1);



  if (levelz[selectedLevel][head->row / 8][head->column / 5]) gameOver = true; // wall collision check



  part *p;

  p = tail;

  while (p != head && !gameOver) // self collision

  {

    if (p->row == head->row && p->column == head->column) gameOver = true;

    p = p->next;

  }

  if (gameOver)

    gameOverFunction();

  else

  {

    ChangeDot(head->row, head->column, true);



    if (head->row == pr && head->column == pc) // point pickup check

    {

      collected++;

      if (gameSpeed < 25) gameSpeed += 3;

      newPoint();

    }

  }

}

//----------------------------------

// funny move

void moveAll()

{

  part *p;

  p = tail;


  ChangeDot(p->row, p->column, false);



  while (p->next != NULL)

  {

    p->row = p->next->row;

    p->column = p->next->column;

    p->dir = p->next->dir;

    p = p->next;

  }

  moveHead();

}

//----------------------------------


// create a snake

void createSnake(int n) // n = size of snake

{

  for (unsigned int i = 0; i < (8 * ROWS * 16); i++)

    Field[i] = 0;



  part *p, *q;

  tail = (part*)malloc(sizeof(part));

  tail->row = 7;

  tail->column = 39 + n / 2;

  tail->dir = 3;

  q = tail;



  ChangeDot(tail->row, tail->column, true);



  for (int i = 0; i < n - 1; i++) // build snake from tail to head

  {

    p = (part*)malloc(sizeof(part));

    p->row = q->row;

    p->column = q->column - 1; //initial snake id placed horizoltally



    ChangeDot(p->row, p->column, true);



    p->dir = q->dir;

    q->next = p;

    q = p;

  }

  if (n > 1)

  {

    p->next = NULL;

    head  = p;

  }

  else

  {

    tail->next = NULL;

    head = tail;

  }

}


void startF()

{

  byte mySnake[8][8] =

  {

    { B00000,

      B00000,

      B00011,

      B00110,

      B01100,

      B11000,

      B00000,

    },

    { B00000,

      B11000,

      B11110,

      B00011,

      B00001,

      B00000,

      B00000,

    },

    { B00000,

      B00000,

      B00000,

      B00000,

      B00000,

      B11111,

      B01110,

    },

    { B00000,

      B00000,

      B00011,

      B01111,

      B11000,

      B00000,

      B00000,

    },

    { B00000,

      B11100,

      B11111,

      B00001,

      B00000,

      B00000,

      B00000,

    },

    { B00000,

      B00000,

      B00000,

      B11000,

      B01101,

      B00111,

      B00000,

    },

    { B00000,

      B00000,

      B01110,

      B11011,

      B11111,

      B01110,

      B00000,

    },

    { B00000,

      B00000,

      B00000,

      B01000,

      B10000,

      B01000,

      B00000,

    }

  };



  gameOver = false;

  gameStarted = false;

  selectedLevel = 1;



  lcd.clear();

  lcd.setCursor(0, 0);

  lcd.print("Select level: ");
  lcd.print(selectedLevel);

  for (int i = 0; i < 8; i++)

  {

    lcd.createChar(i, mySnake[i]);

    lcd.setCursor(i + 4, 1);

    lcd.write(byte(i));

  }




  collected = 0;

  gameSpeed = 0;       // increases the speed of the snake in the game  // the game speed right now is 100 but when you start at first make the speed to 10 or 20 and it can up to 1000 // but would advise playing gthe game in slower speed as the speed of the snake gradually increases.
  createSnake(5);         // number of body parts the snake starts with

  time = 0;

  int a = 0;
  do {                    // takes input from the potentiometer and sets the value to be the gamespeed.
    if (digitalRead(mcr) == LOW) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Select level: ");
      lcd.print(selectedLevel);
      
      int readp = analogRead(pcont);
      Serial.print("Pcont = ");
      Serial.println(readp);
      gameSpeed = analogRead(pcont);
      lcd.setCursor(0, 2);
      lcd.print("GS = ");
      lcd.print(gameSpeed);
      a++;
      Serial.print("a val = ");
      lcd.setCursor(10, 2);
      lcd.print("a = ");
      Serial.println(a);
      lcd.print(a);
      delay(500);
    }
  } while (a <= 30);

  delay(3000);    // delay before the game starts after setting the speed




}

//----------------------------------

void setup()  {
  Serial.begin(9600);
  digitalWrite(JOYSTICK_Z_PIN, HIGH);
  lcd.begin(16, ROWS);
  lcd.clear();
  credits();
  startF();
}


//----------------------------------

void loop() {



  if (!gameOver && !gameStarted)  // if the game does not start

  {

    key = ScanJoyStick(); // check the joystick

    if (key != oldkey)

    {

      delay(50);

      key = ScanJoyStick();

      if (key != oldkey)

      {

        oldkey = key;

        if (key > 0)

        {

          olddir = head->dir;

          if (key == JOYSTICK_UP_ && selectedLevel < levels) selectedLevel++; // increase the selectable level

          else if (key == JOYSTICK_DOWN_ && selectedLevel > 1) selectedLevel--; // decrease the selectable level

        }

      }

    }


    else if (digitalRead(JOYSTICK_Z_PIN))// if the joystick button is pressed, the beginning of the game

    {

      lcd.clear();

      selectedLevel--;

      newPoint();

      gameStarted = true;

    }

    else  // otherwise continue the display of the level of the game

    {

      lcd.setCursor(14, 0);

      lcd.print(selectedLevel);

    }

  }



  else if (!gameOver && gameStarted)  // if there is a game

  {

    skip = false;



    key = ScanJoyStick(); // check the joystick

    if (key != oldkey)

    {

      delay(50);

      key = ScanJoyStick();

      if (key != oldkey)

      {

        oldkey = key;

        if (key > 0)  { // select the direction of movement of the snake depending on the position of the joystick

          olddir = head->dir;

          if (key == JOYSTICK_RIGHT_ && head->dir != 3) head->dir = 2;

          else if (key == JOYSTICK_UP_    && head->dir != 1) head->dir = 0;

          else if (key == JOYSTICK_DOWN_  && head->dir != 0) head->dir = 1;

          else if (key == JOYSTICK_LEFT_  && head->dir != 2) head->dir = 3;

          if (olddir != head->dir)

          {

            skip = true;

            delay(1000 / gameSpeed);

            moveAll();

            drawMatrix();

          }

        }

      }

    }

    if (!skip)

    {

      timeNow = millis();

      if (timeNow - time > 1000 / gameSpeed)

      {

        moveAll();

        drawMatrix();

        time = millis();

      }

    }

  }



  if (gameOver) // if the game is over

  {

    key = ScanJoyStick();// check the joystick

    if (key != oldkey)

    {

      delay(50  );

      key = ScanJoyStick();

      if (key != oldkey)

      {

        oldkey = key;

        if (key > 0)// start the game when you move the joystick in any direction

        {

          startF();

        }

      }

    }



  }

}

//-----------------------------------

// check the joystick

byte ScanJoyStick()

{

  byte Direction = JOYSTICK_RELEASED;

  unsigned int X_Val = analogRead(JOYSTICK_X_PIN);

  unsigned int Y_Val = analogRead(JOYSTICK_Y_PIN);



  if ((Y_Val >= 900) && (X_Val >= 400))

  {

    Direction = JOYSTICK_DOWN_;

  }

  else if ((Y_Val <= 400) && (X_Val >= 400))

  {

    Direction = JOYSTICK_UP_;

  }

  else if ((Y_Val >= 400) && (X_Val >= 900))

  {

    Direction = JOYSTICK_RIGHT_;

  }

  else if ((Y_Val >= 400) && (X_Val <= 400))

  {

    Direction = JOYSTICK_LEFT_;

  }



  return Direction;

}

//-----------------------------------------------------

// making changes to matrix of points on eran

void ChangeDot(unsigned int RowVal, unsigned int ColVal, boolean NewVal)

{

  Field[16 * RowVal + ColVal / 5] &= ~(1 << (ColVal % 5));


  if (NewVal)

    Field[16 * RowVal + ColVal / 5] |= 1 << (ColVal % 5);

}
