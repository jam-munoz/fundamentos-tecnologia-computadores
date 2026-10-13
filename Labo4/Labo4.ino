///////////////////////////////////////////////////////////////
// Fundamentos de Tecnologia de los Computadores
// Ruleta LED RGB con transiciones suaves
// ESP32-S3-WROOM-1 - ESP32-S3 Dev Module
///////////////////////////////////////////////////////////////

#include <Arduino.h>
#include "driver/gpio.h"
#include "esp_system.h"

// LED RGB
#define LED_RGB 38

// Brillo: 0 = apagado, 255 = brillo maximo
uint8_t brillo = 63;

// Variables globales
uint8_t levelGreen = 0;
uint8_t levelRed   = 0;
uint8_t levelBlue  = 0;

// Prototipos
void imprime(uint8_t Green, uint8_t Red, uint8_t Blue);

void color(uint8_t verde, uint8_t rojo, uint8_t azul);

void envia_uno();
void envia_cero();

void transicion(uint8_t verde1, uint8_t rojo1, uint8_t azul1,
                uint8_t verde2, uint8_t rojo2, uint8_t azul2);

///////////////////////////////////////////////////////////////
// setup

void setup() {
  Serial.begin(115200);
  Serial.println("Ruleta LED RGB");
  delay(1000);

  // Configurar GPIO del LED
  gpio_config_t conf = {};
  conf.pin_bit_mask = (1ULL << LED_RGB);
  conf.mode = GPIO_MODE_OUTPUT;
  conf.pull_up_en = GPIO_PULLUP_DISABLE;
  conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  conf.intr_type = GPIO_INTR_DISABLE;

  gpio_config(&conf);
  gpio_set_level((gpio_num_t)LED_RGB, 0);

  // Inicializar el generador aleatorio
  randomSeed(esp_random());

  Serial.println("La ruleta esta girando...");

  // Elegir el color ganador
  int ganador = random(0, 6);

  // Numero de pasos de la ruleta
  int vueltas = 20 + random(0, 10);

  // Color anterior para realizar las transiciones
  uint8_t verdeAnterior = 0;
  uint8_t rojoAnterior  = 0;
  uint8_t azulAnterior  = 0;

  // Simular el giro, frenando progresivamente
  for (int i = 0; i < vueltas; i++) {
    int colorActual = i % 6;

    switch (colorActual) {
      case 0: // Rojo
        levelGreen = 0;
        levelRed = 255;
        levelBlue = 0;
        printf("\033[31mRojo\033[31m\n");
        break;

      case 1: // Verde
        levelGreen = 255;
        levelRed = 0;
        levelBlue = 0;
        printf("\033[32mVerde\033[32m\n");
        break;

      case 2: // Azul
        levelGreen = 0;
        levelRed = 0;
        levelBlue = 255;
        printf("\033[34mAzul\033[34m\n");
        break;

      case 3: // Amarillo
        levelGreen = 255;
        levelRed = 255;
        levelBlue = 0;
        printf("\033[33mAmarillo\033[0m\n");
        break;

      case 4: // Cian
        levelGreen = 255;
        levelRed = 0;
        levelBlue = 255;
        printf("\033[36mCian\033[36m\n");
        break;

      case 5: // Magenta
        levelGreen = 0;
        levelRed = 255;
        levelBlue = 255;
        printf("\033[35mMagenta\033[35m\n");
        break;
    }

    // Transicion gradual del color anterior al siguiente
    transicion(
      verdeAnterior, rojoAnterior, azulAnterior,
      levelGreen, levelRed, levelBlue
    );

    // Guardar el color actual
    verdeAnterior = levelGreen;
    rojoAnterior = levelRed;
    azulAnterior = levelBlue;

    // Cada paso tarda un poco mas: parece que frena
    delay(50 + i * 10);
  }

  /////////////////////////////////////////////////////////////
  // Mostrar el resultado final

  switch (ganador) {
    case 0: // Rojo
      levelGreen = 0;
      levelRed = 255;
      levelBlue = 0;
      Serial.println("\033[31mGANADOR: ROJO\033[31m");
      break;

    case 1: // Verde
      levelGreen = 255;
      levelRed = 0;
      levelBlue = 0;
      Serial.println("\033[32mGANADOR: VERDE\033[32m");
      break;

    case 2: // Azul
      levelGreen = 0;
      levelRed = 0;
      levelBlue = 255;
      Serial.println("\033[34mGANADOR: AZUL\033[34m");
      break;

    case 3: // Amarillo
      levelGreen = 255;
      levelRed = 255;
      levelBlue = 0;
      Serial.println("\033[33mGANADOR: AMARILLO\033[33m");
      break;

    case 4: // Cian
      levelGreen = 255;
      levelRed = 0;
      levelBlue = 255;
      Serial.println("\033[36mGANADOR: CIAN\033[36m");
      break;

    case 5: // Magenta
      levelGreen = 0;
      levelRed = 255;
      levelBlue = 255;
      Serial.println("\033[35mGANADOR: MAGENTA\033[35m");
      break;
  }

  // Transicion final hasta el color ganador
  transicion(
    verdeAnterior, rojoAnterior, azulAnterior,
    levelGreen, levelRed, levelBlue
  );
}

///////////////////////////////////////////////////////////////
// loop

void loop() {
  // La ruleta se ejecuta una sola vez al iniciar.
}

///////////////////////////////////////////////////////////////
// Mostrar valores por el puerto serie

void imprime(uint8_t Green, uint8_t Red, uint8_t Blue) {
  Serial.print(Green);
  Serial.print(" ");
  Serial.print(Red);
  Serial.print(" ");
  Serial.println(Blue);
}

///////////////////////////////////////////////////////////////
// Transicion gradual entre dos colores

void transicion(uint8_t verde1, uint8_t rojo1, uint8_t azul1,
                uint8_t verde2, uint8_t rojo2, uint8_t azul2) {

  const int pasos = 30;

  for (int paso = 0; paso <= pasos; paso++) {
    uint8_t verde = verde1 +
      ((int)verde2 - (int)verde1) * paso / pasos;

    uint8_t rojo = rojo1 +
      ((int)rojo2 - (int)rojo1) * paso / pasos;

    uint8_t azul = azul1 +
      ((int)azul2 - (int)azul1) * paso / pasos;

    color(verde, rojo, azul);
    delay(15);
  }
}

///////////////////////////////////////////////////////////////
// Envio del color RGB en orden GRB
// Se aplica el brillo sin modificar los valores originales.

void color(uint8_t verde, uint8_t rojo, uint8_t azul) {
  // Reducir intensidad
  verde = ((uint16_t)verde * brillo) / 255;
  rojo  = ((uint16_t)rojo  * brillo) / 255;
  azul  = ((uint16_t)azul  * brillo) / 255;

  // Envio del verde
  for (int i = 0; i < 8; i++) {
    if (((verde >> (7 - i)) & 1) == 1)
      envia_uno();
    else
      envia_cero();
  }

  // Envio del rojo
  for (int i = 0; i < 8; i++) {
    if (((rojo >> (7 - i)) & 1) == 1)
      envia_uno();
    else
      envia_cero();
  }

  // Envio del azul
  for (int i = 0; i < 8; i++) {
    if (((azul >> (7 - i)) & 1) == 1)
      envia_uno();
    else
      envia_cero();
  }

  // Intervalo bajo para registrar el color
  delayMicroseconds(80);
}

///////////////////////////////////////////////////////////////
// Envio de un bit 0

void envia_cero() {
  gpio_set_level((gpio_num_t)LED_RGB, 1);
  gpio_set_level((gpio_num_t)LED_RGB, 0);
  gpio_set_level((gpio_num_t)LED_RGB, 0);
  gpio_set_level((gpio_num_t)LED_RGB, 0);
}

///////////////////////////////////////////////////////////////
// Envio de un bit 1

void envia_uno() {
  gpio_set_level((gpio_num_t)LED_RGB, 1);
  gpio_set_level((gpio_num_t)LED_RGB, 1);
  gpio_set_level((gpio_num_t)LED_RGB, 1);
  gpio_set_level((gpio_num_t)LED_RGB, 0);
  gpio_set_level((gpio_num_t)LED_RGB, 0);
}

// END