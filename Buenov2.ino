// libreria para el RTC
#include <Wire.h>
#include "RTClib.h"
// ----- libreria para pantalla---
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//--- variables para el programa general------
int input;
// ------ variables  para  MENSAJE --------
String Mensaje = "" ;
//------- variables para TEMPERATURA----------
float tempC; // Variable para almacenar el valor obtenido del sensor (0 a 1023)
int pinLM35 = 0; // Variable del pin de entrada del sensor (A0)
// -------- variables para RELOJ ----------
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sund", "Mond", "Tuesd", "Wedn", "Thur", "Frid", "Satu"};
// ------ variables para DISTANCIA ------------
long dis;
long tiem;
int led = 16;
const int Trigger = 7;   //Pin digital 2 para el Trigger del sensor
const int Echo = 6;   //Pin digital 3 para el Echo del sensor



void setup() {
  // ---------- GENERAL -------------
#ifndef ESP8266
  while (!Serial);
#endif
  Serial.begin(9600);
  delay(3000); // wait for console opening
  lcd.begin(16, 2);
  lcd.clear();
  // ------------- MENSAJES---------
  pinMode(10 , OUTPUT);
  digitalWrite(10 , HIGH);
  //--------------- RELOJ ----------
  if (! rtc.begin()) {
    Serial.println("NO SE PUDO CONECTAR");
    lcd.println("NO SE ESTA EJECUTANDO LA CONEXION!");
    delay(10000);
    while (1);
  }


  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    lcd.begin(16, 2);
    lcd.clear();
  }
  

  //--------------- TEMPERATURA-----
  analogReference(INTERNAL);

  //sensor ultrasonico

  pinMode(Trigger, OUTPUT); //pin como salida
  pinMode(Echo, INPUT);  //pin como entrada
  digitalWrite(Trigger, LOW);//Inicializamos el pin con 0
}

void loop() {
  if (Serial.available() > 0) {
    //input = Serial.read();
    digitalWrite(led,HIGH);
    input = Decimal_to_ASCII(Serial.read());

    //------------- TEMPERATURA ------------
    if (input == '3') {
      lcd.setCursor(0,0);
      lcd.print("Temperatura a");
      tempC = analogRead(pinLM35);
      // Calculamos la temperatura con la fórmula
      tempC = (1.1 * tempC * 100.0 ) / 1024.0;
      lcd.setCursor(0, 1);
      lcd.print("          C");
      delay(1000);
      lcd.setCursor(5, 1);
      lcd.print(tempC);
      // Envia el dato al puerto serial
      Serial.print(tempC);
      // Salto de línea
      Serial.print("\n");

      // Esperamos un tiempo para repetir el loop
      delay(1000);

    }
    //---------------- HORA -----------
    else if (input == '2') {

      DateTime now = rtc.now();
      lcd.begin(16, 2);
      //lcd.clear();
      lcd.print(now.year(), DEC);
      lcd.print('/');
      Serial.print(now.year(), DEC);
      Serial.print('/');
      lcd.print(now.month(), DEC);
      lcd.print('/');
      Serial.print(now.month(), DEC);
      Serial.print('/');
      lcd.print(now.day(), DEC);
      lcd.print('/');
      Serial.print(now.day(), DEC);
      Serial.print(" (");
      Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
      lcd.print('(');
      lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
      lcd.print(')');
      Serial.print(") ");
      lcd.setCursor(0, 1);
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      lcd.print(now.hour(), DEC);
      lcd.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      lcd.print(now.minute(), DEC);
      lcd.print(':');
      Serial.print(now.second(), DEC);
      Serial.println();
      lcd.print(now.second(), DEC);
      Serial.println();
      delay(1000);
      
    }else if( input == '4' ){  //------------distancia
       long t; //timepo que demora en llegar el eco
  long d; //distancia en centimetros

  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);
  
  t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
  d = t/59;             //escalamos el tiempo a una distancia en cm
  
  lcd.print("Dis:");
  lcd.print(d);      //Enviamos serialmente el valor de la distancia
  lcd.print("cm");
  lcd.println();
  delay(100);          //Hacemos una pausa de 100ms
    }

   
    //--------------- MENSAJES -------------
    else {
      
      int caracteres = 0;
      while (Serial.available() > 0) {
        Mensaje = Mensaje + Decimal_to_ASCII(Serial.read());
      }
      caracteres = Mensaje.length();
      if (caracteres > 16) {
        if (Mensaje != "") {
          lcd.clear();
          lcd.print(Mensaje.substring(0, 16));
          lcd.setCursor(0, 1);
          lcd.print(Mensaje.substring(16, caracteres));

        }

      } else {
        if ( Mensaje != "") {
          lcd.clear();
          lcd.print(Mensaje);
        }
      }
      delay(1000);
      Mensaje = "";
      


    }
  }
  else if ( input == '0'){
    digitalWrite(led,LOW);
  }
}

//----------- METODO INDEPENDIENTE ( FUERA DEL LOOP PARA CODIGO DE  MENSAJES---------------
char Decimal_to_ASCII(int entrada) {
  char salida = ' ' ;
  switch (entrada) {
    case 32:
      salida = ' ';
      break;
    case 33:
      salida = '!';
      break;
    case 34:
      salida = '"';
      break;
    case 35:
      salida = '#';
      break;
    case 36:
      salida = '$';
      break;
    case 37:
      salida = '%';
      break;
    case 38:
      salida = '&';
      break;
    case 39:
      salida = ' ';
      break;
    case 40:
      salida = '(';
      break;
    case 41:
      salida = ')';
      break;
    case 42:
      salida = '*';
      break;
    case 43:
      salida = '+';
      break;
    case 44:
      salida = ',';
      break;
    case 45:
      salida = '-';
      break;
    case 46:
      salida = '.';
      break;
    case 47:
      salida = '/';
      break;
    case 48:
      salida = '0';
      break;
    case 49:
      salida = '1';
      break;
    case 50:
      salida = '2';
      break;
    case 51:
      salida = '3';
      break;
    case 52:
      salida = '4';
      break;
    case 53:
      salida = '5';
      break;
    case 54:
      salida = '6';
      break;
    case 55:
      salida = '7';
      break;
    case 56:
      salida = '8';
      break;
    case 57:
      salida = '9';
      break;
    case 58:
      salida = ':';
      break;
    case 59:
      salida = ';';
      break;
    case 60:
      salida = '<';
      break;
    case 61:
      salida = '=';
      break;
    case 62:
      salida = '>';
      break;
    case 63:
      salida = '?';
      break;
    case 64:
      salida = '@';
      break;
    case 65:
      salida = 'A';
      break;
    case 66:
      salida = 'B';
      break;
    case 67:
      salida = 'C';
      break;
    case 68:
      salida = 'D';
      break;
    case 69:
      salida = 'E';
      break;
    case 70:
      salida = 'F';
      break;
    case 71:
      salida = 'G';
      break;
    case 72:
      salida = 'H';
      break;
    case 73:
      salida = 'I';
      break;
    case 74:
      salida = 'J';
      break;
    case 75:
      salida = 'K';
      break;
    case 76:
      salida = 'L';
      break;
    case 77:
      salida = 'M';
      break;
    case 78:
      salida = 'N';
      break;
    case 79:
      salida = 'O';
      break;
    case 80:
      salida = 'P';
      break;
    case 81:
      salida = 'Q';
      break;
    case 82:
      salida = 'R';
      break;
    case 83:
      salida = 'S';
      break;
    case 84:
      salida = 'T';
      break;
    case 85:
      salida = 'U';
      break;
    case 86:
      salida = 'V';
      break;
    case 87:
      salida = 'W';
      break;
    case 88:
      salida = 'X';
      break;
    case 89:
      salida = 'Y';
      break;
    case 90:
      salida = 'Z';
      break;
    case 91:
      salida = '[';
      break;
    case 92:
      salida = ' ';
      break;
    case 93:
      salida = ']';
      break;
    case 94:
      salida = '^';
      break;
    case 95:
      salida = '_';
      break;
    case 96:
      salida = '`';
      break;
    case 97:
      salida = 'a';
      break;
    case 98:
      salida = 'b';
      break;
    case 99:
      salida = 'c';
      break;
    case 100:
      salida = 'd';
      break;
    case 101:
      salida = 'e';
      break;
    case 102:
      salida = 'f';
      break;
    case 103:
      salida = 'g';
      break;
    case 104:
      salida = 'h';
      break;
    case 105:
      salida = 'i';
      break;
    case 106:
      salida = 'j';
      break;
    case 107:
      salida = 'k';
      break;
    case 108:
      salida = 'l';
      break;
    case 109:
      salida = 'm';
      break;
    case 110:
      salida = 'n';
      break;
    case 111:
      salida = 'o';
      break;
    case 112:
      salida = 'p';
      break;
    case 113:
      salida = 'q';
      break;
    case 114:
      salida = 'r';
      break;
    case 115:
      salida = 's';
      break;
    case 116:
      salida = 't';
      break;
    case 117:
      salida = 'u';
      break;
    case 118:
      salida = 'v';
      break;
    case 119:
      salida = 'w';
      break;
    case 120:
      salida = 'x';
      break;
    case 121:
      salida = 'y';
      break;
    case 122:
      salida = 'z';
      break;
    case 123:
      salida = '{';
      break;
    case 124:
      salida = '|';
      break;
    case 125:
      salida = '}';
      break;
    case 126:
      salida = '~';
      break;
  }
  return salida;
}





