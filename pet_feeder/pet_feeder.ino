/*
 * Projeto Pet Feeder utilizando o Intel Edison
 * 
 * Desenvolvido por:
 *    - Antonio Carlos Falcao Petri
 *    - Christianno Guimaraes
 *    - Gabriela Mattos
 *    - Murilo Ferraz de Almeida
 *    - Samuel Pereira
 * 
 * Apresentado no Intel IoT RoadShow Sao Paulo - 2015
 * #IntelMaker
 */

#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

#include <Twitter.h>
#include <hcsr02.h>

#include <rgb_lcd.h>

#define TOKEN_KEY "SEU_TOKEN_AQUI"

#define DEBUG 0

#define OPEN 80
#define CLOSE 10

// Mac Address:
byte mac[] = { 0xAA, 0xAB, 0xDE, 0x1F, 0x26, 0x5D };

rgb_lcd lcd;
HCSR02 ultrasonic(8,7);
Servo servo;

Twitter twitter(TOKEN_KEY);

int acionado = LOW;
int out_dispenser = 3;
int in_door = 2;
int thisread = LOW, lastread = LOW;
long thistime = 0, lasttime = 0, timeBetweenMeals = 2 * 1000;
int openedTime = 500;

time_t lastOpened;

int contador = 1;    // contador para evitar duplicidade de tweets

void setup() {
  Serial.begin(115200);

  Ethernet.begin(mac);
  ultrasonic.setup();

  servo.attach(out_dispenser);

  lcd.begin(16, 2);
  lcd.setRGB(255, 0, 0);
  
  lcd.print("Pet Feeder");
  lcd.setCursor(0, 1);
  lcd.print("#IntelMaker");

  servo.write(CLOSE);
  pinMode(in_door, INPUT);

  lastOpened = -1;
}

void activateDispenser(int openedTime)
{
  if (DEBUG) Serial.println("Writing to Servo");
  servo.write(OPEN);
  delay(openedTime);
  servo.write(CLOSE);
}

void sendMessage() {
    char stringMsg[] = "Seu Cachorro acabou de comer!!! #IntelMaker"; // mensagem que será postada
    
    time_t t = time(NULL);
    tm* atual = localtime(&t);
    
    char msg[140];
    strncpy(msg, stringMsg, 140);
    snprintf(msg, 140, "%s as %02d:%02d:%02d", stringMsg, atual->tm_hour, atual->tm_min, atual->tm_sec);
    
   if (twitter.post(msg)) {    
    int status = twitter.wait(&Serial);
    if (status == 200) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.println("Mensagem Postada"); // confirmação no LCD
      Serial.print("postado");
      contador = contador + 1;
    } else {
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Falha"); // confirmação de falha
      Serial.print("falha");
      
    }
  } else {
    lcd.println("Falha na conexao."); // ouve falha na conexão
  }
  
  contador = contador + 1;  // incrementar o contador para o próximo tweet
  if (DEBUG) Serial.print(contador);
  delay(3000); // uma espera básica !!
  
  lcd.clear();
  lcd.print("Pet Feeder");
  lcd.setCursor(0, 1);
  lcd.print("#IntelMaker");
}

void loop() {
  thisread = digitalRead(in_door);
  thistime = millis();
  if (inRange() && (thistime - lasttime > timeBetweenMeals))
  {
    if (DEBUG) Serial.println("Button pressed");
    feedThePet();
    lasttime = millis();
  }
}

bool inRange () {
  int distance = ultrasonic.getDistance();
  
  return 0 < distance && distance <= 10;
}
void feedThePet () {
  lastOpened = time(NULL);
  activateDispenser(openedTime);
  sendMessage();
}


