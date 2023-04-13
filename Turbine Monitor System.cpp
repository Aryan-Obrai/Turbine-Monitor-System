#include <SPI.h>
#include <font.h>
#include <stdlib.h>

#define RST A5
#define CE A4
#define DC  2
#define DIN 3 
#define CLK 4 
#define echoPin 8
#define trigPin 10
#define ENA 9
#define INA 6
#define INB 7
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to

//int HUM_C = 40;
int red = 12;
int green = 13;
int speed = 0;
int temp = 130;
int hcalc = 0;
int begintime = 0.0;
int endtime = 0.0;
int h = 50;
int l = 50;
int j;
int DCL = 0;
int DCH = 0;
int DCRL = 0;
int DCRH = 0;
int dcounter = 0;
int counter = 30;
int lightcounter = 0;
double distance = 0;
int duration = 0;
int actual_duration = 0;
bool OBJECT = false;
bool WAIT = false;
bool UP = false;
bool DOWN = false;
bool RIGHT = false;
bool BTN = false;
bool BTNFLAG = false;
bool SPINRIGHT = false;
bool SPINLEFT = false;
bool LEFT = false;
bool RED = false;
bool POWER = false;
bool GREEN = false;
bool LCDRIGHTFLAG;
bool LCDLEFTFLAG;
bool UPFLAG;
bool DOWNFLAG;
int JS_X = A2;
int JS_Y = A1;
int JS_BTN = 11;
int j_x ;
int j_y;
int j_btn;
char dis[8];
char spd[8];
typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);

} task;

const unsigned short tasksNum = 6;
task tasks[tasksNum];

enum SM1_States { SM1_INIT, CHECK, LIGHTS, LIGHTS_OFF};
int SM1_Tick(int state1) {
  switch (state1) { // State transitions
    case SM1_INIT:
    if(BTN == 0) {
      state1 = CHECK;
    }
    else {
      state1 = LIGHTS_OFF;
      j = 0;
    }
      break;
    case CHECK:
      if(BTN == 0) {
      state1 = LIGHTS;
    }
    else {
      state1 = LIGHTS_OFF;
    }
      break;
    case LIGHTS:
      if(BTN == 0) {
      state1 = CHECK;
    }
    else {
      state1 = LIGHTS_OFF;
    }
      break;
    case LIGHTS_OFF:
      if(BTN == 0) {
      state1 = CHECK;
    }
    else {
      state1 = LIGHTS_OFF;
    }
      break;
  }
   switch (state1) { // State actions
    case SM1_INIT:
      break;
    case CHECK:
      // Clears the trigPin condition
      actual_duration = Pulse();
      distance = actual_duration *(0.034 / 2);
      if(distance < 15) {
        OBJECT = true;
      }
      else {
        OBJECT = false;
      }
      break;
    case LIGHTS:
      if(OBJECT) {
        RED = true;
        GREEN = false;
      }
      else {
        RED = false;
        GREEN = true;
      }
      break;
    case LIGHTS_OFF:
    RED = true;
    GREEN = false;
    distance = 0;
    
   }
  return state1;
}

enum SM2_States {SM2_INIT, HIGHG, LOWG, REDG, OFFG};
int SM2_Tick(int state2) {
  switch (state2) { // State transitions
    case SM2_INIT:
    lightcounter = 0;
    l = 10;
    h = 50;
    counter = 110;
    if(RED == true) {
    state2 = HIGHG;
    }
    else {
      state2 = REDG;
    }
    break;
    case HIGHG:
    if(RED == true) {
    if(lightcounter <= h) {
      state2 = HIGHG;
    }
    else {
      state2 = LOWG;
      lightcounter = 0;
      hcalc = h + 5;
      if(hcalc > 0) {
      h = h - 5;
      }
      else {
        h = 0;
      }
    }
    }
    else {
      state2 = REDG;
    }
    break;
    case LOWG:
    if(RED == true) {
    if(lightcounter <= l) {
      state2 = LOWG;
    }
    else {
      state2 = HIGHG;
      lightcounter = 0;
      l = l + 15;
      if(l >= counter) {
        state2 = REDG;
      }
    }
    }
    else {
      state2 = REDG;
    }
    break;
    case REDG:
    if(l >= counter) {
      state2 = REDG;
    }
    else if(RED == true) {
      state2 = REDG;
    }
    else if(GREEN == true) {
      state2 = OFFG;
    }
    else {
      state2 = HIGHG;
    }
    break;
    case OFFG:
    if(RED == false) {
      state2 = OFFG;
    }
    else {
      state2 = HIGHG;
    }
    break;
  }

  switch (state2) { // State actions
    case SM2_INIT:
    break;
    case HIGHG:
    digitalWrite(green,HIGH);
    digitalWrite(red,HIGH);
    lightcounter++;
    //Serial.println("HIGHG");
    //Serial.println(h);
    break;
    case LOWG:
    digitalWrite(green,LOW);
    digitalWrite(red,HIGH);
    lightcounter++;
    //Serial.println("LOWG");
    //Serial.println(l);
    if(l >= 100) {
      j = 0;
    }
    break;
    case REDG:
    digitalWrite(red,HIGH);
    digitalWrite(green,LOW);
    POWER = false;
    //Serial.println("Power is off");
    l = 10;
    h = 50;
    break;
    case OFFG:
    digitalWrite(red,LOW);
    digitalWrite(green,HIGH);
    POWER = true;
    //Serial.println("Power is on");
  }
   
  return state2;
}

enum SM9_States {SM9_INIT, TURNS};
int SM9_Tick(int state9) {
  switch (state9) { // State transitions
   case SM9_INIT:
    state9 = TURNS;
    break;
   case TURNS:
    state9 = TURNS;
    break;
  }
  switch (state9) {
    case SM9_INIT:
    break;
    case TURNS:
      if(RIGHT) {
      LCDRIGHTFLAG = true;
      }
      if(LCDRIGHTFLAG) {
        if(RIGHT == false) {
          SPINLEFT = false;
          SPINRIGHT = true;
          NokiaClear();
          setCursor(30, 26);
          NokiaString("---->");
          LCDRIGHTFLAG = false;
          //setCursor(0, 0);
        }
      }
      if(LEFT) {
      LCDLEFTFLAG = true;
      }
      if(LCDLEFTFLAG) {
        if(LEFT == false) {
          SPINLEFT = true;
          SPINRIGHT = false;
          NokiaClear();
          setCursor(30, 26);
          NokiaString("<----");
          //Serial.println("<-----");
          LCDLEFTFLAG = false;
          //setCursor(0, 0);
        }
      }

      if(UP) {
      UPFLAG = true;
      }
      if(UPFLAG) {
        if(UP == false) {
          j = 255;
          UPFLAG = false;
          //setCursor(0, 0);
        }
      }

      if(DOWN) {
      DOWNFLAG = true;
      }
      if(DOWNFLAG) {
        if(DOWN == false) {
          j = 55;
          DOWNFLAG = false;
          //setCursor(0, 0);
        }
      }
     
      if(BTN == 0) {
      setCursor(0,28);
      NokiaString("Distance: ");
      NokiaString(dtostrf(distance,3,2,dis));
      setCursor(0,0);
      NokiaString("Speed: ");
      NokiaString(dtostrf(j,3,2,spd));
      }
      else {
        NokiaClear();
      }
      //Serial.println(state9);
    break;
  }
  return state9;
}

enum DC_SPINLEFT{SM5_INIT, DCO, DCOF, DCOFF};
int DC_Tick(int state5) {
  switch(state5) {
    case SM5_INIT:
    if(SPINLEFT == true) {
    dcounter = 0;
    DCH = j;
    DCL = 255 - j;
    }
    else {
      state5 = DCOFF;
    }
    break;
    case DCO:
    if(dcounter < DCH) {
      state5 = DCO;
    }
    else {
    state5 = DCOF;
    dcounter = 0;
    }
    break;
    case DCOF:
    if(dcounter < DCL) {
      state5 = DCOF;
    }
    else {
    state5 = DCO;
    dcounter = 0;
    }
    break;
    case DCOFF:
    if(SPINLEFT == true) {
    dcounter = 0;
    DCH = j;
    DCL = 255 - j;
    }
    else {
      state5 = DCOFF;
    }
    break;
  }
  switch(state5) {
    case DCO:
    digitalWrite(INA,LOW);
    digitalWrite(INB,HIGH);
    digitalWrite(ENA,HIGH);
    break;
    case DCOF:
    digitalWrite(INA,LOW);
    digitalWrite(INB,HIGH);
    digitalWrite(ENA,LOW);
    break;
    case DCOFF:
    break;
  }
}

enum DC_SPINRIGHT{SM7_INIT, DCRO, DCORF, DCORFF};
int DCR_Tick(int state7) {
  switch(state7) {
    case SM7_INIT:
    if(SPINRIGHT == true) {
    dcounter = 0;
    DCRH = j;
    DCRL = 255 - j;
    }
    else {
      state7 = DCORFF;
    }
    break;
    case DCRO:
    if(dcounter < DCRH) {
      state7 = DCRO;
    }
    else {
    state7 = DCORF;
    dcounter = 0;
    }
    break;
    case DCORF:
    if(dcounter < DCRL) {
      state7 = DCORF;
    }
    else {
    state7 = DCRO;
    dcounter = 0;
    }
    break;
    case DCORFF:
    if(SPINRIGHT == true) {
    dcounter = 0;
    DCRH = j;
    DCRL = 255 - j;
    }
    else {
      state7 = DCORFF;
    }
    break;
  }
  switch(state7) {
    case DCRO:
    digitalWrite(INA,HIGH);
    digitalWrite(INB,LOW);
    digitalWrite(ENA,HIGH);
    break;
    case DCORF:
    digitalWrite(INA,HIGH);
    digitalWrite(INB,LOW);
    digitalWrite(ENA,LOW);
    break;
    case DCORFF:
    break;
  }
}

enum SM6_States { SM6_INIT, SAMPLE, ADJUST};
int SM6_Tick(int state6) {
  switch (state6) {
    case SM6_INIT:
      state6 = SAMPLE;
      break;
    case SAMPLE:
      state6 = ADJUST;
      break;
    case ADJUST:
      state6 = SAMPLE;
      break;
  }
  switch (state6) {
    case SM6_INIT:
      break;
    case SAMPLE:
      for(int g = 0; g < 60; g++) {
        WAIT = true;
      }
       WAIT = false;
      j_x = analogRead(JS_X);
      j_y = analogRead(JS_Y);
      j_btn = digitalRead(JS_BTN);
      if(j_y < 100) {
        UP = true;
        //Serial.println("UP");
      }
      
      else {
        UP = false;
      }
      if(j_y > 900) {
        DOWN = true;
        //Serial.println("DOWN");
      }
      else {
        DOWN = false;
      }
      if(j_x < 100) {
        RIGHT = true;
        SPINRIGHT = true;
        SPINLEFT = false;
        //Serial.println("RIGHT");
      }
      else {
        RIGHT = false;
        //Serial.println("NOT RIGHT");
      }
      if(j_x > 900) {
        LEFT = true;
        SPINRIGHT = false;
        SPINLEFT = true;
        //Serial.println("LEFT");
      }
      else {
        LEFT = false;
        //Serial.println("NOT LEFT");
      }
      if(j_btn == 0) {
        BTNFLAG = true;
      }
      if(BTNFLAG) {
        if(j_btn == 1) {
          BTN = !BTN;
          BTNFLAG = false;
          if(j > 0) {
            j = 0;
          }
        }
      }

     // Serial.print("System is "); Serial.println(BTN);

      break;
  }
  return state6;
}


int Pulse() {
  digitalWrite(trigPin,LOW);
  digitalWrite(trigPin,HIGH);
  digitalWrite(trigPin,LOW);
  while(digitalRead(echoPin) == LOW) {
    WAIT = true;
  }
  if(digitalRead(echoPin) == HIGH) {
    begintime = micros();
  }
  while(digitalRead(echoPin) == HIGH) {
    WAIT = true;
  }
  if(digitalRead(echoPin) == LOW) {
    endtime = micros();
  }
  duration = endtime - begintime;
  return duration;
}

void Nokia_Init() {
  digitalWrite(RST,LOW);
  digitalWrite(RST,HIGH);
}

void myshiftOut(int datapin, int clockpin, int bitorder, int val) {
  for(int i = 0; i < 8; i++) {
    if(bitorder == LSBFIRST) {
      digitalWrite(DIN, !!(val & (1 << i)));
    }
    else {
      digitalWrite(DIN, !!(val & (1 << (7 - i))));
    }
      digitalWrite(CLK, HIGH);
      digitalWrite(CLK, LOW);
  }
}

void NokiaCommand(byte c) {
  //digitalWrite(CE,HIGH);
  digitalWrite(CE,LOW);
  digitalWrite(CLK,LOW); 
  digitalWrite(DC,LOW);
  myshiftOut(DIN,CLK,MSBFIRST,c); 
  digitalWrite(CE,HIGH);
}

void NokiaData(byte d) {
  //digitalWrite(CE,HIGH);
  digitalWrite(CE,LOW);
  digitalWrite(CLK,LOW); 
  digitalWrite(DC,HIGH);
  myshiftOut(DIN,CLK,MSBFIRST,d); 
  digitalWrite(CE,HIGH);
}

void NokiaChar(char ch) {
  for(int i = 0; i < 5; i++) {
    NokiaData(ASCII[ch - 0x20][i]);
    //NokiaData(0x00);
  }
}

void NokiaClear() {
  for(int i = 0; i < 504; i++) {
    NokiaData(0x00);
  }
}

void NokiaString(char *string) {
  while(*string) {
    NokiaChar(*string++);
  }
} 

void setCursor(int x, int y) {
  NokiaCommand(0x80 | x); 
  NokiaCommand(0x40 | y);   
} 

void setup() {
  // put your setup code here, to run once:
  unsigned char i = 0;
  tasks[i].state = SM1_INIT;
  tasks[i].period = 10;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM1_Tick;
  i++;
  tasks[i].state = SM6_INIT;
  tasks[i].period = 10;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM6_Tick;  
  i++;
  tasks[i].state = SM2_INIT;
  tasks[i].period = 10;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM2_Tick; 
  i++;
  tasks[i].state = SM9_INIT;
  tasks[i].period = 10;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM9_Tick; 
  i++;
  tasks[i].state = DCO;
  tasks[i].period = 10;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &DC_Tick; 
  i++;
  tasks[i].state = DCRO;
  tasks[i].period = 10;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &DCR_Tick; 


  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ENA,OUTPUT);
  pinMode(INA,OUTPUT);
  pinMode(INB,OUTPUT);
  pinMode(RST,OUTPUT);
  pinMode(CE,OUTPUT);
  pinMode(DC,OUTPUT);
  pinMode(DIN,OUTPUT);
  pinMode(CLK,OUTPUT);
  pinMode(JS_BTN, INPUT_PULLUP);
  //digitalWrite(CE,HIGH);
  Nokia_Init();

  NokiaCommand(0x21);
  NokiaCommand(0xC3);
  NokiaCommand(0x04);
  NokiaCommand(0x14);
  NokiaCommand(0x0C);
  NokiaCommand(0x20);
  NokiaCommand(0x0C);


  // set up the LCD's number of columns and rows:
 /* lcd.begin(16, 2); */
  // Print a message to the LCD.
  Serial.begin(9600);
}

void loop() {
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
    if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
      tasks[i].state = tasks[i].TickFct(tasks[i].state);
      tasks[i].elapsedTime = millis(); // Last time this task was ran
    }
  }
  
}