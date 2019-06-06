#include <dht11.h>
#include<LiquidCrystal.h> //include LCD library to work with LCD display
LiquidCrystal lcd(8, 9, 10, 11, 12, 13); //LCD connected to pins RS-En, E-Rs, 10-D4, 11-D5, 12-D6, 13-D7

int P = 6;  //Powersensor - 6
int R = 5;  //Reed Switch - 5
int X = 7;  //PIR sensor - 7
int T = A2;   // TEMPERATURE - A2 has already been defined as Analog Pin 2 in arduino lib
int H = A5; //HUMIDITY
int L = A4; //LIGHT
float CELSIUS, HUM, LIGHT;
dht11 DHT11;

/*==========================================================================================*/
/*                                         SETUP FUNCTION                                   */
/*==========================================================================================*/

void setup()
{
  lcd.begin(16, 2);  // Begin LCD 16 Coloumns and 2Rows
  pinMode(P, INPUT);
  pinMode(R, INPUT);
  pinMode(X, INPUT);
  Serial.begin(9600);
}

/*==========================================================================================*/
/*                     FUNCTION TO GET TEMPERATURE SENSOR OUTPUT                            */
/*==========================================================================================*/

void TEMPERATURE()
{
  lcd.clear();
  int value_temp = analogRead(T); //Read analog value of temperature sensor output from pin A2
  delay(10);
  value_temp = analogRead(T);
  delay(10);
  float millivolts_temp = (value_temp / 1023.0) * 5000; //convert it to milli volts output ([actual temperature output from sensor] * [Input voltage (5V = 5000mV)] / [Resolution of ADC 2^10 = 1024])
  CELSIUS = millivolts_temp / 10;
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(CELSIUS);
  Serial.print("Temperature : ");
  Serial.println(CELSIUS);
}

/*==========================================================================================*/
/*                     FUNCTION TO GET HUMIDITY SENSOR OUTPUT                               */
/*==========================================================================================*/

void HUMIDITY()
{
  int chk = DHT11.read(H);
  HUM = DHT11.humidity;
  lcd.setCursor(9, 0);
  lcd.print("H:");
  lcd.print(HUM);
  Serial.print("Humidity : ");
  Serial.println(HUM);
}

/*==========================================================================================*/
/*                     FUNCTION TO GET LDR SENSOR OUTPUT                                    */
/*==========================================================================================*/

void LIG()
{
  int value_lig = analogRead(L);
  delay(10);
  value_lig = analogRead(L);
  float volts_lig = (value_lig / 1023.0) * 5;
  int LIGHT = 500/(4*((5-volts_lig)/volts_lig));  // calculate the Lux = 500/[R1 * ((Vin - Vsense)/Vsense)]
  lcd.setCursor(0, 1);
  lcd.print("L:");
  lcd.print(LIGHT);
  lcd.print ("Lx");
  Serial.print("Light : ");
  Serial.print(LIGHT);
  Serial.println("  Lux");
  delay(2000);
  lcd.clear();
}

/*==========================================================================================*/
/*                     FUNCTION TO GET POWER SENSOR OUTPUT                                  */
/*==========================================================================================*/

void POWER()
{
  if (digitalRead(P) == LOW) // if output form sensor is '0' then print NO power
  {
    lcd.setCursor(0, 0);
    lcd.print("P:OFF");
    Serial.println("Power : OFF");
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("P:ON");
    Serial.println("Power : ON");
  }
}

/*==========================================================================================*/
/*                     FUNCTION TO GET REED SWITCH SENSOR OUTPUT                            */
/*==========================================================================================*/

void REED()
{
  if (digitalRead(R) == LOW)
  {
    lcd.setCursor(6, 0);
    lcd.print("R:OPEN");
    Serial.println("Reed/Door : OPEN");
  }
  else
  {
    lcd.setCursor(6, 0);
    lcd.print("R:CLOSE");
    Serial.println("Reed/Door : CLOSE");
  }
}

/*==========================================================================================*/
/*                           FUNCTION TO GET PIR SENSOR OUTPUT                              */
/*==========================================================================================*/

void PIR()
{
  if (digitalRead(X) == LOW)
  {
    lcd.setCursor(0, 1);
    lcd.print("X:YES");
    Serial.println("PIR : YES");
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print("X:NO ");
    Serial.println("PIR : NO");
  }
  delay(2000);
  Serial.println();
}

/*==========================================================================================*/
/*                     MAIN FUNCTION TO LOOP ALL THE ABOVE FUNCTIONS                        */
/*==========================================================================================*/

void loop()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Axetla Systems");
  Serial.println("Axetla Systems - All Sensor");
  Serial.println();
  Serial.println();
  delay(2000);
  lcd.clear();
  while (1)
  {
    TEMPERATURE();
    HUMIDITY();
    LIG();
    REED();
    POWER();
    PIR();
  }
}
