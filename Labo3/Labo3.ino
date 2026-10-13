// Fundamentos de Tecnología de Computadores
// Curso 2026-2027
// Pirámide ASCII en terminal usando bucles for y printf

// ************ Includes
#include <Arduino.h>  // Básicas
#include <stdio.h>

// ****************** Setup ************************
void setup() 
{
  delay(2000);                                // Retraso de 2 segundos

  Serial.begin(115200);                       // Inicio Serial Monitor y velocidad
}

void loop()
{
  for(int i = 1; i <= 10; i++)
  {
    delay(1000);
    for(int j = 10; j >= i; j--)
    {
      Serial.printf(" ");
    }                             // Retraso de 1 segundos
    for(int j = 1; j <= i; j++)
    {
      Serial.printf("* ");
    }
    Serial.printf("\n");                   // Mensaje a transmitir por Monitor
  }

  while(1);                                     // Bucle infinito

}
