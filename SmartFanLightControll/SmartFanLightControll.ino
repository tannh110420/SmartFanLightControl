#include <DHT.h>

#define DHTPIN 7
#define DHTTYPE DHT22
#define LED1 5
#define FAN1 6
#define LIGHT_SENSOR_PIN A0  

const float TEMP_THRESHOLD = 35.0;

bool lightAutoMode = false;
bool fanAutoMode = false;

int lightLevel = 0;  
int fanLevel = 0;    

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(FAN1, OUTPUT);
  Serial.begin(9600);
  dht.begin();
  analogWrite(LED1, 0);
  analogWrite(FAN1, 0);

  Serial.println("System Ready.");
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    // --------- LED thủ công ---------
    if (cmd == "LED1_ON") {
      if (!lightAutoMode) {
        lightLevel = 192;
        analogWrite(LED1, lightLevel);
      } //else {
      //   Serial.println("LED1 đang ở chế độ tự động, không thể điều khiển thủ công.");
      // }
    }
    else if (cmd == "LED1_OFF") {
      if (!lightAutoMode) {
        lightLevel = 0;
        analogWrite(LED1, 0);
      } //else {
        //Serial.println("LED1 đang ở chế độ tự động, không thể điều khiển thủ công.");
      //}
    }
    else if (cmd == "LED1_UP") {
      if (!lightAutoMode) {
        if (lightLevel > 0 && lightLevel < 255) {
          lightLevel = min(lightLevel + 64, 255);
          if (lightLevel < 1) lightLevel = 1;
          analogWrite(LED1, lightLevel);
        }
      } //else {
        //Serial.println("LED1 đang ở chế độ tự động, không thể điều khiển thủ công.");
      //}
    }
    else if (cmd == "LED1_DOWN") {
      if (!lightAutoMode) {
        if (lightLevel > 1) {
          lightLevel = max(lightLevel - 64, 1);
          analogWrite(LED1, lightLevel);
        }
      } //else {
        //Serial.println("LED1 đang ở chế độ tự động, không thể điều khiển thủ công.");
      //}
    }

    // --------- Quạt thủ công ---------
    else if (cmd == "FAN1_ON") {
      if (!fanAutoMode) {
        fanLevel = 192;
        analogWrite(FAN1, fanLevel);
      } //else {
        //Serial.println("FAN1 đang ở chế độ tự động, không thể điều khiển thủ công.");
      //}
    }
    else if (cmd == "FAN1_OFF") {
      if (!fanAutoMode) {
        fanLevel = 0;
        analogWrite(FAN1, 0);
      } //else {
        //Serial.println("FAN1 đang ở chế độ tự động, không thể điều khiển thủ công.");
      //}
    }
    else if (cmd == "FAN1_UP") {
      if (!fanAutoMode) {
        if (fanLevel > 0 && fanLevel < 255) {
          fanLevel = min(fanLevel + 64, 255);
          if (fanLevel < 1) fanLevel = 1;
          analogWrite(FAN1, fanLevel);
        }
      } //else {
        //Serial.println("FAN1 đang ở chế độ tự động, không thể điều khiển thủ công.");
      //}
    }
    else if (cmd == "FAN1_DOWN") {
      if (!fanAutoMode) {
        if (fanLevel > 1) {
          fanLevel = max(fanLevel - 64, 1);
          analogWrite(FAN1, fanLevel);
        }
      } //else {
        //Serial.println("FAN1 đang ở chế độ tự động, không thể điều khiển thủ công.");
      //}
    }

    // --------- Auto mode ---------
    else if (cmd == "FAN1_AUTO_ON") {
      fanAutoMode = true;
      Serial.println("FAN1: Chế độ tự động đã bật.");
    }
    else if (cmd == "FAN1_AUTO_OFF") {
      fanAutoMode = false;
      fanLevel = 0;
      analogWrite(FAN1, 0);
      Serial.println("FAN1: Chế độ tự động đã tắt.");
    }

    else if (cmd == "LED1_AUTO_ON") {
      lightAutoMode = true;
      Serial.println("LED1: Chế độ tự động đã bật.");
    }
    else if (cmd == "LED1_AUTO_OFF") {
      lightAutoMode = false;
      lightLevel = 0;
      analogWrite(LED1, 0);
      Serial.println("LED1: Chế độ tự động đã tắt.");
    }

    // Bật/tắt tất cả 
    else if (cmd == "ALL_ON") {
      lightAutoMode = false;
      fanAutoMode = false;
      lightLevel = 192;
      fanLevel = 192;
      analogWrite(LED1, lightLevel);
      analogWrite(FAN1, fanLevel);
      Serial.println("Tất cả thiết bị đã bật ở chế độ thủ công.");
    }
    else if (cmd == "ALL_OFF") {
      lightAutoMode = false;
      fanAutoMode = false;
      lightLevel = 0;
      fanLevel = 0;
      analogWrite(LED1, 0);
      analogWrite(FAN1, 0);
      Serial.println("Tất cả thiết bị đã tắt.");
    }
  }

  // Tự động điều khiển quạt theo nhiệt độ 
  if (fanAutoMode) {
    float temp = dht.readTemperature();

    if (!isnan(temp)) {
      Serial.print("Nhiệt độ: ");
      Serial.print(temp);
      Serial.println("°C");

      if (temp >= TEMP_THRESHOLD) {
        analogWrite(FAN1, 255);
      } else {
        analogWrite(FAN1, 0);
      }
    } else {
      Serial.println("Không đọc được cảm biến DHT!");
    }
  }

  // Tự động điều khiển đèn theo ánh sáng
  if (lightAutoMode) {
    int cdsValue = analogRead(LIGHT_SENSOR_PIN); // Giá trị từ 0 (sáng) đến 1023 (tối)

    Serial.print("Độ sáng (A0): ");
    Serial.println(cdsValue);

    if (cdsValue > 800) {
      analogWrite(LED1, 255);  // Trời tối, đèn sáng mạnh
    }
    else if (cdsValue > 500) {
      analogWrite(LED1, 128);  // Ánh sáng yếu, đèn sáng vừa
    }
    else {
      analogWrite(LED1, 0);    // Trời sáng, đèn tắt
    }
  }

  delay(2000);
}
