#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <esp_camera.h>

#define CAMERA_MODEL_AI_THINKER // Has PSRAM
// Camera pin definitions for OV2640 module
#define PWDN_GPIO_NUM 17
#define RESET_GPIO_NUM 13
#define XCLK_GPIO_NUM 18
#define SIOD_GPIO_NUM 10
#define SIOC_GPIO_NUM 12
#define Y9_GPIO_NUM 8
#define Y8_GPIO_NUM 4
#define Y7_GPIO_NUM 7
#define Y6_GPIO_NUM 3
#define Y5_GPIO_NUM 6
#define Y4_GPIO_NUM 2
#define Y3_GPIO_NUM 5
#define Y2_GPIO_NUM 1
#define VSYNC_GPIO_NUM 9
#define HREF_GPIO_NUM 11
#define PCLK_GPIO_NUM 14

const char *ssid = "HotSpot";
const char *password = "11111111";

// const char* webSocketUrl = "ws://your-nodejs-server-ip:8080/";
const char *websockets_server_host = "192.168.157.242"; // 192.168.62.242
const uint16_t websockets_server_port = 8080;           // Enter server port

using namespace websockets;

WebsocketsClient client;

void setup()
{
  Serial.begin(115200);
  // Connect to wifi
  WiFi.begin("HotSpot", "11111111");
  Serial.println("Wifi Connecting!");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Wifi Connected with IP: ");
  Serial.println(WiFi.localIP());

  Serial.println("Connecting to server.");
  bool connected = client.connect(websockets_server_host, 8080, "/");
  if (connected)
  {
    Serial.println("WebSocket Connected!");
    // client.send("Hello");
  }
  else
  {
    Serial.println("Not Connected!");
  }

  // run callback when messages are received
  client.onMessage([&](WebsocketsMessage message)
                   {
        Serial.print("Got Message: ");
        Serial.println(message.data());
        if(message.data()=="stop"){
          client.close();
        } });
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 10000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_VGA;
  config.jpeg_quality = 10; // 10
  config.fb_count = 2;      // 1

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  // for larger pre-allocated frame buffer.
  if (psramFound())
  {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
    config.grab_mode = CAMERA_GRAB_LATEST;
  }
  else
  {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK)
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  // OV2640
  if (s->id.PID == OV2640_PID)
  {
    s->set_gain_ctrl(s, 1);     // auto gain on
    s->set_exposure_ctrl(s, 1); // auto exposer on
    s->set_awb_gain(s, 1);
    s->set_saturation(s, 1);
  }

  // if initial sensorsa are filpped vertically and colors are a bit saturated
  if (s->id.PID == OV2640_PID)
  {
    s->set_vflip(s, 1);
    s->set_brightness(s, 1);
    s->set_saturation(s, -2);
  }

  // camera framesize
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
}

void loop()
{
  // let the websockets client check for incoming messages
  if (client.available())
  {
    client.poll();
  }
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb)
  {
    Serial.println("Camera capture failed");
    esp_camera_fb_return(fb);
    return;
  }

  if (fb->format != PIXFORMAT_JPEG)
  {
    Serial.println("Non-JPEG data not implemented");
    return;
  }

  client.sendBinary((const char *)fb->buf, fb->len);
  esp_camera_fb_return(fb);
}
