#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define I2C_SLAVE_ADDRESS 0x22

LiquidCrystal_I2C lcd(0x22,16,2);


byte karo[] = {
  B00000,
  B00000,
  B00100,
  B01110,
  B11111,
  B01110,
  B00100,
  B00000
};

byte skocko[] = {
  B11011,
  B10001,
  B01010,
  B00000,
  B11111,
  B01110,
  B00100,
  B00000
};

byte zvezda[] = {
  B00000,
  B00100,
  B10101,
  B01010,
  B00100,
  B01010,
  B10001,
  B00000
};

byte herc[] = {
  B00000,
  B00000,
  B01010,
  B10101,
  B01010,
  B00100,
  B00000,
  B00000
};

byte spades[] = {
  B00000,
  B00100,
  B01110,
  B11111,
  B11111,
  B00100,
  B00100,
  B00000
};

byte tref[]= {
  B00000,
  B01110,
  B01110,
  B11111,
  B11111,
  B11111,
  B00100,
  B00000
};

int recievedCombination[4];
int inputCombination[4];
boolean gameWon = false;
int dat = 0;
int correctPos = 0;
int correctSimbol = 0;
int anotherOne = 0;
boolean heSent = false;

void setup()
{
  	Wire.begin(I2C_SLAVE_ADDRESS);
  	Serial.begin(9600);
  
  	Wire.onReceive(receiveEvent);
  	
  	delay(1000);

  	lcd.init();
  	lcd.backlight();
   	
  	lcd.createChar(1, skocko);
  	lcd.createChar(2, karo);
  	lcd.createChar(3, zvezda);
  	lcd.createChar(4, herc);
  	lcd.createChar(5, spades);
  	lcd.createChar(6, tref);

}


void loop()
{
  while(!gameWon){
    lcd.clear();
  	lcd.setCursor(0,0);
    lcd.print("Type combination");
    lcd.setCursor(0,1);
    
    lcd.print("1");
    lcd.write(1);
    lcd.print("2");
    lcd.write(2);
    lcd.print("3");
    lcd.write(3);
    lcd.print("4");
    lcd.write(4);
    lcd.print("5");
    lcd.write(5);
    lcd.print("6");
    lcd.write(6);
    
    gameLogic();
   }
  	lcd.setCursor(0,0);
  	lcd.print("New game?");
  	
  	lcd.setCursor(0,1);
  	lcd.print("1 y, 0 n");
  
  	anotherOneFunc();
    delay(2000);
}

void receiveEvent(int bytes) {
  for(int i = 0; i < 4; i ++) {
      recievedCombination[i] = Wire.read();
   	  Serial.print(recievedCombination[i]);
    }
}

void gameLogic(){
  	Serial.println("ENTER");
  	delay(3000);
  
    if(Serial.available() >= 3) {

    	for(int i = 0; i < 4; i ++) {

       		inputCombination[i] = Serial.read()-48;
          	Serial.print(inputCombination[i]);
    	}
     Serial.flush();
	}
  delay(2000);
  correctPos = 0;
  correctSimbol = 0;
  for(int i = 0; i < 4; i++){
    
    if(inputCombination[i] == recievedCombination[i]){
    	correctPos++;
    }
    for(int j = 0; j < 4; j++){
      if(inputCombination[i] == recievedCombination[j]){
      	correctSimbol++;
        break;
      }
    }
  }
    if(correctPos == 4){
      	lcd.clear();
    	lcd.setCursor(3,0);
      	lcd.print("Game won!!");
      	gameWon = true;
      	delay(1500);
      	lcd.clear();
    } else{
 		lcd.clear();
        lcd.setCursor(2,0);
    	lcd.print("Game failed!");
      	delay(1000);
      
      	lcd.setCursor(0,0);
      	lcd.print("You guessed ");
      	lcd.print(correctSimbol);
      
      	lcd.setCursor(0,1);
      	lcd.print("In right pos ");
      	lcd.print(correctPos);
      
      	delay(2000);
    }
}

void anotherOneFunc(){
Serial.println("ENTER");
  	delay(3000);
  
    if(Serial.available() > 0) {
		anotherOne = Serial.read()-48;
      	Serial.println(anotherOne);
    }
  	
  	delay(1000);
  
  if(anotherOne == 1){
  	anotherOne = 0;
    heSent = true;
  	Wire.onRequest(handler);
    delay(5000);
    gameWon = false;
  } else if (anotherOne == 0){
  	lcd.clear();
    while(1){
    lcd.setCursor(0,0);
    lcd.print("Game done..");
    }
  }

}
void handler(){
  if(gameWon && heSent){
	Wire.write(1);
    heSent = false;
  } else{
  	Wire.write(0);
  }
}
