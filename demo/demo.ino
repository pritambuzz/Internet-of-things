
#include<LiquidCrystal.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(3,4);
LiquidCrystal lcd(8,9,10,11,12,13);
int CNT=4;
String gsm_input="";
String RES_input="";
int i=1;
int J=0;
int Start_chck=0;

void setup()
    {
      Serial.begin(9600);
      mySerial.begin(9600);
      lcd.begin(16,2);
    }
//////////***GSM INITILALIZATION***/////////// 
void gsminit()    
{
       mySerial.println("AT");
       DisplayGSMResponse();
       mySerial.println("ATE0");
       DisplayGSMResponse();
}
//////////***HTTP INITIALIZATION***/////////// 
void httpinit()   
    {
      Serial.println("HTTP INITIALIZING");
      mySerial.println("AT+CSQ");
      delay(2000);
      mySerial.println("AT+CREG?");
      delay(2000);
      mySerial.println("AT+CGATT?");
      delay(2000);
      mySerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
      delay(2000);
      DisplayGSMResponse();
      mySerial.println("AT+SAPBR=3,1,\"APN\",\"INTERNET\"");
      delay(2000);  
      DisplayGSMResponse();
      mySerial.println("AT+SAPBR=1,1");
      delay(15000);
      DisplayGSMResponse();
      mySerial.println("AT+SAPBR=2,1");
      delay(2000);
      DisplayGSMResponse();
      mySerial.println("AT+HTTPINIT");
      delay(2000);
      DisplayGSMResponse();
      mySerial.println("AT+HTTPPARA=\"CID\",1");
      delay(2000);
      DisplayGSMResponse();
      mySerial.println("AT+HTTPPARA=\"URL\",\"http://aws2.axelta.com/services/data\"");
      delay(2000);
      DisplayGSMResponse();
      mySerial.println("AT+HTTPPARA=\"Content\",\"application/json\"");
      delay(2000);
      DisplayGSMResponse();
    }
//////////***POST THE DATA***///////////  
void httppost()   
    {
      Serial.println("POSTING THE DATA");
      String data;
      delay(500);
      data += "{\"device_no\":\"ENVS\",\"client\":\"Environment setup\",\"device_type\":\"Cold Chain\",\"device_key\":\"KSUCQHLYMNTEPWLW4XVC\",\"node_no\":\"001\",\"Temp\":\"";
      data += String("45");
      data += "\"}";                              
      Serial.println(data);
      int size = data.length();
      String initData = "AT+HTTPDATA=";
      initData += String(size);
      initData += ", 40000";
      mySerial.println(initData);
      delay(500);
      DisplayGSMResponse();
      mySerial.println(data);
      lcd.clear();
      lcd.print("Connecting to");
      lcd.setCursor(0,1);
      lcd.print("Server....!!!");
      delay(2000);
      mySerial.println("AT+HTTPACTION=1");
      delay(10000);
      if(mySerial.available())
      {
      String RES_input="";
      while(mySerial.available()) // read the data into a variable as long as the 
      {
      RES_input+= (char)mySerial.read();
      }
      lcd.clear();
      Serial.println(RES_input);
      if(find_string(RES_input,","))
      {
      int loc = find_char_loc(RES_input,'1');
      String no = RES_input.substring(loc+1);
      String response=no.substring(1,4);   
      Serial.println(response);
      if(response=="200")
      {
      lcd.clear();
      lcd.print("RESPONSE:");
      lcd.setCursor(10,0);
      lcd.print(response);
      lcd.setCursor(0,1);
      lcd.print("DATA POSTED");
      Serial.println("Board Working Fine");
      delay(1000);
      }
      else
      {
      lcd.clear();
      lcd.print("Error in Posting");
      delay(1000);
        }
      }  
     }
   }
//////////***GSM TERMINATION***///////////    
void gsmterm()    
    {
      mySerial.println("AT+ HTTPTERM");
      delay(2000);
      DisplayGSMResponse();
      mySerial.println("AT+SAPBR=0,1");
      delay(2000);
      DisplayGSMResponse();
      lcd.clear();
    }

   
//////////***MAIN LOOP***///////////   
void loop()
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Axetla Systems");
      delay(1000);
      lcd.clear();
      while(1)
      {        
      if(CNT<1)   //CNT for Time Delay
      {
      if(Start_chck==0)
      {
      GSM_Check();
      i=0;
      delay(2000);
      }
      else
      {
      gsminit();
      delay(2000);
      httpinit();
      delay(2000);
      httppost();
      delay(2000);
      gsmterm();
      delay(2000);
      CNT=4;
      i=1;
      J=0;
      Start_chck=0;
      }
      }
      J++;
     lcd.print(J);
     Serial.println(J);
     delay(2000);
     Serial.println();
      CNT--;
      }
    }
//////////***GSM RESPONSE***///////////     
void DisplayGSMResponse()
{
      if(mySerial.available())
      {
      String gsm_input="";
      while(mySerial.available()) 
      {
      gsm_input+= (char)mySerial.read();
      }
      gsm_input.trim();
      Serial.println(gsm_input);
      delay(1000);
      }
}
//////////***GSN CONNECTION CHECK***/////////// 
void GSM_Check()
{
      mySerial.println("AT");
      if(mySerial.available())
      {
      String gsm_input="";
      while(mySerial.available()) // read the data into a variable as long as the 
      {
      gsm_input+= (char)mySerial.read();
      }
      if(find_string(gsm_input,"O"))
      {
      int loc = find_char_loc(gsm_input,'O');
      String response = gsm_input.substring(loc);
      Serial.println(response);
      if(response="K")
      {
      lcd.clear();
      lcd.print("GSM CONNECTED..!");
      Serial.println("GSM CONNECTED..!");
      Start_chck=1;
      i=1;
      }    
      }
      }
     if(i==0)
      {
      lcd.clear();
      lcd.print("GSM NOT");
      lcd.setCursor(0,1);
      lcd.print("CONNECTED..!!");  
      Serial.println("GSM  NOT CONNECTED..!");
      }
}


// A Function to locate a given search string in a given base string
boolean find_string(String base, String search)
{
     int len = search.length(); // find the length of the base string
     for(int m = 0; m<((base.length()-len)+1);m++)// Iterate from the beginning of the base string till the end minus length of the substring
     {
     if(base.substring(m,(m+len))==search) // Check if the extracted Substring Matches the Search String
     {
     return true; // if it matches exit the function with a true value
     }
     }
     return false; // if the above loop did not find any matches, control would come here and return a false value
}

    // A Function to locate a given search character in a given base string and return its position
int find_char_loc(String base, char search)
{
      for(int m = 0; m < base.length();m++)// Iterate from the beginning of the base string till the end minus length of the substring
      {
        if(base[m]==search) // Check if the character Matches the Search character
        {
          return m; // if it matches exit the function with the current location value
        }
      }
}
