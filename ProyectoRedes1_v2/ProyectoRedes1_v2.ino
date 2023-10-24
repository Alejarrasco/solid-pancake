#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_MLX90614.h>
#include <SPIFFS.h>
#include "config.h"
#include "ESP32_MailClient.h"
#include "indexhtml.h"
#include "mailhtml.h"

String Recipient_email = "miguel.carrasco@ucb.edu.bo";
String enableEmailChecked = "checked";
String Notification_enabled = "true";
String Threshold_temp = "30";
String last_temperature;

int tolerance = 0;
#define TOLERANCE_THRESHOLD 10

float historic[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
int h = 10;

void addHistoric(float newval, int n){
  for(int i=0; i<n-1; i++){
    historic[i] = historic[i+1];
  }
  historic[n-1] = newval;
}


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

AsyncWebServer server(80);

String processor(const String& var){
  
  if(var == "TEMPERATURE"){
    return last_temperature;
  }
  else if(var == "EMAIL_INPUT"){
    return Recipient_email;
  }
  else if(var == "ENABLE_EMAIL"){
    return enableEmailChecked;
  }
  else if(var == "THRESHOLD"){
    return Threshold_temp;
  }
  return String();
}

bool Email_sent = false;

const char* input_parameter1 = "email_input";
const char* input_parameter2 = "enable_email_input";
const char* input_parameter3 = "threshold_input";

unsigned long previousMillis = 0;     
const long interval = 5000;    //after every 10 seconds new sensor readings will be updated 

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

SMTPData smtpData;

void setup() {
    Serial.begin(115200);

    if(!SPIFFS.begin())
     { Serial.println("ha ocurrido un error al montar SPIFFS");
       return; }
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Connecting...");
      return;
    }
    Serial.println();
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    if (!mlx.begin()) {
      Serial.println("Error connecting to MLX sensor. Check wiring.");
      while (1);
    };
  
    Serial.print("Emissivity = "); Serial.println(mlx.readEmissivity());
    Serial.println("================================================");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", index_html, processor);
    });

    server.on("/chart", HTTP_GET, [](AsyncWebServerRequest *request){
      String output = "";
      for (int i = 0; i < h; i++) {
        output += String(historic[i]) + ",";
      }
      Serial.println(output);
      request->send(200, "text/plain", output);
    });

    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {

      if (request->hasParam(input_parameter1)) {
        Recipient_email = request->getParam(input_parameter1)->value();
        if (request->hasParam(input_parameter2)) {
          Notification_enabled = request->getParam(input_parameter2)->value();
          enableEmailChecked = "checked";
        } else {
          Notification_enabled = "false";
          enableEmailChecked = "";
        }
        if (request->hasParam(input_parameter3)) {
          Threshold_temp = request->getParam(input_parameter3)->value();
        }
      } else {
        Recipient_email = "No message sent";
      }
      Serial.println(Recipient_email);
      Serial.println(Notification_enabled);
      Serial.println(Threshold_temp);
      request->send(200, "text/html", "HTTP GET request sent to your ESP.<br><a href=\"/\">Return to Home Page</a>");
    });
    server.onNotFound(notFound);
    server.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    float amb_temperature = mlx.readAmbientTempC();
    float obj_temperature = mlx.readObjectTempC();
    Serial.print("Ambient = "); Serial.print(amb_temperature);
    Serial.print("*C\tObject = "); Serial.print(obj_temperature); Serial.println("*C");

    addHistoric(obj_temperature, h);
    last_temperature = String(obj_temperature);

    if(obj_temperature > Threshold_temp.toFloat()){
      tolerance++;
      
      if (tolerance >= TOLERANCE_THRESHOLD && Notification_enabled == "true" && !Email_sent){
        String emailMessage = String("Temperature above threshold. Current temperature: ") + 
                            String(obj_temperature) + String("C");
      
      
        if(sendEmailNotification(emailMessage)) {
          Serial.println(emailMessage);
          Email_sent = true;
        }
        else {
          Serial.println("Email failed to send");
        } 
      }
         
    } else if(obj_temperature < Threshold_temp.toFloat()) {
      if (tolerance > 0) tolerance--;
      else if (tolerance == 0) Email_sent = false;
    }
    
  }

}

bool sendEmailNotification(String emailMessage){
  smtpData.setLogin(smtpServer, smtpPort, smtpUsername, smtpPassword);

  smtpData.setSender("ESP32", smtpUsername);
  smtpData.setPriority("High");
  smtpData.setSubject(email_subject);
  smtpData.setMessage(makeEmailContent(historic, h, Threshold_temp, Recipient_email), true);
  smtpData.addRecipient(Recipient_email);

  if (!MailClient.sendMail(smtpData)) {
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
    return false;
  }
  smtpData.empty();
  return true;
}
