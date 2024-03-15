#define MOTOR_DERECHA_A      12  //Izquierdo
#define MOTOR_DERECHA_B      13  //Derecho
#define MOTOR_DERECHA_E      14
#define MOTOR_IZQUIERDA_A    15  //Izquierdo
#define MOTOR_IZQUIERDA_B    18  //Derecho
#define MOTOR_IZQUIERDA_E    19 
#define NEUTRAL              2048  //1900
#define HISTERESIS           100
#define MINPWM               100

int pwm_y, pwm_x;

void setup() {
  Serial.begin(115200);
  Serial.println("TORTUGA V1");

  pinMode(MOTOR_DERECHA_A, OUTPUT);
  pinMode(MOTOR_DERECHA_B, OUTPUT);
  pinMode(MOTOR_DERECHA_E, OUTPUT);
  pinMode(MOTOR_IZQUIERDA_A, OUTPUT);
  pinMode(MOTOR_IZQUIERDA_B, OUTPUT);
  pinMode(MOTOR_IZQUIERDA_E, OUTPUT);
}

void loop() {
  int x = analogRead(36); // Valor analógico del eje x del joystick
  int y = analogRead(32); // Valor analógico del eje y del joystick

  pwm_y=map(y,4095,0,-255,255);
  pwm_x=map(x,0,4095,-255,255);
  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.print(pwm_x);
  Serial.print("\t");
  Serial.print(pwm_y);
  Serial.print("\t");

  if (y<= (NEUTRAL- HISTERESIS))
  {
    digitalWrite(MOTOR_IZQUIERDA_E,HIGH); //Se habilita
    if(abs(pwm_y) > 100){
      analogWrite(MOTOR_DERECHA_B,abs(pwm_y));
      analogWrite(MOTOR_IZQUIERDA_A,abs(pwm_y));
    }
    Serial.print("Atras");
    Serial.print("\t");
  }
  else if (y>= (NEUTRAL+ HISTERESIS))
  { 
    Serial.print("Adelante");
    if(abs(pwm_y) > 100){
      analogWrite(MOTOR_DERECHA_A,abs(pwm_y));
      analogWrite(MOTOR_IZQUIERDA_B,abs(pwm_y));
    }
    Serial.print("\t");
  }
  else if (x>= (NEUTRAL + HISTERESIS))
  {
    Serial.print("Izquierda");
    Serial.print("\t");
  }
  else if (x<= (NEUTRAL - HISTERESIS))
  {
    Serial.print("Derecha");
    Serial.print("\t");
    }
  else
  {
    Serial.print("Detener");
    analogWrite(MOTOR_DERECHA_A,0);
    analogWrite(MOTOR_DERECHA_B,0);
    analogWrite(MOTOR_IZQUIERDA_A,0);
    analogWrite(MOTOR_IZQUIERDA_B,0);
    Serial.print("\t");
  }

  Serial.print("\n");


}
