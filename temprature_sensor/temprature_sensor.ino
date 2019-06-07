#include<LiquidCrystal.h> //include LCD library to work with LCD display
LiquidCrystal lcd(8, 9, 10, 11, 12, 13); //LCD connected to pins RS-En, E-Rs, 10-D4, 11-D5, 12-D6, 13-D7

int P = 6; //POWER SWITCH
int T = A2; // A2 has already been defined as Analog Pin 2 in arduino lib


void setup()
{
  lcd.begin(16, 2);
  pinMode(P, INPUT);
  Serial.begin(9600);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Systems");
  Serial.println("Systems - TEMPERATURE & POWER Sensor");
  Serial.println();
  Serial.println();
  delay(2000);
  lcd.clear();
}


/*==========================================================================================*/
/*                     FUNCTION TO GET TEMPERATURE SENSOR OUTPUT                            */
/*==========================================================================================*/

void TEMPERATURE()
{
  lcd.clear();
  int value_temp = analogRead(T); //Read analog value of temperature sensor output from pin A2
  float millivolts_temp = (value_temp / 1023.0) * 5000; //convert it to milli volts output ([actual temperature output from sensor] * [Input voltage (5V = 5000mV)] / [Resolution of ADC 2^10 = 1024])
  float CELSIUS = millivolts_temp / 10;
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(CELSIUS);
  Serial.print("Temperature : ");
  Serial.println(CELSIUS);
}

/*==========================================================================================*/
/*                     FUNCTION TO GET POWER SENSOR OUTPUT                                  */
/*==========================================================================================*/

void POWER()
{
  if (digitalRead(P) == LOW)
  {
    lcd.setCursor(0, 0);
    lcd.print("P:");
    lcd.print("OFF");
    Serial.println("Power : OFF");
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("P:");
    lcd.print("ON ");
    Serial.println("Power : ON");
  }
}

/*==========================================================================================*/
/*                     MAIN FUNCTION TO LOOP ALL THE ABOVE FUNCTIONS                        */
/*==========================================================================================*/

void loop()
{
  TEMPERATURE();
  POWER();
  Serial.println();
  delay(2000);
}

