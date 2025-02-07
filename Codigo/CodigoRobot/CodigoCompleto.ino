#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_SSD1306.h>

// Configuración WiFi
const char* ssid = "ssid";
const char* password = "password";

// Crear el objeto para el controlador PCA9685
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Definición de pines de los servos
const int servoPins[] = { 0, 1, 2, 4, 5, 6, 8, 9, 10, 12, 13, 14 };
const char* servoNames[] = { "Base Pie Derecho", "Rodilla Derecha", "Pierna Derecha", "Mano Derecha", "Codo Derecho", "Brazo Derecho", "Mano Izquierda", "Codo Izquierdo", "Brazo Izquierdo", "Base Pie Izquierdo", "Rodilla Izquierda", "Pierna Izquierda" };

// Variables para almacenar los ángulos actuales de los servos
int servoActuales[12] = { 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90 };

// Función para convertir ángulos en pulsos de PWM
uint16_t angleToPulse(int angle) {
  return map(angle, 0, 180, 150, 600);
}

WebServer server(80);

// Configuración de pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Estado de los ojos
int ref_eye_height = 40;
int ref_eye_width = 40;
int ref_space_between_eye = 10;
int ref_corner_radius = 10;

int left_eye_height = ref_eye_height;
int left_eye_width = ref_eye_width;
int left_eye_x = SCREEN_WIDTH / 2 - ref_eye_width / 2 - ref_space_between_eye / 2;
int left_eye_y = SCREEN_HEIGHT / 2;
int right_eye_x = SCREEN_WIDTH / 2 + ref_eye_width / 2 + ref_space_between_eye / 2;
int right_eye_y = SCREEN_HEIGHT / 2;
int right_eye_height = ref_eye_height;
int right_eye_width = ref_eye_width;

bool isActionRunning = false; 

// Función para dibujar los ojos
void draw_eyes(bool update = true) {
  display.clearDisplay();
  int x = int(left_eye_x - left_eye_width / 2);
  int y = int(left_eye_y - left_eye_height / 2);
  display.fillRoundRect(x, y, left_eye_width, left_eye_height, ref_corner_radius, SSD1306_WHITE);

  x = int(right_eye_x - right_eye_width / 2);
  y = int(right_eye_y - right_eye_height / 2);
  display.fillRoundRect(x, y, right_eye_width, right_eye_height, ref_corner_radius, SSD1306_WHITE);

  if (update) {
    display.display();
  }
}

// Simula el estado de "dormir"
void sleep() {
  left_eye_height = 2;
  right_eye_height = 2;
  draw_eyes(true);
}

// Simula un parpadeo
void blink(int speed = 8) {
  for (int i = 0; i < 3; i++) {
    left_eye_height -= speed;
    right_eye_height -= speed;
    draw_eyes();
    delay(50);
  }
  for (int i = 0; i < 3; i++) {
    left_eye_height += speed;
    right_eye_height += speed;
    draw_eyes();
    delay(50);
  }
}

// Simula el despertar con parpadeo (Expresión Feliz)
void wakeup() {
  isActionRunning = true;
  sleep();
  delay(500);
  for (int h = 0; h <= ref_eye_height; h += 2) {
    left_eye_height = h;
    right_eye_height = h;
    draw_eyes(true);
    delay(20);
  }
  delay(1000);
  isActionRunning = false;
}

// Centra los ojos en la pantalla
void center_eyes(bool update = true) {
  left_eye_x = SCREEN_WIDTH / 2 - ref_eye_width / 2 - ref_space_between_eye / 2;
  left_eye_y = SCREEN_HEIGHT / 2;
  right_eye_x = SCREEN_WIDTH / 2 + ref_eye_width / 2 + ref_space_between_eye / 2;
  right_eye_y = SCREEN_HEIGHT / 2;

  left_eye_height = ref_eye_height;
  left_eye_width = ref_eye_width;
  right_eye_height = ref_eye_height;
  right_eye_width = ref_eye_width;

  draw_eyes(update);
}

// Mueve los ojos hacia la derecha con efecto de parpadeo
void move_right_big_eye() {
  isActionRunning = true;
  int direction_movement_amplitude = 2;
  int blink_amplitude = 5;

  for (int i = 0; i < 3; i++) {
    left_eye_x += direction_movement_amplitude;
    right_eye_x += direction_movement_amplitude;
    right_eye_height -= blink_amplitude;
    left_eye_height -= blink_amplitude;
    draw_eyes();
    delay(50);
  }
  for (int i = 0; i < 3; i++) {
    left_eye_x += direction_movement_amplitude;
    right_eye_x += direction_movement_amplitude;
    right_eye_height += blink_amplitude;
    left_eye_height += blink_amplitude;
    draw_eyes();
    delay(50);
  }

  delay(500);

  for (int i = 0; i < 3; i++) {
    left_eye_x -= direction_movement_amplitude;
    right_eye_x -= direction_movement_amplitude;
    right_eye_height -= blink_amplitude;
    left_eye_height -= blink_amplitude;
    draw_eyes();
    delay(50);
  }
  for (int i = 0; i < 3; i++) {
    left_eye_x -= direction_movement_amplitude;
    right_eye_x -= direction_movement_amplitude;
    right_eye_height += blink_amplitude;
    left_eye_height += blink_amplitude;
    draw_eyes();
    delay(50);
  }

  center_eyes();
  isActionRunning = false;
}

// Mueve los ojos hacia la izquierda con efecto de parpadeo
void move_left_big_eye() {
  isActionRunning = true;
  int direction = -1;
  int direction_oversize = 1;
  int direction_movement_amplitude = 2;
  int blink_amplitude = 5;

  for (int i = 0; i < 3; i++) {
    left_eye_x += direction_movement_amplitude * direction;
    right_eye_x += direction_movement_amplitude * direction;
    right_eye_height -= blink_amplitude;
    left_eye_height -= blink_amplitude;
    left_eye_height += direction_oversize;
    left_eye_width += direction_oversize;

    draw_eyes();
    delay(50);
  }
  for (int i = 0; i < 3; i++) {
    left_eye_x += direction_movement_amplitude * direction;
    right_eye_x += direction_movement_amplitude * direction;
    right_eye_height += blink_amplitude;
    left_eye_height += blink_amplitude;
    left_eye_height += direction_oversize;
    left_eye_width += direction_oversize;

    draw_eyes();
    delay(50);
  }

  delay(500);

  for (int i = 0; i < 3; i++) {
    left_eye_x -= direction_movement_amplitude * direction;
    right_eye_x -= direction_movement_amplitude * direction;
    right_eye_height -= blink_amplitude;
    left_eye_height -= blink_amplitude;
    left_eye_height -= direction_oversize;
    left_eye_width -= direction_oversize;

    draw_eyes();
    delay(50);
  }
  for (int i = 0; i < 3; i++) {
    left_eye_x -= direction_movement_amplitude * direction;
    right_eye_x -= direction_movement_amplitude * direction;
    right_eye_height += blink_amplitude;
    left_eye_height += blink_amplitude;
    left_eye_height -= direction_oversize;
    left_eye_width -= direction_oversize;

    draw_eyes();
    delay(50);
  }

  center_eyes();
  isActionRunning = false;
}

// Animación de movimientos aleatorios de los ojos
void random_eye_movements() {
  center_eyes(true);
  for (int i = 0; i < 20; i++) {
    int dir_x = random(-1, 2);
    int dir_y = random(-1, 2);
    saccade(dir_x, dir_y);
    delay(50);
    saccade(-dir_x, -dir_y);
    delay(50);
  }
}

// Simula movimientos rápidos de los ojos (saccades)
void saccade(int direction_x, int direction_y) {
  int movement_x = 6 * direction_x;
  int movement_y = 4 * direction_y;

  left_eye_x += movement_x;
  right_eye_x += movement_x;
  left_eye_y += movement_y;
  right_eye_y += movement_y;

  draw_eyes();
  delay(100);
}

// Simula la expresión de ojos felices
void happy_eye() {
  draw_eyes(false);
  int offset = ref_eye_height / 2;
  for (int i = 0; i < 10; i++) {
    display.fillTriangle(left_eye_x - ref_eye_width / 2, left_eye_y + offset, left_eye_x + ref_eye_width / 2, left_eye_y + 5 + offset, left_eye_x - ref_eye_width / 2, left_eye_y + ref_eye_height + offset, SSD1306_BLACK);
    display.fillTriangle(right_eye_x + ref_eye_width / 2, right_eye_y + offset, right_eye_x - ref_eye_width / 2, right_eye_y + 5 + offset, right_eye_x + ref_eye_width / 2, right_eye_y + ref_eye_height + offset, SSD1306_BLACK);
    offset -= 2;
    display.display();
    delay(1);
  }
  delay(1000);
}

// Página HTML para el servidor web
String HTML_PAGE = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Control OLED - ESP32</title>
    <style>
        body {
            text-align: center;
            font-family: Arial, sans-serif;
            background-color: #f4f4f4;
            color: #333;
            padding: 20px;
        }
        h2 {
            color: #008CBA;
        }
        .container {
            max-width: 400px;
            margin: auto;
            background: white;
            padding: 20px;
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        }
        button {
            display: block;
            width: 100%;
            padding: 15px;
            font-size: 18px;
            margin: 10px 0;
            border: none;
            cursor: pointer;
            background: #008CBA;
            color: white;
            border-radius: 5px;
            transition: background 0.3s;
        }
        button:hover {
            background: #005f73;
        }
        .status {
            margin-top: 20px;
            font-size: 16px;
            font-weight: bold;
            color: #00796b;
        }
        .container-servo {
            display: flex;
            flex-wrap: wrap;
            justify-content: space-between;
        }
        .column {
            width: 23%;
            background: #fff;
            padding: 15px;
            border-radius: 8px;
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
            margin-bottom: 20px;
        }
        input[type='range'] {
            width: 100%;
        }
    </style>
    <script>
        function sendCommand(command) {
            fetch('/' + command)
            .then(response => response.text())
            .then(data => {
                document.getElementById("status").innerText = "Acción: " + command + " ejecutada";
            });
        }
        function updateServo(index, angle) {
            fetch('/servo?index=' + index + '&angle=' + angle)
            .then(response => response.text())
            .then(data => {
                console.log(data);
            });
        }
    </script>
</head>
<body>
    <div class="container">
        <h2>Control de Gestos OLED - ESP32</h2>
        <button onclick="sendCommand('feliz')">😊 Levantarse</button>
        <button onclick="sendCommand('mover_ojos_derecha')">➡️ Mover Ojos Derecha</button>
        <button onclick="sendCommand('mover_ojos_izquierda')">⬅️ Mover Ojos Izquierda</button>
        <button onclick="sendCommand('movimientos_aleatorios')">🔄 Movimientos Aleatorios</button>
        <button onclick="sendCommand('ojos_felices')">😊 Ojos Felices</button>
        <button onclick="sendCommand('dormir')">😴 Dormir</button>
        <div class="status" id="status">Esperando comando...</div>
    </div>
    <div class="container-servo">
        <div class='column'>
            <h2>Pierna Derecha</h2>
            <p>Base Pie Derecho: <input type='range' min='0' max='180' value='90' onchange='updateServo(0, this.value)'></p>
            <p>Rodilla Derecha: <input type='range' min='0' max='180' value='90' onchange='updateServo(1, this.value)'></p>
            <p>Pierna Derecha: <input type='range' min='0' max='180' value='90' onchange='updateServo(2, this.value)'></p>
        </div>
        <div class='column'>
            <h2>Brazo Derecho</h2>
            <p>Mano Derecha: <input type='range' min='0' max='180' value='90' onchange='updateServo(3, this.value)'></p>
            <p>Codo Derecho: <input type='range' min='0' max='180' value='90' onchange='updateServo(4, this.value)'></p>
            <p>Brazo Derecho: <input type='range' min='0' max='180' value='90' onchange='updateServo(5, this.value)'></p>
        </div>
        <div class='column'>
            <h2>Brazo Izquierdo</h2>
            <p>Mano Izquierda: <input type='range' min='0' max='180' value='90' onchange='updateServo(6, this.value)'></p>
            <p>Codo Izquierdo: <input type='range' min='0' max='180' value='90' onchange='updateServo(7, this.value)'></p>
            <p>Brazo Izquierdo: <input type='range' min='0' max='180' value='90' onchange='updateServo(8, this.value)'></p>
        </div>
        <div class='column'>
            <h2>Pierna Izquierda</h2>
            <p>Base Pie Izquierdo: <input type='range' min='0' max='180' value='90' onchange='updateServo(9, this.value)'></p>
            <p>Rodilla Izquierda: <input type='range' min='0' max='180' value='90' onchange='updateServo(10, this.value)'></p>
            <p>Pierna Izquierda: <input type='range' min='0' max='180' value='90' onchange='updateServo(11, this.value)'></p>
        </div>
    </div>
</body>
</html>
)rawliteral";

// Manejadores para los botones
void handleFeliz() {
  Serial.println("Expresión Feliz activada");
  wakeup();
  server.send(200, "text/plain", "OK");
}

void handleMoverOjosDerecha() {
  Serial.println("Movimiento de ojos a la derecha activado");
  move_right_big_eye();
  server.send(200, "text/plain", "OK");
}

void handleMoverOjosIzquierda() {
  Serial.println("Movimiento de ojos a la izquierda activado");
  move_left_big_eye();
  server.send(200, "text/plain", "OK");
}

void handleMovimientosAleatorios() {
  Serial.println("Movimientos aleatorios de los ojos activados");
  random_eye_movements();
  server.send(200, "text/plain", "OK");
}

void handleDormir() {
  Serial.println("Estado de dormir activado");
  sleep();
  server.send(200, "text/plain", "OK");
}

void handleOjosFelices() {
  Serial.println("Expresión de ojos felices activada");
  happy_eye();
  server.send(200, "text/plain", "OK");
}

void handleRoot() {
  server.send(200, "text/html", HTML_PAGE);
}

// Función para manejar las solicitudes a "/servo" (actualizar ángulo del servo)
void handleServo() {
  if (!server.hasArg("index") || !server.hasArg("angle")) {
    server.send(400, "text/plain", "Solicitud inválida");
    return;
  }
  int index = server.arg("index").toInt();
  int angle = server.arg("angle").toInt();
  if (index >= 0 && index < 12 && angle >= 0 && angle <= 180) {
    servoActuales[index] = angle;
    pwm.setPWM(servoPins[index], 0, angleToPulse(angle));
    server.send(200, "text/plain", "Servo " + String(servoNames[index]) + " a " + String(angle) + " grados");
  } else {
    server.send(400, "text/plain", "Valores fuera de rango");
  }
}

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Error al iniciar OLED"));
    for (;;)
      ;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Servidor Web Iniciado"));
  display.display();
  delay(2000);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("\nConectado a WiFi!");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/feliz", handleFeliz);
  server.on("/mover_ojos_derecha", handleMoverOjosDerecha);
  server.on("/mover_ojos_izquierda", handleMoverOjosIzquierda);
  server.on("/movimientos_aleatorios", handleMovimientosAleatorios);
  server.on("/dormir", handleDormir);
  server.on("/ojos_felices", handleOjosFelices);
  server.on("/servo", handleServo);
  server.begin();

  sleep();

  pwm.begin();
  pwm.setPWMFreq(50);

  for (int i = 0; i < 12; i++) {
    pwm.setPWM(servoPins[i], 0, angleToPulse(servoActuales[i]));
  }

  Serial.println("Servidor web iniciado");
}

void loop() {
  server.handleClient();
  if (!isActionRunning) {
    blink(8);
    delay(2000);
  }
}