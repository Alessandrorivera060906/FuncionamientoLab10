# Laboratorio 10 - Arquitectura del Computador

Sistema de control de movimiento con joystick: un servo motor y un motor stepper,
con bloqueo de seguridad por sensor de vibración.

## Componentes
- Arduino UNO
- Joystick HW-504
- Servo motor SG90
- Motor stepper 28BYJ-48 + driver ULN2003
- Sensor de vibración SW-520D
- LED + resistencia 220Ω

## Mapa de pines
| Componente              | Pin Arduino |
|--------------------------|-------------|
| Joystick VRx             | A0          |
| Joystick VRy             | A1          |
| Joystick SW (botón)      | D2          |
| Sensor de vibración      | D3          |
| LED de bloqueo           | D8          |
| Servo (señal)            | D9          |
| ULN2003 IN1              | D4          |
| ULN2003 IN2              | D5          |
| ULN2003 IN3              | D6          |
| ULN2003 IN4              | D7          |

## Funcionamiento
- El eje X del joystick controla el ángulo del servo (0°-180°, centro = 90°).
- El eje Y del joystick controla el sentido y la velocidad del motor stepper
  (entre más lejos del centro, más rápido gira).
- Al detectar un movimiento brusco con el sensor de vibración, el sistema se
  bloquea: el stepper se detiene, el servo queda fijo en su posición y se
  enciende el LED. El joystick deja de tener efecto.
- Presionando el botón del joystick se desbloquea el sistema (se apaga el LED
  y vuelve a responder normalmente).

## Integrantes
- María Inés Leiva Casiano [1089524]
- Javier Alessandro Rivera Lemus [1241224]
- Jennifer Fernanda Turcios Estrada [1088724]
