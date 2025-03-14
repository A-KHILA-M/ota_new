///https://docs.google.com/spreadsheets/d/1uJD5nqwJE25X-TCBUowdOIqDUynoOkmlepiBBgUbSsQ/edit?usp=sharing    
#include <Arduino.h>
#include <ArduinoRS485.h>       // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>                                                                
#include <WiFi.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <ArduinoJson.h>
// Google Apps Script details
String GOOGLE_SCRIPT_ID = "AKfycbysfX5poJinxCZ5LojvxqaOgGpzAO22lu39VjIp2yKubDJc-sxBT4XPa4819ARVVb_J";
String dynamicURL = ""; // Placeholder for the URL to fetch
String alert = "";
const char* ssid = "Payagri";
const char* password = "payAgri@600113";
long dat[125] = {0};
//ROOM6
uint16_t R6Temp, R6Hum, R6Co2, R6Ethylene, R6pulpTemp;
uint16_t R6temp_high, R6temp_low, R6hum_high, R6hum_low, R6co2_high, R6co2_low, R6ethylene_high, R6ethylene_low;
uint16_t R6curr_stage, R6set_dose_interval, R6set_lockcycle, R6curr_dose_interval, R6curr_lockcycle, R6precool_time, R6precool_elapsed_time, R6dose_time, R6dose_elapsed_time, R6vent_time, R6vent_elapsedTime, R6hold_time, R6hold_elapsedTime;
uint16_t R6CompDo, R6HumDo, R6DampDo, R6SolDo;
//ROOM7
uint16_t R7Temp, R7Hum, R7Co2, R7Ethylene, R7pulpTemp;
uint16_t R7temp_high, R7temp_low, R7hum_high, R7hum_low, R7co2_high, R7co2_low, R7ethylene_high, R7ethylene_low;
uint16_t R7curr_stage, R7set_dose_interval, R7set_lockcycle, R7curr_dose_interval, R7curr_lockcycle, R7precool_time, R7precool_elapsed_time, R7dose_time, R7dose_elapsed_time, R7vent_time, R7vent_elapsedTime, R7hold_time, R7hold_elapsedTime;
uint16_t R7CompDo, R7HumDo, R7DampDo, R7SolDo;
//ROOM8
uint16_t R8Temp, R8Hum, R8Co2, R8Ethylene, R8pulpTemp;
uint16_t R8temp_high, R8temp_low, R8hum_high, R8hum_low, R8co2_high, R8co2_low, R8ethylene_high, R8ethylene_low;
uint16_t R8curr_stage, R8set_dose_interval, R8set_lockcycle, R8curr_dose_interval, R8curr_lockcycle, R8precool_time, R8precool_elapsed_time, R8dose_time, R8dose_elapsed_time, R8vent_time, R8vent_elapsedTime, R8hold_time, R8hold_elapsedTime;
uint16_t R8CompDo, R8HumDo, R8DampDo, R8SolDo;
//ROOM9
uint16_t R9Temp, R9Hum, R9Co2, R9Ethylene, R9pulpTemp;
uint16_t R9temp_high, R9temp_low, R9hum_high, R9hum_low, R9co2_high, R9co2_low, R9ethylene_high, R9ethylene_low;
uint16_t R9curr_stage, R9set_dose_interval, R9set_lockcycle, R9curr_dose_interval, R9curr_lockcycle, R9precool_time, R9precool_elapsed_time, R9dose_time, R9dose_elapsed_time, R9vent_time, R9vent_elapsedTime, R9hold_time, R9hold_elapsedTime;
uint16_t R9CompDo, R9HumDo, R9DampDo, R9SolDo;
uint16_t sol_onTime, sol_offTime, CylinderPressure, Sensor_Mode;

// Email timing and interval
unsigned long lastEmailTime = 0; // Store the last email send time
const unsigned long emailInterval = 60000;   //5 * 60 * 1000; // 5 minutes in milliseconds

RS485Class RS1_485(Serial2, 17, 16, 5, 5, 0);
ModbusRTUServerClass ModbusRTUServer1;
WiFiClient espClient;
int status = WL_IDLE_STATUS;
WiFiManager wifiManager;
unsigned long previousMillis = 0;
const int interval = 60000;
void fetchDynamicURL() {
    HTTPClient http;
    String url7 = "https://script.google.com/macros/s/" +GOOGLE_SCRIPT_ID+ "/exec?read";
    http.begin(url7.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();
    if (httpCode > 0) 
    {
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
        // Parse JSON response to get the URL
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, payload);
        if (!error) 
        {
            dynamicURL = doc["url7"].as<String>(); // Assuming the spreadsheet returns {"url7": "<your_url>"}
            Serial.println("Fetched URL: " + dynamicURL);
        } 
        else 
        {
            Serial.println("JSON parsing error");
        }
    } 
    else 
    {
        Serial.println("Error on HTTP request");
    }
    http.end();
}
void spreadsheet_comm1(float r6Temp, float r6Hum, float r6Co2, float r6Ethylene, float r6pulpTemp, float r6temp_high, float r6temp_low, float r6hum_high, float r6hum_low, float r6co2_high, float r6co2_low, float r6ethylene_high, float r6ethylene_low,  float r6curr_stage, float r6set_dose_interval, float r6set_lockcycle, float r6curr_dose_interval, float r6curr_lockcycle, float r6precool_time, float r6precool_elapsed_time, float r6dose_time, float r6dose_elapsed_time, float r6vent_time, float r6vent_elapsedTime, float r6hold_time, float r6hold_elapsedTime, float r6CompDo, float r6HumDo, float r6DampDo, float r6SolDo, float r7Temp, float r7Hum, float r7Co2, float r7Ethylene, float r7pulpTemp, float r7temp_high, float r7temp_low, float r7hum_high, float r7hum_low, float r7co2_high, float r7co2_low, float r7ethylene_high, float r7ethylene_low,  float r7curr_stage, float r7set_dose_interval, float r7set_lockcycle, float r7curr_dose_interval, float r7curr_lockcycle, float r7precool_time, float r7precool_elapsed_time, float r7dose_time, float r7dose_elapsed_time, float r7vent_time, float r7vent_elapsedTime, float r7hold_time, float r7hold_elapsedTime, float r7CompDo, float r7HumDo, float r7DampDo, float r7SolDo, float r8Temp, float r8Hum, float r8Co2, float r8Ethylene, float r8pulpTemp, float r8temp_high, float r8temp_low, float r8hum_high, float r8hum_low, float r8co2_high, float r8co2_low, float r8ethylene_high, float r8ethylene_low,  float r8curr_stage, float r8set_dose_interval, float r8set_lockcycle, float r8curr_dose_interval, float r8curr_lockcycle, float r8precool_time, float r8precool_elapsed_time, float r8dose_time, float r8dose_elapsed_time, float r8vent_time, float r8vent_elapsedTime, float r8hold_time, float r8hold_elapsedTime, float r8CompDo, float r8HumDo, float r8DampDo, float r8SolDo, float r9Temp, float r9Hum, float r9Co2, float r9Ethylene, float r9pulpTemp, float r9temp_high, float r9temp_low, float r9hum_high, float r9hum_low, float r9co2_high, float r9co2_low, float r9ethylene_high, float r9ethylene_low,  float r9curr_stage, float r9set_dose_interval, float r9set_lockcycle, float r9curr_dose_interval, float r9curr_lockcycle, float r9precool_time, float r9precool_elapsed_time, float r9dose_time, float r9dose_elapsed_time, float r9vent_time, float r9vent_elapsedTime, float r9hold_time, float r9hold_elapsedTime, float r9CompDo, float r9HumDo, float r9DampDo, float r9SolDo)
{    
    if (dynamicURL.isEmpty()) 
    {
        Serial.println("Dynamic URL not set!");
        return;
    }
        HTTPClient http;
        http.begin(dynamicURL);
        http.addHeader("Content-Type", "application/json"); 
        String payload = "{";
        payload += "\"sensor6\": {\"r6curr_stage\":" + String(r6curr_stage) + 
                 ",\"r6Temp\":" + String(r6Temp) + 
                 ",\"r6Hum\":" + String(r6Hum) + 
                 ",\"r6Co2\":" + String(r6Co2) + 
                 ",\"r6Ethylene\":" + String(r6Ethylene) + 
                 ",\"r6pulpTemp\":" + String(r6pulpTemp) + 
                 ",\"r6temp_high\":" + String(r6temp_high) + 
                 ",\"r6temp_low\":" + String(r6temp_low) + 
                 ",\"r6hum_high\":" + String(r6hum_high) + 
                 ",\"r6hum_low\":" + String(r6hum_low) + 
                 ",\"r6co2_high\":" + String(r6co2_high) + 
                 ",\"r6co2_low\":" + String(r6co2_low) + 
                 ",\"r6ethylene_high\":" + String(r6ethylene_high) + 
                 ",\"r6ethylene_low\":" + String(r6ethylene_low) + 
                 ",\"r6set_dose_interval\":" + String(r6set_dose_interval) + 
                 ",\"r6set_lockcycle\":" + String(r6set_lockcycle) +
                 ",\"r6curr_dose_interval\":" + String(r6curr_dose_interval) + 
                 ",\"r6curr_lockcycle\":" + String(r6curr_lockcycle) + 
                 ",\"r6precool_time\":" + String(r6precool_time) + 
                 ",\"r6precool_elapsed_time\":" + String(r6precool_elapsed_time) + 
                 ",\"r6dose_time\":" + String(r6dose_time) + 
                 ",\"r6dose_elapsed_time\":" + String(r6dose_elapsed_time) + 
                 ",\"r6vent_time\":" + String(r6vent_time) + 
                 ",\"r6vent_elapsedTime\":" + String(r6vent_elapsedTime) + 
                 ",\"r6hold_time\":" + String(r6hold_time) + 
                 ",\"r6hold_elapsedTime\":" + String(r6hold_elapsedTime) + 
                 ",\"r6CompDo\":" + String(r6CompDo) + 
                 ",\"r6HumDo\":" + String(r6HumDo) +
                 ",\"r6DampDo\":" + String(r6DampDo) + 
                 ",\"r6SolDo\":" + String(r6SolDo) + "},";
        payload += "\"sensor7\": {\"r7curr_stage\":" + String(r7curr_stage) + 
                 ",\"r7Temp\":" + String(r7Temp) + 
                 ",\"r7Hum\":" + String(r7Hum) + 
                 ",\"r7Co2\":" + String(r7Co2) + 
                 ",\"r7Ethylene\":" + String(r7Ethylene) + 
                 ",\"r7pulpTemp\":" + String(r7pulpTemp) + 
                 ",\"r7temp_high\":" + String(r7temp_high) + 
                 ",\"r7temp_low\":" + String(r7temp_low) + 
                 ",\"r7hum_high\":" + String(r7hum_high) + 
                 ",\"r7hum_low\":" + String(r7hum_low) + 
                 ",\"r7co2_high\":" + String(r7co2_high) + 
                 ",\"r7co2_low\":" + String(r7co2_low) + 
                 ",\"r7ethylene_high\":" + String(r7ethylene_high) + 
                 ",\"r7ethylene_low\":" + String(r7ethylene_low) + 
                 ",\"r7set_dose_interval\":" + String(r7set_dose_interval) + 
                 ",\"r7set_lockcycle\":" + String(r7set_lockcycle) +
                 ",\"r7curr_dose_interval\":" + String(r7curr_dose_interval) + 
                 ",\"r7curr_lockcycle\":" + String(r7curr_lockcycle) + 
                 ",\"r7precool_time\":" + String(r7precool_time) + 
                 ",\"r7precool_elapsed_time\":" + String(r7precool_elapsed_time) + 
                 ",\"r7dose_time\":" + String(r7dose_time) + 
                 ",\"r7dose_elapsed_time\":" + String(r7dose_elapsed_time) + 
                 ",\"r7vent_time\":" + String(r7vent_time) + 
                 ",\"r7vent_elapsedTime\":" + String(r7vent_elapsedTime) + 
                 ",\"r7hold_time\":" + String(r7hold_time) + 
                 ",\"r7hold_elapsedTime\":" + String(r7hold_elapsedTime) + 
                 ",\"r7CompDo\":" + String(r7CompDo) + 
                 ",\"r7HumDo\":" + String(r7HumDo) +
                 ",\"r7DampDo\":" + String(r7DampDo) + 
                 ",\"r7SolDo\":" + String(r7SolDo) + "},";
        payload += "\"sensor8\": {\"r8curr_stage\":" + String(r8curr_stage) + 
                 ",\"r8Temp\":" + String(r8Temp) + 
                 ",\"r8Hum\":" + String(r8Hum) + 
                 ",\"r8Co2\":" + String(r8Co2) + 
                 ",\"r8Ethylene\":" + String(r8Ethylene) + 
                 ",\"r8pulpTemp\":" + String(r8pulpTemp) + 
                 ",\"r8temp_high\":" + String(r8temp_high) + 
                 ",\"r8temp_low\":" + String(r8temp_low) + 
                 ",\"r8hum_high\":" + String(r8hum_high) + 
                 ",\"r8hum_low\":" + String(r8hum_low) + 
                 ",\"r8co2_high\":" + String(r8co2_high) + 
                 ",\"r8co2_low\":" + String(r8co2_low) + 
                 ",\"r8ethylene_high\":" + String(r8ethylene_high) + 
                 ",\"r8ethylene_low\":" + String(r8ethylene_low) + 
                 ",\"r8set_dose_interval\":" + String(r8set_dose_interval) + 
                 ",\"r8set_lockcycle\":" + String(r8set_lockcycle) +
                 ",\"r8curr_dose_interval\":" + String(r8curr_dose_interval) + 
                 ",\"r8curr_lockcycle\":" + String(r8curr_lockcycle) + 
                 ",\"r8precool_time\":" + String(r8precool_time) + 
                 ",\"r8precool_elapsed_time\":" + String(r8precool_elapsed_time) + 
                 ",\"r8dose_time\":" + String(r8dose_time) + 
                 ",\"r8dose_elapsed_time\":" + String(r8dose_elapsed_time) + 
                 ",\"r8vent_time\":" + String(r8vent_time) + 
                 ",\"r8vent_elapsedTime\":" + String(r8vent_elapsedTime) + 
                 ",\"r8hold_time\":" + String(r8hold_time) + 
                 ",\"r8hold_elapsedTime\":" + String(r8hold_elapsedTime) + 
                 ",\"r8CompDo\":" + String(r8CompDo) + 
                 ",\"r8HumDo\":" + String(r8HumDo) +
                 ",\"r8DampDo\":" + String(r8DampDo) + 
                 ",\"r8SolDo\":" + String(r8SolDo) + "},";
        payload += "\"sensor9\": {\"r9curr_stage\":" + String(r9curr_stage) + 
                 ",\"r9Temp\":" + String(r9Temp) + 
                 ",\"r9Hum\":" + String(r9Hum) + 
                 ",\"r9Co2\":" + String(r9Co2) + 
                 ",\"r9Ethylene\":" + String(r9Ethylene) + 
                 ",\"r9pulpTemp\":" + String(r9pulpTemp) + 
                 ",\"r9temp_high\":" + String(r9temp_high) + 
                 ",\"r9temp_low\":" + String(r9temp_low) + 
                 ",\"r9hum_high\":" + String(r9hum_high) + 
                 ",\"r9hum_low\":" + String(r9hum_low) + 
                 ",\"r9co2_high\":" + String(r9co2_high) + 
                 ",\"r9co2_low\":" + String(r9co2_low) + 
                 ",\"r9ethylene_high\":" + String(r9ethylene_high) + 
                 ",\"r9ethylene_low\":" + String(r9ethylene_low) + 
                 ",\"r9set_dose_interval\":" + String(r9set_dose_interval) + 
                 ",\"r9set_lockcycle\":" + String(r9set_lockcycle) +
                 ",\"r9curr_dose_interval\":" + String(r9curr_dose_interval) + 
                 ",\"r9curr_lockcycle\":" + String(r9curr_lockcycle) + 
                 ",\"r9precool_time\":" + String(r9precool_time) + 
                 ",\"r9precool_elapsed_time\":" + String(r9precool_elapsed_time) + 
                 ",\"r9dose_time\":" + String(r9dose_time) + 
                 ",\"r9dose_elapsed_time\":" + String(r9dose_elapsed_time) + 
                 ",\"r9vent_time\":" + String(r9vent_time) + 
                 ",\"r9vent_elapsedTime\":" + String(r9vent_elapsedTime) + 
                 ",\"r9hold_time\":" + String(r9hold_time) + 
                 ",\"r9hold_elapsedTime\":" + String(r9hold_elapsedTime) + 
                 ",\"r9CompDo\":" + String(r9CompDo) + 
                 ",\"r9HumDo\":" + String(r9HumDo) +
                 ",\"r9DampDo\":" + String(r9DampDo) + 
                 ",\"r9SolDo\":" + String(r9SolDo) + "}";
        payload += "}";
        
        int httpResponseCode = http.POST(payload);
        Serial.println("Response code: " + String(httpResponseCode));
        Serial.println("Response: " + http.getString());
        
        http.end();
}
void setup()
{
  Serial.begin(19200);
  wifiManager.setDebugOutput(true);
//  wifiManager.resetSettings();
  while (!Serial) {
    delay(100);
  }
   // Serial.println("Connected to WiFi");
  RS1_485.begin(19200,50,50);  
  if (!ModbusRTUServer1.begin(RS1_485,6,19200,SERIAL_8N1)) 
  {     
    Serial.println("Failed to start Modbus RTU Server!");
    while (1);
  }
  ModbusRTUServer1.configureHoldingRegisters(0x00,125);
  // Check WiFi connection status
  if (WiFi.status() != WL_CONNECTED) 
  {
    Serial.println("WiFi connection lost. Reconnecting...");
    // Attempt to reconnect
    WiFi.begin();
    unsigned long startAttemptTime = millis();
    // Wait for connection attempt to timeout
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) 
    {
      delay(100);
    }
    if (WiFi.status() != WL_CONNECTED) 
    {
      Serial.println("Failed to reconnect to WiFi");
      delay(5000); // Wait before retrying
      return; // Exit the loop if reconnection fails
    }
    Serial.println("Reconnected to WiFi");
  }
  fetchDynamicURL();
}
void loop()
{
   if (!wifiManager.autoConnect("6_ROOM_IOT")) {

    Serial.println("Failed to connect and hit timeout");
  }

  // Check WiFi connection status
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost. Reconnecting...");
    // Attempt to reconnect
    WiFi.begin();
    unsigned long startAttemptTime = millis();
    // Wait for connection attempt to timeout
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
      delay(100);
    }
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Failed to reconnect to WiFi");
      delay(5000); // Wait before retrying
      return; // Exit the loop if reconnection fails
    }
    Serial.println("Reconnected to WiFi");
  }
  ModbusRTUServer1.poll(); //ROOM 1

  for(int i=0;i<125;i++){
    dat[i] = ModbusRTUServer1.holdingRegisterRead(i);
    Serial.println("dat["+String(i)+"] : "+String(dat[i]));
   // delay(6000);
  }
  R6Temp=dat[1];
  R6Hum =dat[2];
  R6Co2=dat[3];
  R6Ethylene=dat[4];
  R6pulpTemp=dat[5];
  R6temp_high=dat[6];
  R6temp_low=dat[7];
  R6hum_high=dat[8];
  R6hum_low=dat[9];
  R6co2_high=dat[10];
  R6co2_low=dat[11];
  R6ethylene_high=dat[12];
  R6ethylene_low=dat[13];
  R6curr_stage=dat[14];
  R6set_dose_interval =dat[15];
  R6set_lockcycle=dat[16];
  R6curr_dose_interval=dat[17];
  R6curr_lockcycle=dat[18];
  R6precool_time =dat[19];
  R6precool_elapsed_time =dat[20];
  R6dose_time =dat[21];
  R6dose_elapsed_time =dat[22];
  R6vent_time=dat[23];
  R6vent_elapsedTime=dat[24];
  R6hold_time =dat[25];
  R6hold_elapsedTime=dat[26];
  R6CompDo=dat[27];
  R6HumDo=dat[28];
  R6DampDo=dat[29];
  R6SolDo=dat[30];

  R7Temp=dat[31];
  R7Hum =dat[32];
  R7Co2=dat[33];
  R7Ethylene=dat[34];
  R7pulpTemp=dat[35];
  R7temp_high=dat[36];
  R7temp_low=dat[37];
  R7hum_high=dat[38];
  R7hum_low=dat[39];
  R7co2_high=dat[40];
  R7co2_low=dat[41];
  R7ethylene_high=dat[42];
  R7ethylene_low=dat[43];
  R7curr_stage=dat[44];
  R7set_dose_interval =dat[45];
  R7set_lockcycle=dat[46];
  R7curr_dose_interval=dat[47];
  R7curr_lockcycle=dat[48];
  R7precool_time =dat[49];
  R7precool_elapsed_time =dat[50];
  R7dose_time =dat[51];
  R7dose_elapsed_time =dat[52];
  R7vent_time=dat[53];
  R7vent_elapsedTime=dat[54];
  R7hold_time =dat[55];
  R7hold_elapsedTime=dat[56];
  R7CompDo=dat[57];
  R7HumDo=dat[58];
  R7DampDo=dat[59];
  R7SolDo=dat[60];

  R8Temp=dat[61];
  R8Hum =dat[62];
  R8Co2=dat[63];
  R8Ethylene=dat[64];
  R8pulpTemp=dat[65];
  R8temp_high=dat[66];
  R8temp_low=dat[67];
  R8hum_high=dat[68];
  R8hum_low=dat[69];
  R8co2_high=dat[70];
  R8co2_low=dat[71];
  R8ethylene_high=dat[72];
  R8ethylene_low=dat[73];
  R8curr_stage=dat[74];
  R8set_dose_interval =dat[75];
  R8set_lockcycle=dat[76];
  R8curr_dose_interval=dat[77];
  R8curr_lockcycle=dat[78];
  R8precool_time =dat[79];
  R8precool_elapsed_time =dat[80];
  R8dose_time =dat[81];
  R8dose_elapsed_time =dat[82];
  R8vent_time=dat[83];
  R8vent_elapsedTime=dat[84];
  R8hold_time =dat[85];
  R8hold_elapsedTime=dat[86];
  R8CompDo=dat[87];
  R8HumDo=dat[88];
  R8DampDo=dat[89];
  R8SolDo=dat[90];

  R9Temp=dat[91];
  R9Hum =dat[92];
  R9Co2=dat[93];
  R9Ethylene=dat[94];
  R9pulpTemp=dat[95];
  R9temp_high=dat[96];
  R9temp_low=dat[97];
  R9hum_high=dat[98];
  R9hum_low=dat[99];
  R9co2_high=dat[100];
  R9co2_low=dat[101];
  R9ethylene_high=dat[102];
  R9ethylene_low=dat[103];
  R9curr_stage=dat[104];
  R9set_dose_interval =dat[105];
  R9set_lockcycle=dat[106];
  R9curr_dose_interval=dat[107];
  R9curr_lockcycle=dat[108];
  R9precool_time =dat[109];
  R9precool_elapsed_time =dat[110];
  R9dose_time =dat[111];
  R9dose_elapsed_time =dat[112];
  R9vent_time=dat[113];
  R9vent_elapsedTime=dat[114];
  R9hold_time =dat[115];
  R9hold_elapsedTime=dat[116];
  R9CompDo=dat[117];
  R9HumDo=dat[118];
  R9DampDo=dat[119];
  R9SolDo=dat[120];
 
   unsigned long currentMillis = millis();
  // Send data every 5 minutes
  if (currentMillis - previousMillis >= interval) 
 {
    previousMillis = currentMillis;
    // Send data for Room 1
    spreadsheet_comm1(R6Temp, R6Hum, R6Co2, R6Ethylene, R6pulpTemp, R6temp_high, R6temp_low, R6hum_high, R6hum_low, R6co2_high, R6co2_low, R6ethylene_high, R6ethylene_low, R6curr_stage, R6set_dose_interval, R6set_lockcycle, R6curr_dose_interval, R6curr_lockcycle, R6precool_time, R6precool_elapsed_time, R6dose_time, R6dose_elapsed_time, R6vent_time, R6vent_elapsedTime, R6hold_time, R6hold_elapsedTime, R6CompDo, R6HumDo, R6DampDo, R6SolDo, R7Temp, R7Hum, R7Co2, R7Ethylene, R7pulpTemp, R7temp_high, R7temp_low, R7hum_high, R7hum_low, R7co2_high, R7co2_low, R7ethylene_high, R7ethylene_low, R7curr_stage, R7set_dose_interval, R7set_lockcycle, R7curr_dose_interval, R7curr_lockcycle, R7precool_time, R7precool_elapsed_time, R7dose_time, R7dose_elapsed_time, R7vent_time, R7vent_elapsedTime, R7hold_time, R7hold_elapsedTime, R7CompDo, R7HumDo, R7DampDo, R7SolDo, R8Temp, R8Hum, R8Co2, R8Ethylene, R8pulpTemp, R8temp_high, R8temp_low, R8hum_high, R8hum_low, R8co2_high, R8co2_low, R8ethylene_high, R8ethylene_low, R8curr_stage, R8set_dose_interval, R8set_lockcycle, R8curr_dose_interval, R8curr_lockcycle, R8precool_time, R8precool_elapsed_time, R8dose_time, R8dose_elapsed_time, R8vent_time, R8vent_elapsedTime, R8hold_time, R8hold_elapsedTime, R8CompDo, R8HumDo, R8DampDo, R8SolDo, R9Temp, R9Hum, R9Co2, R9Ethylene, R9pulpTemp, R9temp_high, R9temp_low, R9hum_high, R9hum_low, R9co2_high, R9co2_low, R9ethylene_high, R9ethylene_low, R9curr_stage, R9set_dose_interval, R9set_lockcycle, R9curr_dose_interval, R9curr_lockcycle, R9precool_time, R9precool_elapsed_time, R9dose_time, R9dose_elapsed_time, R9vent_time, R9vent_elapsedTime, R9hold_time, R9hold_elapsedTime, R9CompDo, R9HumDo, R9DampDo, R9SolDo);
}
}
