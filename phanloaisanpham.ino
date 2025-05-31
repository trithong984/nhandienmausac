#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

#define SERVO_1_PIN 13
#define SERVO_2_PIN 14

#define MOTOR_IN1 17
#define MOTOR_IN2 16
#define MOTOR_EN 5

#define HONG_NGOAI_PIN1 25
#define HONG_NGOAI_PIN2 26
// #define LED_PIN 12


Servo gServo1;
Servo gServo2;

const char* ssid = "AndroidAP495A";
const char* password = "05012004";

WebServer server(80);

String lastColor = "";  // Lưu lại màu vừa nhận

// Hàm điều khiển băng chuyền chạy
void bangchuyenchay(int speed) {
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  analogWrite(MOTOR_EN, speed);
}

// Hàm dừng băng chuyền
void bangchuyendung() {
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  analogWrite(MOTOR_EN, 220);
}

void setup() {
  Serial.begin(115200);

  // Kết nối WiFi
  WiFi.begin(ssid, password);
  Serial.print("Đang kết nối WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("ESP32 đã kết nối, IP: ");
  Serial.println(WiFi.localIP());

  // Cấu hình động cơ
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_EN, OUTPUT);
  bangchuyenchay(360);  

  // Cấu hình cảm biến
  pinMode(HONG_NGOAI_PIN1, INPUT);
   pinMode(HONG_NGOAI_PIN2, INPUT);
//   pinMode(LED_PIN, OUTPUT);
// digitalWrite(LED_PIN, HIGH);
  // Cấu hình servo
  gServo1.setPeriodHertz(50);
  gServo1.attach(SERVO_1_PIN);
  gServo1.write(35);  // Vị trí mặc định

  gServo2.setPeriodHertz(50);
  gServo2.attach(SERVO_2_PIN);
  gServo2.write(140);  // Vị trí mặc định

  // Xử lý HTTP từ Python gửi đến
  server.on("/color", HTTP_GET, []() {
    String color = server.arg("name");
    color.trim();
    lastColor = color;  // Cập nhật màu cuối cùng

    Serial.println("Nhận màu từ Python: " + color);
    server.send(200, "text/plain", "Đã nhận màu: " + color);
  });

  server.begin();
  Serial.println("Web server đã khởi động");
}

void loop() {
  server.handleClient();

  int irValue1 = digitalRead(HONG_NGOAI_PIN1);
int irValue2 = digitalRead(HONG_NGOAI_PIN2);
    if (lastColor == "ORANGE") {
      if (irValue1 == LOW) {
      delay(800);
      bangchuyendung();
      gServo1.write(80);
      delay(1000);
      gServo1.write(35);
      bangchuyenchay(360); 
      } // Khởi động lại băng chuyền sau xử lý
    } else if (lastColor == "BLUE") {
      if (irValue2 == LOW) {
      delay(750);
      bangchuyendung();
      gServo2.write(230);
      delay(1000);
      gServo2.write(140);
      bangchuyenchay(360);  // Khởi động lại
    }  // Tránh xử lý liên tục
  }
  delay(500);
}
