// Time - Version: Latest 
#include <Time.h>
#include <TimeLib.h>

float temperatura = 0;
int compruebaTemp = 25;
int codigos[11][7] = {
// A  B  C  D  E  F  G
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}, // 9
  {1, 0, 0, 1, 1, 1, 0}  // C (10)
};

void setup() {
  // Configuración de los pines
  pinMode(13, OUTPUT); // Segmento A
  pinMode(12, OUTPUT); // Segmento B
  pinMode(11, OUTPUT); // Segmento C
  pinMode(10, OUTPUT); // Segmento D
  pinMode(9, OUTPUT);  // Segmento E
  pinMode(8, OUTPUT);  // Segmento F
  pinMode(7, OUTPUT);  // Segmento G
  
  // Estos pines activan o desactivan segmentos
  pinMode(6, OUTPUT);  // Decenas hora
  pinMode(5, OUTPUT);  // Unidades hora
  pinMode(4, OUTPUT);  // Decenas minutos
  pinMode(3, OUTPUT);  // Unidades minutos

  pinMode(2, INPUT);   // Boton que muestra la temperatura al ser presionado
  
  // Iniciar segmentos apagados
  digitalWrite(6, 1);
  digitalWrite(5, 1);
  digitalWrite(4, 1);
  digitalWrite(3, 1);

  Serial.begin(9600);
}

void loop() {
  String data;
  if(digitalRead(2) == 1) {
    mostrarTemperatura();
    if(compruebaTemp == 25) {
      checaTemperatura();
      compruebaTemp = 0;
    } else {
      compruebaTemp += 1;
    }
  } else {
    mostrarHora();
    if(compruebaTemp == 25) {
      checaTemperatura();
      compruebaTemp = 0;
    } else {
      compruebaTemp += 1;
    }
  }
  if(Serial.available() > 0) {
    data = Serial.readStringUntil('\n');
    if(data.startsWith("calibra")) {
      // Obtenemos los datos para hacer la calibración del reloj
      // Se necesita conocer el formato en el que fue recibida la información para manipularla
      // setTime(hora,minutos,segundos,dia,mes,año);
      setTime(12, 32, 0, 13, 12, 2016);
    }
  }
}

void mostrarHora() {
  int valor;
  
  // Unidades minutos
  valor = minute() % 10;
  for(int pin = 13, output = 0; pin >= 7; pin--, output++) {
    digitalWrite(pin, codigos[valor][output]);
  }
  digitalWrite(3, 0);
  delay(15);
  digitalWrite(3, 1);
  
  // Decenas minutos
  valor = minute() / 10;
  for(int pin = 13, output = 0; pin >= 7; pin--, output++) {
    digitalWrite(pin, codigos[valor][output]);
  }
  digitalWrite(4, 0);
  delay(15);
  digitalWrite(4, 1);
  
  // Unidades hora
  valor = hour() % 10;
  for(int pin = 13, output = 0; pin >= 7; pin--, output++) {
    digitalWrite(pin, codigos[valor][output]);
  }
  digitalWrite(5, 0);
  delay(15);
  digitalWrite(5, 1);
  
  // Decenas hora
  valor = hour() / 10;
  for(int pin = 13, output = 0; pin >= 7; pin--, output++) {
    digitalWrite(pin, codigos[valor][output]);
  }
  digitalWrite(6, 0);
  delay(15);
  digitalWrite(6, 1);
}

void mostrarTemperatura() {
  int valor;
  int parteEntera = (int) temperatura;
  
  delay(15);
  // Decenas minutos
  for(int pin = 13, output = 0; pin >= 7; pin--, output++) {
    digitalWrite(pin, codigos[10][output]);
  }
  digitalWrite(4, 0);
  delay(15);
  digitalWrite(4, 1);
  
  // Unidades hora
  valor = parteEntera % 10;
  for(int pin = 13, output = 0; pin >= 7; pin--, output++) {
    digitalWrite(pin, codigos[valor][output]);
  }
  digitalWrite(5, 0);
  delay(15);
  digitalWrite(5, 1);
  
  // Decenas hora
  valor = parteEntera / 10;
  for(int pin = 13, output = 0; pin >= 7; pin--, output++) {
    digitalWrite(pin, codigos[valor][output]);
  }
  digitalWrite(6, 0);
  delay(15);
  digitalWrite(6, 1);
}

void checaTemperatura() {
  String temp;
  temperatura = analogRead(0);
  // Formula para calcular temperatura sin simplificar:
  // (lectura * voltajeMax(5) / numeroValoresPosibles(1024)) / voltajePorGradoCentigrado(0.01 | 1/100)
  temperatura = (5.0 * temperatura * 100.0) / 1024.0;
  temp = String(temperatura, 2);
  Serial.println(temp);
}
