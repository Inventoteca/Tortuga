/*
  Check the new incoming messages, and print via serialin 115200 baud rate.

  by Aaron.Lee from HelTec AutoMation, ChengDu, China
  成都惠利特自动化科技有限公司
  www.heltec.cn

  this project also realess in GitHub:
  https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
*/

#include "heltec.h"
#include "Adafruit_VL53L0X.h"

#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6
#define M_DER_PWM   22
#define M_DER_A     23
#define M_DER_B     17

#define M_IZQ_PWM   25
#define M_IZQ_A     12
#define M_IZQ_B     13


int pwm_izq;
int pwm_der;


Adafruit_VL53L0X lox = Adafruit_VL53L0X();


void setup()
{
  //WIFI Kit series V1 not support Vext control
  //Serial.begin(15200);
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
  Serial.println("UTXJ Receiver");
  pinMode(M_DER_PWM, OUTPUT);
  pinMode(M_DER_A, OUTPUT);
  pinMode(M_DER_B, OUTPUT);
  //analogWrite(M_DER_PWM, 0);
  digitalWrite(M_DER_A, LOW);
  digitalWrite(M_DER_B, LOW);

  pinMode(M_IZQ_PWM, OUTPUT);
  pinMode(M_IZQ_A, OUTPUT);
  pinMode(M_IZQ_B, OUTPUT);
  //analogWrite(M_DER_PWM, 0);
  digitalWrite(M_IZQ_A, LOW);
  digitalWrite(M_IZQ_B, LOW);
}

void loop()
{

  VL53L0X_RangingMeasurementData_t measure;

  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.println(measure.RangeMilliMeter);
    
    delay(50);
  }

  // Leer datos enviados por LoRa
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    String data = "";
    while (LoRa.available())
    {
      data += (char)LoRa.read();
    }

    // Convertir la cadena recibida en dos valores enteros
    int commaIndex = data.indexOf(',');
    String izqString = data.substring(0, commaIndex);
    String derString = data.substring(commaIndex + 1);

    pwm_izq = izqString.toInt();
    pwm_der = derString.toInt();
    Serial.print(pwm_izq);
    Serial.print("\t");
    Serial.print(pwm_der);
    Serial.println("\t");

    // Mostrar valores en la pantalla OLED en columnas
    Heltec.display->clear();
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
    Heltec.display->setFont(ArialMT_Plain_16);

    // Imprimir valores de X y Y en columnas
    Heltec.display->drawString(0, 0, String(pwm_izq));
    Heltec.display->drawString(64, 0, String(pwm_der));
    Heltec.display->drawString(0, 30, String(measure.RangeMilliMeter));

    Heltec.display->display();

    // String sensorData = String(measure.RangeMilliMeter);

  }




  motor_der();
  motor_izq();

  
  //delay(2000);
  //String sensorData = String(measure.RangeMilliMeter) + "," + String(0);
  //LoRa.beginPacket();
  //LoRa.print(sensorData);
  //LoRa.endPacket();


}


// ------------------------------------ motor_der
void motor_der()
{


  if (pwm_der > 1 )
  {
    digitalWrite(M_DER_A, LOW);
    digitalWrite(M_DER_B, HIGH);
    analogWrite(M_DER_PWM, abs(pwm_der));
    //digitalWrite(M_DER_PWM, HIGH);
    Serial.println("DER >");
  }
  else if (pwm_der < -1 )
  {
    digitalWrite(M_DER_A, HIGH);
    digitalWrite(M_DER_B, LOW);
    analogWrite(M_DER_PWM, abs(pwm_der));
    //digitalWrite(M_DER_PWM, HIGH);
    Serial.println("DER <");
  }
  else
  {
    analogWrite(M_DER_PWM, 0);
    //digitalWrite(M_DER_PWM,LOW);
    Serial.println(" - DER -");
    digitalWrite(M_DER_A, LOW);
    digitalWrite(M_DER_B, LOW);
  }


}

// ------------------------------------ motor_izq
void motor_izq()
{


  if (pwm_izq > 1 )
  {
    digitalWrite(M_IZQ_A, LOW);
    digitalWrite(M_IZQ_B, HIGH);
    analogWrite(M_IZQ_PWM, abs(pwm_izq));
    //digitalWrite(M_DER_PWM, HIGH);
    Serial.println("IZQ >");
  }
  else if (pwm_izq < -1 )
  {
    digitalWrite(M_IZQ_A, HIGH);
    digitalWrite(M_IZQ_B, LOW);
    analogWrite(M_IZQ_PWM, abs(pwm_izq));
    //digitalWrite(M_DER_PWM, HIGH);
    Serial.println("IZQ <");
  }
  else
  {
    analogWrite(M_IZQ_PWM, 0);
    //digitalWrite(M_DER_PWM,LOW);
    Serial.println(" - IZQ -");
    digitalWrite(M_IZQ_A, LOW);
    digitalWrite(M_IZQ_B, LOW);
  }


}
