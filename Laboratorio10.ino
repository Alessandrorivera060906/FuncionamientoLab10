#include <Servo.h>

const byte PIN_JOY_X   = A0;
const byte PIN_JOY_Y   = A1;
const byte PIN_JOY_SW  = 2;

const byte PIN_VIBRACION = 3;
const byte PIN_LED_BLOQUEO = 8;

const byte PIN_SERVO = 9;

const byte PIN_STEPPER_IN1 = 4;
const byte PIN_STEPPER_IN2 = 5;
const byte PIN_STEPPER_IN3 = 6;
const byte PIN_STEPPER_IN4 = 7;

const int JOY_CENTRO = 512;
const int ZONA_MUERTA = 60;

const unsigned long INTERVALO_PASO_MIN = 2;
const unsigned long INTERVALO_PASO_MAX = 25;

Servo servoMotor;

volatile bool sistemaBloqueado = false;

int anguloServoActual = 90;

int pasoStepperActual = 0;
unsigned long ultimoPasoStepper = 0;

int estadoBotonAnterior = HIGH;

const byte SECUENCIA_PASOS[8][4] = {
  {1, 0, 0, 0},
  {1, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 0, 0, 1},
  {1, 0, 0, 1}
};

void setup()
{
  pinMode(PIN_JOY_SW, INPUT_PULLUP);
  pinMode(PIN_VIBRACION, INPUT_PULLUP);
  pinMode(PIN_LED_BLOQUEO, OUTPUT);

  pinMode(PIN_STEPPER_IN1, OUTPUT);
  pinMode(PIN_STEPPER_IN2, OUTPUT);
  pinMode(PIN_STEPPER_IN3, OUTPUT);
  pinMode(PIN_STEPPER_IN4, OUTPUT);

  digitalWrite(PIN_LED_BLOQUEO, LOW);

  servoMotor.attach(PIN_SERVO);
  servoMotor.write(anguloServoActual);

  attachInterrupt(digitalPinToInterrupt(PIN_VIBRACION), onVibracionDetectada, RISING);
}

void loop()
{
  manejarBotonDesbloqueo();

  if (sistemaBloqueado)
  {
    detenerStepper();
    return;
  }

  controlarServo();
  controlarStepper();
}

void onVibracionDetectada()
{
  if (!sistemaBloqueado)
  {
    sistemaBloqueado = true;
    digitalWrite(PIN_LED_BLOQUEO, HIGH);
  }
}

void manejarBotonDesbloqueo()
{
  int estadoBoton = digitalRead(PIN_JOY_SW);

  if (estadoBoton == LOW && estadoBotonAnterior == HIGH)
  {
    if (sistemaBloqueado)
    {
      sistemaBloqueado = false;
      digitalWrite(PIN_LED_BLOQUEO, LOW);
    }
  }

  estadoBotonAnterior = estadoBoton;
}

void controlarServo()
{
  int valorX = analogRead(PIN_JOY_X);

  if (abs(valorX - JOY_CENTRO) < ZONA_MUERTA)
  {
    anguloServoActual = 90;
  }
  else
  {
    anguloServoActual = map(valorX, 0, 1023, 0, 180);
    anguloServoActual = constrain(anguloServoActual, 0, 180);
  }

  servoMotor.write(anguloServoActual);
}

void controlarStepper()
{
  int valorY = analogRead(PIN_JOY_Y);
  int distanciaCentro = valorY - JOY_CENTRO;

  if (abs(distanciaCentro) < ZONA_MUERTA)
  {
    return;
  }

  int magnitud = constrain(abs(distanciaCentro), ZONA_MUERTA, JOY_CENTRO);
  unsigned long intervalo = map(magnitud, ZONA_MUERTA, JOY_CENTRO,
                                 INTERVALO_PASO_MAX, INTERVALO_PASO_MIN);

  unsigned long ahora = millis();
  if (ahora - ultimoPasoStepper < intervalo)
  {
    return;
  }
  ultimoPasoStepper = ahora;

  if (distanciaCentro > 0)
  {
    pasoStepperActual = (pasoStepperActual + 1) % 8;
  }
  else
  {
    pasoStepperActual = (pasoStepperActual + 7) % 8;
  }

  aplicarPasoStepper(pasoStepperActual);
}

void aplicarPasoStepper(int indice)
{
  digitalWrite(PIN_STEPPER_IN1, SECUENCIA_PASOS[indice][0]);
  digitalWrite(PIN_STEPPER_IN2, SECUENCIA_PASOS[indice][1]);
  digitalWrite(PIN_STEPPER_IN3, SECUENCIA_PASOS[indice][2]);
  digitalWrite(PIN_STEPPER_IN4, SECUENCIA_PASOS[indice][3]);
}

void detenerStepper()
{
  digitalWrite(PIN_STEPPER_IN1, LOW);
  digitalWrite(PIN_STEPPER_IN2, LOW);
  digitalWrite(PIN_STEPPER_IN3, LOW);
  digitalWrite(PIN_STEPPER_IN4, LOW);
}