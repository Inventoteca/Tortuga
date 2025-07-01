// vamos a trabajar con el dht22

#define MOTOR_DERECHA_A      12  //Izquierdo
#define MOTOR_DERECHA_B      13  //Derecho
#define MOTOR_DERECHA_E      14
#define MOTOR_IZQUIERDA_A    15  //Izquierdo
#define MOTOR_IZQUIERDA_B    18  //Derecho
#define MOTOR_IZQUIERDA_E    19 
#define NEUTRAL              2048  //1900
#define HISTERESIS           100
#define MINPWM               100
#define VUELTACOMPLETA       200

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

  digitalWrite(MOTOR_IZQUIERDA_E, HIGH);
  digitalWrite(MOTOR_DERECHA_E, HIGH);

  //Map pwm eje x y y
  pwm_y=map(y,4095,0,-255,255);
  pwm_x=map(x,0,4095,-255,255);

  Serial.print("Valor X: ");
  Serial.print(x);
  Serial.print("\t");
  Serial.print("Valor Y: ");
  Serial.print(y);
  Serial.print("\t");
  Serial.print(pwm_x);
  Serial.print("\t");
  Serial.print(pwm_y);
  Serial.print("\t");

  if (y<= (NEUTRAL- HISTERESIS))
  {
    digitalWrite(MOTOR_IZQUIERDA_E,HIGH); //Se habilita
    if(abs(pwm_y) > MINPWM){
      Serial.print("Vel_Motores: ");
      Serial.print(abs(pwm_y));
      analogWrite(MOTOR_DERECHA_B,abs(pwm_y));
      analogWrite(MOTOR_IZQUIERDA_A,abs(pwm_y));
    }
    Serial.print("\t");
    Serial.print("Atras");
  }
  else if (y>= (NEUTRAL+ HISTERESIS))
  { 
    if(abs(pwm_y) > MINPWM){
      Serial.print("Vel_Motores: ");
      Serial.print(abs(pwm_y));
      analogWrite(MOTOR_DERECHA_A,abs(pwm_y));
      analogWrite(MOTOR_IZQUIERDA_B,abs(pwm_y));
    }
    Serial.print("\t");
    Serial.print("Adelante");
  }
  else if (x>= (NEUTRAL + HISTERESIS))
  {
    //Serial.print("Izquierda");
    if(abs(pwm_x) > MINPWM && abs(pwm_x) < VUELTACOMPLETA){
      Serial.print("Vel_Motores: ");
      Serial.print(abs(pwm_x));
      analogWrite(MOTOR_DERECHA_A,0);
      analogWrite(MOTOR_IZQUIERDA_B,abs(pwm_x));
      Serial.print("\t");
      Serial.print("Izquierda");
    }
    else if(abs(pwm_x) > VUELTACOMPLETA){
      Serial.print("Vel_Motores: ");
      Serial.print(abs(pwm_x));
      analogWrite(MOTOR_IZQUIERDA_A,abs(pwm_x));
      analogWrite(MOTOR_IZQUIERDA_B,abs(pwm_x));
      Serial.print("\t");
      Serial.print("Izquierda Completa");
    }
  }
  else if (x<= (NEUTRAL - HISTERESIS))
  {
    //Serial.print("Derecha");
    if(abs(pwm_x) > MINPWM && abs(pwm_x) < VUELTACOMPLETA){
      Serial.print("Vel_Motores: ");
      Serial.print(abs(pwm_x));
      analogWrite(MOTOR_DERECHA_A,abs(pwm_x));
      analogWrite(MOTOR_IZQUIERDA_B,0);
      Serial.print("\t");
      Serial.print("Derecha");
    }
    else if(abs(pwm_x) > VUELTACOMPLETA){
      Serial.print("Vel_Motores: ");
      Serial.print(abs(pwm_x));
      analogWrite(MOTOR_DERECHA_A,abs(pwm_x));
      analogWrite(MOTOR_DERECHA_B,abs(pwm_x));
      Serial.print("\t");
      Serial.print("Derecha Completa");
    }
    }
  else
  {
    Serial.print("Vel_Motores: ");
    Serial.print(abs(pwm_y + pwm_x));
    Serial.print("\t");
    analogWrite(MOTOR_DERECHA_A,0);
    analogWrite(MOTOR_DERECHA_B,0);
    analogWrite(MOTOR_IZQUIERDA_A,0);
    analogWrite(MOTOR_IZQUIERDA_B,0);
    Serial.print("Detener");
  }
  Serial.print("\n");


}
