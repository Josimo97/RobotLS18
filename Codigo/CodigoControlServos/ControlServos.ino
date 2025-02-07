#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Crear el objeto para el controlador PCA9685
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Configuración de los servos
#define SERVO_MIN 150 // Pulso mínimo para el servo
#define SERVO_MAX 600 // Pulso máximo para el servo

#define SERVO_ANGLE_MIN 0   // Ángulo mínimo del servo
#define SERVO_ANGLE_MAX 180 // Ángulo máximo del servo

// Canales de los servos
#define SERVO_CHANNEL_0 0   // BASE PIE derecha
#define SERVO_CHANNEL_1 1   // RODILLA izquierda
#define SERVO_CHANNEL_2 2   // PIERNA derecho
#define SERVO_CHANNEL_4 4   // Codo izquierdo
#define SERVO_CHANNEL_5 5   // Pierna derecha
#define SERVO_CHANNEL_6 6   // Pierna izquierda
#define SERVO_CHANNEL_8 8   // Brazo derecho
#define SERVO_CHANNEL_9 9   // Brazo izquierdo
#define SERVO_CHANNEL_10 10 // Pierna derecha
#define SERVO_CHANNEL_12 12 // Pierna izquierda
#define SERVO_CHANNEL_13 13 // Brazo derecho
#define SERVO_CHANNEL_14 14 // Brazo izquierdo

void setup()
{
    Serial.begin(115200); // Inicializar comunicación serial
    pwm.begin();          // Inicializar el controlador PCA9685
    pwm.setPWMFreq(50);   // Configurar frecuencia del PCA9685 a 50 Hz
}

// Función para convertir ángulos en pulsos de PWM
uint16_t angleToPulse(int angle)
{
    return map(angle, SERVO_ANGLE_MIN, SERVO_ANGLE_MAX, SERVO_MIN, SERVO_MAX);
}

void loop()
{

    pwm.setPWM(SERVO_CHANNEL_0, 0, angleToPulse(90));
    pwm.setPWM(SERVO_CHANNEL_1, 0, angleToPulse(90));
    pwm.setPWM(SERVO_CHANNEL_2, 0, angleToPulse(90));
    pwm.setPWM(SERVO_CHANNEL_4, 0, angleToPulse(90));
    pwm.setPWM(SERVO_CHANNEL_5, 0, angleToPulse(90));
    pwm.setPWM(SERVO_CHANNEL_6, 0, angleToPulse(90));
    pwm.setPWM(SERVO_CHANNEL_8, 0, angleToPulse(90));
    pwm.setPWM(SERVO_CHANNEL_9, 0, angleToPulse(90));
    pwm.setPWM(SERVO_CHANNEL_10, 0, angleToPulse(90));
    pwm.setPWM(SERVO_CHANNEL_12, 0, angleToPulse(90));
    pwm.setPWM(SERVO_CHANNEL_13, 0, angleToPulse(90));
    pwm.setPWM(SERVO_CHANNEL_14, 0, angleToPulse(90));
    delay(1000); // Esperar 1 segundo

    // Mover servos a posiciones iniciales
    pwm.setPWM(SERVO_CHANNEL_0, 0, angleToPulse(90));
    pwm.setPWM(SERVO_CHANNEL_1, 0, angleToPulse(0));
    pwm.setPWM(SERVO_CHANNEL_2, 0, angleToPulse(90));
    pwm.setPWM(SERVO_CHANNEL_4, 0, angleToPulse(90));
    pwm.setPWM(SERVO_CHANNEL_5, 0, angleToPulse(90));
    pwm.setPWM(SERVO_CHANNEL_6, 0, angleToPulse(90));
    pwm.setPWM(SERVO_CHANNEL_8, 0, angleToPulse(90));
    pwm.setPWM(SERVO_CHANNEL_9, 0, angleToPulse(90));
    pwm.setPWM(SERVO_CHANNEL_10, 0, angleToPulse(90));
    pwm.setPWM(SERVO_CHANNEL_12, 0, angleToPulse(90));
    pwm.setPWM(SERVO_CHANNEL_13, 0, angleToPulse(90));
    pwm.setPWM(SERVO_CHANNEL_14, 0, angleToPulse(90));

    delay(1000); // Esperar 1 segundo
}
