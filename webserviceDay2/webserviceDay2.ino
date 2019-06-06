#include <dht11.h>
#include<LiquidCrystal.h> //include LCD library to work with LCD display
#include <SoftwareSerial.h>
SoftwareSerial WiFi_Serial(3, 4); //WiFi module connected to pins 3-Rx, 4-Tx
LiquidCrystal lcd(8, 9, 10, 11, 12, 13); //LCD connected to pins RS-En, E-Rs, 10-D4, 11-D5, 12-D6, 13-D7

/*==========================================================================================*/
/*CHANGE THE PARAMETERS BELOW ACCORDING TO YOUR DEVICE INFORMATION CREATED ON AXELTA OSMOSIS*/
/*==========================================================================================*/

#define Device_No        "WKSP-01"
#define client           "Workshop"
#define Device_Type      "SAM"
#define Device_Key       "LUJPIOJV7L5NHCWQ1F4J"
#define Node_No          "012"
/*==========================================================================================*/
#define SSID "Tenda_52E3C0"      //Your WiFi username
#define PASS "pritam@1234567"  //Your WiFi Password
/*==========================================================================================*/
#define DST_IP "52.26.112.134" //axelta.com URL / IP address

int R = 5;  // REED SWITCH
int P = 6;  // POWER
int X = 7;  // PIR
int T = A2; // TEMPERATURE - A2 has already been defined as Analog Pin 2 in
int H = A5; //HUMIDITY
int L = A4; //LIGHT
int CNT = 7;
float CELSIUS, HUM, LIGHT;
dht11 DHT11;
int i = 1;
int Start_chck = 0;

/*==========================================================================================*/
/*                                         SETUP FUNCTION                                   */
/*==========================================================================================*/

void setup()
{
  Serial.begin(115200);  // Begin serial monitor to receive 115200 bits per second (BAUD RATE)
  lcd.begin(16, 2);
  WiFi_Serial.begin(115200);
  WiFi_Serial.println("AT+UART_DEF=9600,8,1,0,0"); // set WiFi Send/Receive from 115200 bits per second (BAUD RATE) to 9600 bits per second
  delay(2000);
  WiFi_Serial.begin(9600); // Begin serial monitor to receive 9600 bits per second (BAUD RATE)
  WiFi_Serial.println("ATE0");
  delay(200);
  WiFi_Serial.println("AT+CWQAP"); // Disconnect from previous network connections
  delay(500);
  pinMode(P, INPUT);
  pinMode(R, INPUT);
  pinMode(X, INPUT);

}

/*==========================================================================================*/
/*            FUNCTION TO CHECK WIFI Module CONNECTED TO ARDUINO BOARD                      */
/*==========================================================================================*/

void WIFI_Check()
{
  WiFi_Serial.println("AT"); // send a Attention command
  delay(500);
  if (WiFi_Serial.available())
  {
    if (WiFi_Serial.find("OK")) // check with expected output
    {
      Serial.println("WIFI PLUGGED ON TO THE BOARD..!");
      Serial.println();
      Serial.println();
      WiFi_Serial.println("AT+CWMODE=1"); //set mode to client mode
      delay(500);
      Start_chck = 1;
      i = 1;
    }
  }
  else
  {
    Serial.println("WIFI NOT PLUGGED..!");
    Serial.println();
    Serial.println("PLUG IN YOUR WIFI CHIP");
    Serial.println();
    Serial.println();
  }
}

/*==========================================================================================*/
/*            FUNCTION TO CHECK WIFI MODULE CONNECTED TO WIFI NETWORK                       */
/*==========================================================================================*/

void connectWiFi()
{
  WiFi_Serial.println("AT+CWJAP?"); //check if WIFI connected to any WiFi network
  delay(5000);
  if (WiFi_Serial.available())
  {
    if (WiFi_Serial.find("No AP")) //we receive reponse "No AP" when not connected to any network
    {
      Serial.println("NOT CONNECTED TO WIFI NETWORK");
      Serial.println();
      Serial.println();
      Serial.println("Trying to Connect to WiFi Network");
      String cmd = "AT+CWJAP=\""; // connected to specified network passing mentioned WiFi username and password
      cmd += SSID;
      cmd += "\",\"";
      cmd += PASS;
      cmd += "\"";
      WiFi_Serial.println(cmd);
      Serial.println(cmd);
      delay(5000);
      if (WiFi_Serial.available())
      {
        String RES_input = "";
        while (WiFi_Serial.available()) // read the data into a variable as long as the
        {
          RES_input += (char)WiFi_Serial.read();
        }
        Serial.println(RES_input);
        if (WiFi_Serial.find("WIFI CONNECTED"))
        {
          Serial.println("CONNECTED TO WIFI NETWORK..!");
        }
      }
    }
    else
    {
      lcd.clear();
      lcd.print("CONNECTED TO");
      lcd.setCursor(0, 1);
      lcd.print("WIFI NETWORK..!!");
      Serial.println("CONNECTED TO WIFI NETWORK..!");
      Serial.println();
      Serial.println();
      post();
      i = 0;
    }
  }
}

/*==========================================================================================*/
/*                     FUNCTION TO GET TEMPERATURE SENSOR OUTPUT                            */
/*==========================================================================================*/

void TEMPERATURE()
{
  Serial.print("Geeting Temp\t");
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
  Serial.print("T:\t");
  Serial.print(CELSIUS); Serial.print("\t");
}

/*==========================================================================================*/
/*                     FUNCTION TO GET HUMIDITY SENSOR OUTPUT                               */
/*==========================================================================================*/

void HUMIDITY()
{
  Serial.print("Getting Hum ");
  int chk = DHT11.read(H);
  HUM = DHT11.humidity;
  lcd.setCursor(9, 0);
  lcd.print("H:");
  lcd.print(HUM);
  Serial.print("H: ");
  Serial.print(HUM); Serial.print("\t");
}

/*==========================================================================================*/
/*                     FUNCTION TO GET LDR SENSOR OUTPUT                                    */
/*==========================================================================================*/

void LIG()
{
  Serial.print("Getting Lig ");
  int value_lig = analogRead(L);
  delay(10);
  value_lig = analogRead(L);
  float volts_lig = (value_lig / 1023.0) * 5;
  LIGHT = 500/(4*((5-volts_lig)/volts_lig));  // calculate the Lux = 500/[R1 * ((Vin - Vsense)/Vsense)]
  lcd.setCursor(0, 1);
  lcd.print("L:");
  lcd.print(LIGHT);
  Serial.print("L: ");
  Serial.print(LIGHT); Serial.println("\t");
  delay(2000);
  lcd.clear();
}

/*==========================================================================================*/
/*                     FUNCTION TO GET POWER SENSOR OUTPUT                                  */
/*==========================================================================================*/

void POWER()
{
  Serial.print("PWR STATUS\t");
  if (digitalRead(P) == LOW) // if output form sensor is '0' then print NO power
  {
    lcd.setCursor(0, 0);
    lcd.print("P:OFF");
    Serial.print("P:OFF"); Serial.print("\t");
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("P:ON");
    Serial.print("P:ON"); Serial.print("\t");
  }
}

/*==========================================================================================*/
/*                     FUNCTION TO GET REED SWITCH SENSOR OUTPUT                            */
/*==========================================================================================*/

void REED()
{
  Serial.print("REED STATUS ");
  if (digitalRead(R) == LOW)
  {
    lcd.setCursor(6, 0);
    lcd.print("R:OPEN");
    Serial.print("R:OPEN"); Serial.print("\t");
  }
  else
  {
    lcd.setCursor(6, 0);
    lcd.print("R:CLOSE");
    Serial.print("R:CLOSE"); Serial.print("\t");
  }
}

/*==========================================================================================*/
/*                           FUNCTION TO GET PIR SENSOR OUTPUT                              */
/*==========================================================================================*/

void PIR()
{
  Serial.print("PIR STATUS ");
  if (digitalRead(X) == LOW)
  {
    lcd.setCursor(0, 1);
    lcd.print("X:YES");
    Serial.print("X:YES"); Serial.print("\t");
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print("X:NO ");
    Serial.print("X:NO"); Serial.println("\t");
  }
  delay(2000);
}

/*==========================================================================================*/
/*          FUNCTION TO POST THE DATA TO AXELTA OSMOSIS CLOUD PLATFORM                      */
/*==========================================================================================*/

void post()
{
  String data; //form the JSON string with the available sensor data
  data += "{\"device_no\":\"" + String(Device_No) + "\",\"client\":\"" + String(client) + "\",\"device_type\":\"" + String(Device_Type) + "\",\"device_key\":\"" +  String(Device_Key) + "\",\"node_no\":\"" + String(Node_No) + "\",\"TEMP\":\"";
  data += String(CELSIUS);
  data += "\",\"HUM\":\"";
  data += String(HUM);
  data += "\",\"LDR\":\"";
  data += String(LIGHT);
  data += "\",\"POWER\":\"";
  if (digitalRead(P) == LOW)
  {
    data += "ON.";
  }
  else
  {
    data += "OFF";
  }
  data += "\",\"DOOR\":\"";
  if (digitalRead(R) == LOW)
  {
    data += "OPEN.";
  } 
  else
  {
    data += "CLOSE";
  }
  data += "\"}";
  String uri = "/WebServicesTraining/rest/service/storeData";
  String port = "8080";
  String http_req = "POST " + uri + " HTTP/1.1\r\n" + "Host: " + DST_IP + ":" + port + "\r\n" + "Accept: *" + "/" + "*\r\n" + "Content-Length: " + data.length() + "\r\n" ;
  String http_req1 = "Content-Type: application/json\r\n\r\n" ; // form the headder to post the WiFi data
  Serial.println(data);
  Serial.println(http_req);
  Serial.println(http_req1);
  int len_hedder1 = (http_req.length());
  int len_hedder2 = (http_req1.length());
  int len_data = data.length();
  int Total_req_data_Length = (len_hedder1 + len_hedder2 + len_data); // total length of headder and data
  Serial.println();
  String cmd = "AT+CIPSTART=\"TCP\",\""; // open the tcp port for Axelta Osmosis
  cmd += DST_IP;
  cmd += "\",8080";
  Serial.println(cmd);
  WiFi_Serial.println(cmd);
  delay(5000);
  if (WiFi_Serial.available())
  {
    String RES_input = "";
    while (WiFi_Serial.available()) // read the data into a variable as long as the
    {
      RES_input += (char)WiFi_Serial.read();
    }
    Serial.println(RES_input);
    if (WiFi_Serial.find("CONNECT"));
    Serial.print("AT+CIPSEND="); // start sending data to opened TCP port
    WiFi_Serial.print("AT+CIPSEND=");
    WiFi_Serial.println(Total_req_data_Length); //specify the total length of data
    delay(100);
    if (WiFi_Serial.available());
    RES_input = "";
    while (WiFi_Serial.available()) // read the data into a variable as long as the
    {
      RES_input += (char)WiFi_Serial.read();
    }
    Serial.println(RES_input);
    {
      if (WiFi_Serial.find(">")); // when ">" is response from WiFi that means it is ready to receive the total length of data 
      {
        WiFi_Serial.print(http_req); //Send headder first
        WiFi_Serial.print(http_req1);
        WiFi_Serial.print(data); //later send data
        delay(2000);
      }
    }
    if (WiFi_Serial.available()) // wait for response and print the same to terminal
    {
      delay(100);
      String RES_input = "";
      while (WiFi_Serial.available()) // read the data into a variable as long as the
      {
        RES_input += (char)WiFi_Serial.read();
      }
      lcd.clear();
      Serial.println(RES_input);
      Serial.println();
      Serial.println();
    }
  }
}
void loop()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Axetla Systems");
  delay(1000);
  lcd.clear();
  while (1)
  {
    if (CNT < 1) //CNT for Time Delay
    {
      if (Start_chck == 0)
      {
        WIFI_Check();
        if (i == 1)
        {
          connectWiFi();
        }
      }
      else
      {
        CNT = 7;
        Start_chck = 0;
      }
    }
    TEMPERATURE();
    HUMIDITY();
    LIG();
    POWER();
    REED();
    PIR();
    Serial.println();
    CNT--;
  }
}
