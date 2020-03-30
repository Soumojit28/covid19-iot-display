/*
CoVid19 Stat IoT Display
By- Soumojit Ash
soumojitash@gmail.com
*/
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); 

int confirmed; 
int recovered;
int deaths;
int current_active;

const char *ssid = "Soumojit";  //ENTER YOUR WIFI SETTINGS
const char *password = "28042002a";

void setup() {
  delay(1000);
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Covid-19 Display");
  lcd.setCursor(0,1);
  lcd.print("By-Soumojit Ash");
  delay(1000);
  WiFi.mode(WIFI_STA);        
  
  WiFi.begin(ssid, password); 
  Serial.println("");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connecting");
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  lcd.setCursor(0,0);
  lcd.print("Connected!");
}

void loop() {
  HTTPClient http;    

  http.begin("http://coronago.xyz/api/data.json");  //API            

  int httpCode = http.GET();   //Send the request
  String payload = http.getString();    //Get the response payload

  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  http.end();  //Close connection
  
  StaticJsonBuffer<300> JSONBuffer;   //Memory pool
  JsonObject& parsed = JSONBuffer.parseObject(payload); //Parse message
  confirmed = parsed["confirmed"]; 
  recovered = parsed["recovered"];
  deaths = parsed["deaths"];
  current_active = parsed["active"];
  Serial.print("confirmed: ");
  Serial.println(confirmed);
  Serial.print("recovered: ");
  Serial.println(recovered);
  Serial.print("currenty_active: ");
  Serial.println(current_active);
  Serial.print("deaths: ");
  Serial.println(deaths);
  if(httpCode==200){  //display the data in lcd
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Confirmed: ");
  lcd.print(confirmed);
  lcd.setCursor(0,1);
  lcd.print("Deaths: ");
  lcd.print(deaths);
  delay(2500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Active: ");
  lcd.print(current_active);
  lcd.setCursor(0,1);
  lcd.print("Recovered:  ");
  lcd.print(recovered);
  delay(2500);
  
  }
}
