#include <ReticSchedule.h>
#include "htmlpages.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

AsyncWebSocket ws("/ws");

ReticSchedule rs(14, 12, 5, 10, 15);

const char *ssid = "retic";

Times savedTimes{10, 10, 10, 10};

DynamicJsonDocument doc(512);

IPAddress local_IP(10, 1, 1, 1);
IPAddress gateway(10, 1, 1, 1);
IPAddress subnet(255, 255, 255, 0);

void notifyClients()
{
  ws.textAll("Toggle Light");
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len, AsyncWebSocketClient *client)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    DeserializationError err = deserializeJson(doc, data);
    if (err)
    {
      Serial.println("deserialization failed");
      return;
    }
    else
    {
      const char *button = doc["toggle"];
      Serial.print("Button number:");
      Serial.println(String(button));
      rs.toggle(button, &savedTimes);
      sendDataWs(client);
      // Create a function to toggle the correct station in ReticSchedule and use it here
    }
  }
}

void sendDataWs(AsyncWebSocketClient *client)
{
  DynamicJsonDocument jsonBuffer(512);
  JsonObject root = jsonBuffer.to<JsonObject>();
  root["s1"] = rs.checkStationStatus(1);
  root["s2"] = rs.checkStationStatus(2);
  root["s3"] = rs.checkStationStatus(3);
  root["s4"] = rs.checkStationStatus(4);
  root["s1name"] = "Driveway";
  root["s2name"] = "Front Garden";
  root["s3name"] = "Pool Area";
  root["s4name"] = "Side Garden";
  root["s1time"] = savedTimes.s1;
  root["s2time"] = savedTimes.s2;
  root["s3time"] = savedTimes.s3;
  root["s4time"] = savedTimes.s4;

  String output;
  if (true)
  {
    serializeJson(root, output);
    ws.textAll(output);
  }
}

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
               void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
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

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "test/plain", "Not found");
}
void setup()
{
  Serial.begin(115200);
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid) ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", index_html); });

  server.onNotFound(notFound);
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  server.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:
  rs.loop();
}
