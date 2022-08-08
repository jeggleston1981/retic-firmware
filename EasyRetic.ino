#include <ReticSchedule.h>
#include "htmlpages.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>


AsyncWebServer server(80);

AsyncWebSocket ws("/ws");

ReticSchedule rs(14, 12, 5, 10, 15);

const char* ssid = "retic";

struct Config {
  int s1time;
  int s2time;
  int s3time;
  int s4time;
};

Config config{1,2,1,2};

DynamicJsonDocument doc(512);

IPAddress local_IP(192,168,4,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

void notifyClients() {
  ws.textAll("Toggle Light");
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len, AsyncWebSocketClient *client) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    DeserializationError err = deserializeJson(doc, data);
    if(err){
      Serial.println("deserialization failed");
      return;
    }else{
      const char * button = doc["toggle"];
      Serial.print("Button number:");
      Serial.println(String(button));
      sendDataWs(client);
      // Create a function to toggle the correct station in ReticSchedule and use it here
    }
    //client->text('{"message" : "ok"}');
  }
}

void sendDataWs(AsyncWebSocketClient * client)
{
    DynamicJsonDocument jsonBuffer(512);
    JsonObject root = jsonBuffer.to<JsonObject>();
    root["s1"] = 1;
    root["s2"] = 0;
    root["s3"] = 0;
    root["s4"] = 0;
    root["s1name"] = "Driveway";
    root["s2name"] = "Front Garden";
    root["s3name"] = "Pool Area";
    root["s4name"] = "Side Garden";
    root["s1time"] = 10;
    root["s2time"] = 5;
    root["s3time"] = 10;
    root["s4time"] = 10;

    String output;
    //AsyncWebSocketMessageBuffer * buffer = ws.makeBuffer(len); //  creates a buffer (len + 1) for you.
    if (true) {
        serializeJson(root, output);
        ws.textAll(output);
    }
}

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len, client);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void notFound(AsyncWebServerRequest *request){
  request->send(404, "test/plain", "Not found");
}
void setup() {
  Serial.begin(115200);
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid) ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/html", index_html);
  });

  server.onNotFound(notFound);
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  server.begin();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  rs.loop();
}
