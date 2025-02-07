#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Configuración de la pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Estado inicial 
bool demo_mode = true;
const int MAX_ANIMATION_INDEX = 8;
int current_animation_index = 0;

// Parámetros de referencia de los ojos
const int REF_EYE_HEIGHT = 40;
const int REF_EYE_WIDTH = 40;
const int REF_SPACE_BETWEEN_EYE = 10;
const int REF_CORNER_RADIUS = 10;

// Estado actual de los ojos
int left_eye_x = 32;
int left_eye_y = 32;
int right_eye_x = left_eye_x + REF_EYE_WIDTH + REF_SPACE_BETWEEN_EYE;
int right_eye_y = 32;
int eye_height = REF_EYE_HEIGHT;
int eye_width = REF_EYE_WIDTH;

// Función para dibujar los ojos
void draw_eyes(bool update = true) {
    display.clearDisplay();
    display.fillRoundRect(left_eye_x - eye_width / 2, left_eye_y - eye_height / 2, eye_width, eye_height, REF_CORNER_RADIUS, SSD1306_WHITE);
    display.fillRoundRect(right_eye_x - eye_width / 2, right_eye_y - eye_height / 2, eye_width, eye_height, REF_CORNER_RADIUS, SSD1306_WHITE);
    if (update) display.display();
}

// Centrar los ojos en la pantalla
void center_eyes(bool update = true) {
    left_eye_x = SCREEN_WIDTH / 2 - REF_EYE_WIDTH / 2 - REF_SPACE_BETWEEN_EYE / 2;
    right_eye_x = SCREEN_WIDTH / 2 + REF_EYE_WIDTH / 2 + REF_SPACE_BETWEEN_EYE / 2;
    left_eye_y = right_eye_y = SCREEN_HEIGHT / 2;
    eye_height = REF_EYE_HEIGHT;
    eye_width = REF_EYE_WIDTH;
    draw_eyes(update);
}

// Función para parpadear
void blink(int speed = 12) {
    for (int i = 0; i < 3; i++) {
        eye_height -= speed;
        draw_eyes();
        delay(1);
    }
    for (int i = 0; i < 3; i++) {
        eye_height += speed;
        draw_eyes();
        delay(1);
    }
}

// Función para simular los ojos dormidos
void sleep() {
    eye_height = 2;
    draw_eyes(true);
}

// Función para despertar los ojos
void wakeup() {
    sleep();
    for (int h = 0; h <= REF_EYE_HEIGHT; h += 2) {
        eye_height = h;
        draw_eyes(true);
    }
}

// Función para simular ojos felices
void happy_eye() {
    center_eyes(false);
    int offset = REF_EYE_HEIGHT / 2;
    for (int i = 0; i < 10; i++) {
        display.fillTriangle(left_eye_x - REF_EYE_WIDTH / 2, left_eye_y + offset, left_eye_x + REF_EYE_WIDTH / 2, left_eye_y + 5 + offset, left_eye_x - REF_EYE_WIDTH / 2, left_eye_y + REF_EYE_HEIGHT + offset, SSD1306_BLACK);
        display.fillTriangle(right_eye_x + REF_EYE_WIDTH / 2, right_eye_y + offset, right_eye_x - REF_EYE_WIDTH / 2, right_eye_y + 5 + offset, right_eye_x + REF_EYE_WIDTH / 2, right_eye_y + REF_EYE_HEIGHT + offset, SSD1306_BLACK);
        offset -= 2;
        display.display();
        delay(1);
    }
    delay(1000);
}

// Función para mover los ojos en movimientos sacádicos
void saccade(int direction_x, int direction_y) {
    left_eye_x += 8 * direction_x;
    right_eye_x += 8 * direction_x;
    left_eye_y += 6 * direction_y;
    right_eye_y += 6 * direction_y;
    eye_height -= 8;
    draw_eyes();
    delay(1);
    eye_height += 8;
    draw_eyes();
    delay(1);
}

// Lanzar animaciones según el índice
void launch_animation(int animation_index) {
    if (animation_index > MAX_ANIMATION_INDEX) animation_index = MAX_ANIMATION_INDEX;
    switch (animation_index) {
        case 0: wakeup(); break;
        case 1: center_eyes(true); break;
        case 2: saccade(1, 0); break;
        case 3: saccade(-1, 0); break;
        case 4: blink(10); break;
        case 5: blink(20); break;
        case 6: happy_eye(); break;
        case 7: sleep(); break;
        case 8:
            center_eyes(true);
            for (int i = 0; i < 20; i++) {
                int dir_x = random(-1, 2);
                int dir_y = random(-1, 2);
                saccade(dir_x, dir_y);
                delay(1);
                saccade(-dir_x, -dir_y);
                delay(1);
            }
            break;
    }
}

void setup() {
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    Serial.begin(115200);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Steveen Sinchiguano"));
    display.display();
    delay(2000);
    sleep();
    delay(2000);
}

void loop() {
    if (demo_mode) {
        launch_animation(current_animation_index++);
        if (current_animation_index > MAX_ANIMATION_INDEX) current_animation_index = 0;
    }
    if (Serial.available()) {
        String data = Serial.readString();
        data.trim();
        if (data[0] == 'A') {
            demo_mode = false;
            int anim = data.substring(1).toInt();
            launch_animation(anim);
            Serial.println(anim);
        }
    }
}
