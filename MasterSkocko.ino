#include <Wire.h> //for i2c communication
#include <LiquidCrystal_I2C.h> //for lcd display

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

int gameCombination[4];
int read;

#define LCD_I2C_SLAVE 0x22

LiquidCrystal_I2C lcd(0x20,16,2); 

void setup()
{
	Wire.begin();
  	Serial.begin(9600);
  
  	randomSeed(analogRead(0));
  	
  	lcd.init();
    lcd.backlight();
  
  	//Skocko character initialization
  	
    lcd.createChar(1, skocko);
  	lcd.createChar(2, karo);
  	lcd.createChar(3, zvezda);
  	lcd.createChar(4, herc);
  	lcd.createChar(5, spades);
  	lcd.createChar(6, tref);
  	//Generate combination
  	for(int i = 0; i < 4; i++){
    	gameCombination[i] = random(1,7);
    	Serial.print(gameCombination[i]);
  	}
  	lcd.setCursor(2,0);
  	lcd.print("Combination: ");
  
 	lcd.setCursor(6,1);
  	printChar();
  
  	sendCombinationToSlave();
  	
}

void loop(){
  Wire.requestFrom(LCD_I2C_SLAVE, 1);
  
  while (Wire.available()){
  	read = Wire.read();
    Serial.print(read);
  }
  delay(1000);
    
  if(read == 1){
      	read = 0;
  	  	for(int i = 0; i < 4; i++){
    	gameCombination[i] = random(1,7);
    	Serial.print(gameCombination[i]);
  	}
    printChar();
    delay(1000);
    sendCombinationToSlave();
  }
    delay(4000);

}

void printChar(){
 	lcd.setCursor(6,1);
	for(int i = 0; i < 4; i++){
   	 switch(gameCombination[i]){
     	case 1:
      		lcd.write(1);
     	 	break;
      	case 2:
      		lcd.write(2);
      		break;
      	case 3:
      		lcd.write(3);
      		break;
      	case 4:
      		lcd.write(4);
      		break;
      	case 5:
      		lcd.write(5);
      		break;
      	case 6:
      		lcd.write(6);
      		break;
    }
  }
}
  void sendCombinationToSlave(){
  	Wire.beginTransmission(LCD_I2C_SLAVE);
    for(int i = 0; i < 4; i++){
    	Wire.write(gameCombination[i]);
    }
    delay(200);
    Wire.endTransmission(); 
  }