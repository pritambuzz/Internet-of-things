#include <dht11.h>
#include<LiquidCrystal.h> //include LCD library to work with LCD display
#include <SoftwareSerial.h>
SoftwareSerial GSM_Serial(3, 4); //GSM module connected to pins 3-Rx, 4-Tx
LiquidCrystal lcd(8, 9, 10, 11, 12, 13); //LCD connected to pins RS-En, E-Rs, 10-D4, 11-D5, 12-D6, 13-D7
int LED = A3; // LED Connected to GPIO pin A3 of Arduino 
String response1 = "on"; // Expected Response from SMS

/*==========================================================================================*/
/*                                         SETUP FUNCTION                                   */
/*==========================================================================================*/

void setup()
{
  Serial.begin(9600);   // Begin serial monitor to receive 9600 bits per second (BAUD RATE)
  GSM_Serial.begin(9600); // Begin GSM Module to send/receive 9600 bits per second (BAUD RATE)
  lcd.begin(16, 2);  // Begin LCD 16 Coloumns and 2Rows 
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

/*==========================================================================================*/
/*          A Function to locate a given search string in a given base string               */
/*==========================================================================================*/

boolean find_string(String base, String search)
{
  int len = search.length(); // find the length of the base string
  for (int m = 0; m < ((base.length() - len) + 1); m++) // Iterate from the beginning of the base string till the end minus length of the substring
  {
    if (base.substring(m, (m + len)) == search) // Check if the extracted Substring Matches the Search String
    {
      return true; // if it matches exit the function with a true value
    }
  }
  return false; // if the above loop did not find any matches, control would come here and return a false value
}

/*=============================================================================================*/
/* A Function to locate a given search character in a given base string and return its position*/
/*=============================================================================================*/

boolean find_char_loc(String base, char search)
{
  for (int m = 0; m < base.length(); m++) // Iterate from the beginning of the base string till the end minus length of the substring
  {
    if (base[m] == search) // Check if the character Matches the Search character
    {
      return m; // if it matches exit the function with the current location value
    }
  }
  return 0; // if the above loop did not find any matches, control would come here and return a zero value
}

/*=============================================================================================*/
/*          A Function to initialize GSM module and read the first available SMS               */
/*=============================================================================================*/

void gsminit()
{
  GSM_Serial.println("AT");
  delay(2000);
  GSM_Serial.println("ATE0");
  delay(2000);
  GSM_Serial.println("AT+CMGF=1");
  delay(2000);
  GSM_Serial.println("AT+CNMI=1,2,0,0,0");
  delay(1000);
}

/*=============================================================================================*/
/*             A Function to read the first SMS and compare with expected string               */
/*=============================================================================================*/

void gsm_sms_read()
{
  if (GSM_Serial.available())
  {
    String gsm_input = "";
    while (GSM_Serial.available()) // read the data into a variable as long as the buffer is not empty
    {
      gsm_input += (char)GSM_Serial.read();// take character by character from the avaialble data on GSM and form a String
    }
    Serial.print(gsm_input);
    gsm_input.toLowerCase();
    if (find_string(gsm_input, response1)) // Check if the message contains the text stored in the response1 variable
    {
      Serial.print("SMS received");
      lcd.clear();
      lcd.print("SMS received");
      lcd.setCursor(0, 1);
      lcd.print("LED ON ");
      delay(1000);
      digitalWrite(LED, HIGH);
      delay(10000);
      lcd.clear();
      lcd.print("waiting for SMS");
      lcd.setCursor(0, 1);
      lcd.print("LED OFF");
      digitalWrite(LED, LOW);
      GSM_Serial.println("AT+CMGD=1,4"); // Delete the previous read SMS
      delay(3000);
    }
  }
}

/*==========================================================================================*/
/*                     MAIN FUNCTION TO LOOP ALL THE ABOVE FUNCTIONS                        */
/*==========================================================================================*/

void loop()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Axetla Systems");
  gsminit();
  GSM_Serial.println("AT+CMGD=1,4");
  lcd.clear();
  lcd.print("waiting for SMS");
  lcd.setCursor(0, 1);
  lcd.print("ALARM OFF");
  while (1)
  {
    gsm_sms_read();
  }
}
