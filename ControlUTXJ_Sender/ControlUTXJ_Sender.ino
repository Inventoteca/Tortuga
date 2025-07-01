
#include "heltec.h"


#define BAND      915E6  //you can set band here directly,e.g. 868E6,915E6
#define x_com     2044-1843     // Compensacion para el centro del joystick
#define y_com     2044-1871      // Compensacion para el centro del joystick
#define pwm_min   20      // PWM minimo para mover los motores


#define JOYSTICK_X_PIN 12
#define JOYSTICK_Y_PIN 13

int distancia = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("UTXJ Control SENDER ----->>>");

  // Inicializar Heltec OLED
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa enable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  // Configurar pines del joystick
  pinMode(JOYSTICK_X_PIN, INPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);
}

void loop() 
{// Leer datos enviados por LoRa
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    String data_in = "";
    while (LoRa.available())
    {
      data_in += (char)LoRa.read();
    }

    // Convertir la cadena recibida en dos valores enteros
    int commaIndex = data_in.indexOf(',');
    String dist = data_in.substring(0, commaIndex);
    String temp = data_in.substring(commaIndex + 1);

    distancia = dist.toInt();
    Serial.println("LoRa Get");


  }
  
  
  int xValue = analogRead(JOYSTICK_X_PIN) + x_com;  //
  int yValue = analogRead(JOYSTICK_Y_PIN) + y_com;

  // Map values from 0-4095 to -100 to 100
  int xMapped = map(xValue, 0, 4095, -255, 255);
  int yMapped = map(yValue, 0, 4095, -255, 255);


  int pwm_izq = xMapped - yMapped;
  int pwm_der = xMapped + yMapped;

  if (abs(pwm_izq) < pwm_min)
    pwm_izq = 0;
  if (abs(pwm_der) < pwm_min)
    pwm_der = 0;

  if (pwm_izq < -255) // ES negativo
  {
    pwm_izq = -255;
  }
  if (pwm_izq > 255) // ES positivo
  {
    pwm_izq = 255;
  }

  if (pwm_der < -255) // ES negativo
  {
    pwm_der = -255;
  }
  if (pwm_der > 255) // ES positivo
  {
    pwm_der = 255;
  }


  // Mostrar valores en la pantalla OLED en columnas
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_16);

  // Imprimir valores de X y Y en columnas
  Heltec.display->drawString(0, 0, String(pwm_izq));
  Heltec.display->drawString(64, 0, String(pwm_der));
  Heltec.display->drawString(0, 30, String(distancia));

  Heltec.display->display();


  // Imprimir valores en el monitor serial
  Serial.print(xValue);
  Serial.print("\t");
  Serial.print(yValue);
  Serial.print("\t");
  //Serial.print(xMapped);
  //Serial.print("\t");
  //Serial.print(yMapped);
  //Serial.print("\t");
  Serial.print(pwm_izq);
  Serial.print("\t");
  Serial.print(pwm_der);
  Serial.print("\t");
  Serial.print(distancia);
  Serial.println("\t");

  // Enviar valores del joystick por LoRa
  String data = String(pwm_izq) + "," + String(pwm_der) + ",";
  // send packet
  LoRa.beginPacket();
  /*
    LoRa.setTxPower(txPower,RFOUT_pin);
    txPower -- 0 ~ 20
    RFOUT_pin could be RF_PACONFIG_PASELECT_PABOOST or RF_PACONFIG_PASELECT_RFO
      - RF_PACONFIG_PASELECT_PABOOST -- LoRa single output via PABOOST, maximum output 20dBm
      - RF_PACONFIG_PASELECT_RFO     -- LoRa single output via RFO_HF / RFO_LF, maximum output 14dBm
  */
  LoRa.setTxPower(14, RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print(data);
  //LoRa.print(counter);
  LoRa.endPacket();
  //counter++;



  
}
