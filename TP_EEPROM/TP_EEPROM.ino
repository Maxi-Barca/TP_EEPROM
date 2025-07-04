//BARCAROLO, MORAT Y RESNIK

#include <U8g2lib.h>
#include "DHT.h"
#include <Preferences.h>
/* ----------------------------------------------------------------------------------------------------- */

// Definicion de constructores y variables globales

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

/* ----------------------------------------------------------------------------------------------------- */
void printBMP_OLED(void );
void printBMP_OLED2(void) ;
#define BOTON1 34
#define BOTON2 35
#define P1 0
#define P2 1
#define RST 20
#define ESPERA1 2
#define ESPERA2 3
#define AUMENTAR 4
#define RESTAR 5
int estado = RST;
#define LED 25
#define DHTPIN 23
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float temp;
int valorU = 23;
int millis_valor;
int millis_actual;
int millis_actual2;
int millis_boton;

Preferences preferences;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BOTON1, INPUT_PULLUP);
  pinMode(BOTON2, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));
  u8g2.begin();
  dht.begin();

  preferences.begin("config", false); 
  valorU = preferences.getInt("valorU", 23); 
  preferences.end();

  Serial.print("Valor U leído desde memoria: ");
  Serial.println(valorU);
}

void loop() {
  millis_actual = millis();
  if (millis_actual - millis_valor >= 2000) {
    temp = dht.readTemperature();
    if (isnan(temp)) {
      Serial.println(F("Failed to read from DHT sensor!"));
    }
    millis_valor = millis_actual;
  }

    if (temp >= valorU) {
          digitalWrite(LED, HIGH);
        }
        if (temp < valorU) {
          digitalWrite(LED, LOW);
        }
  
    switch (estado) {
      case RST:
        {
          millis_actual2 = millis();
          estado = P1;
        }
          break;
        case P1:
          {
            printBMP_OLED();
            millis_actual2 = millis();
            if (digitalRead(BOTON1) == LOW) {
              millis_boton = millis_actual2;
              Serial.println("va a espera1");
              estado = ESPERA1;
            }
          }
          break;
        case ESPERA1:
          {
            Serial.println("espera1");
            millis_actual2 = millis();
            if (millis_actual2 - millis_boton >= 5000 && digitalRead(BOTON1) == HIGH) {
              Serial.println("5 seg");
              estado = P2;
            }
            if (millis_actual2 - millis_boton < 5000 && digitalRead(BOTON1) == HIGH) {
              millis_boton = millis_actual2;
              Serial.println("vuelve p1");
              estado = P1;
            }

          }
          break;
        case P2:
          {
            millis_boton = millis_actual2;
            millis_actual2 = millis();
            printBMP_OLED2();
            if (digitalRead(BOTON1) == LOW) {
              estado = AUMENTAR;
            }
            if (digitalRead(BOTON2) == LOW) {
              estado = RESTAR;
            }
            if (digitalRead(BOTON2) == LOW) {
              millis_boton = millis_actual2;
              estado = ESPERA2;
            }
          }
          break;
        case ESPERA2:
          {
            Serial.println("espera2");
            millis_actual2 = millis();
            if (millis_actual2 - millis_boton >= 5000) {
              Serial.println("va a p1");
              preferences.begin("config", false);
              preferences.putInt("valorU", valorU);
              preferences.end();
              millis_boton = millis_actual2;
              estado = P1;
            }
            if (digitalRead(BOTON2) == HIGH) {
              valorU = valorU - 1;
              millis_boton = millis_actual2;
              estado = P2;
            }
          }
          break;

        case AUMENTAR:
          {
            
            if (digitalRead(BOTON1) == HIGH) {
              valorU = valorU + 1;
               estado = P2;
            }

          }
          break;

        }   
    }


void printBMP_OLED(void) {
  char stringU[5];
  char stringtemp[5];
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_t0_11b_tr); // choose a suitable font
  sprintf (stringtemp, "%.2f" , temp); ///convierto el valor float a string
  sprintf (stringU, "%d" , valorU); ///convierto el valor float a string
  u8g2.drawStr(0, 35, "T. Actual:");
  u8g2.drawStr(60, 35, stringtemp);
  u8g2.drawStr(90, 35, "°C");
  u8g2.drawStr(0, 50, "V. Umbral:");
  u8g2.drawStr(60, 50, stringU);
  u8g2.drawStr(75, 50, "°C");
  u8g2.sendBuffer();          // transfer internal memory to the display
}

void printBMP_OLED2(void) {
  char stringU[5];
  u8g2.clearBuffer();          // clear the internal memory
  sprintf (stringU, "%d" , valorU);
  u8g2.setFont(u8g2_font_t0_11b_tr); // choose a suitable font
  u8g2.drawStr(0, 50, "V. Umbral:");
  u8g2.drawStr(60, 50, stringU);
  u8g2.drawStr(75, 50, "°C");
  u8g2.sendBuffer();          // transfer internal memory to the display
}
//11-6200-5317
