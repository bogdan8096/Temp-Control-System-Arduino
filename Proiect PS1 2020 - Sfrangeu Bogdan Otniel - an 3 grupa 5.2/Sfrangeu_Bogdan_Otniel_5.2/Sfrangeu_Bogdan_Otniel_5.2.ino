#include <EEPROM.h>
#include <dht.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
dht DHT;
#define DHT11_PIN 7
#define HEAT_PIN 6

#define BUTTON_OK 2
#define BUTTON_CANCEL 3
#define BUTTON_PREV 4
#define BUTTON_NEXT 5

unsigned int menu = 0; bool sw = false;
unsigned int currentState[4] = {0, 0, 0, 0}, lastState[4] = {0, 0, 0, 0};
volatile float temperature[3] = {50.0, 0.0, 0.0}, t2set[3] = {60.0, 20.0, 120.0}, k2set[3] = {3.0, 0.5, 100.0};
volatile float Err = 0.0, lastErr = 0.0, sumErr = 0.0, dErr = 0.0, dt = 1.0, output = 0.0;
volatile float moving_setpoint = 0.0; unsigned int uptime = 0;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);
  setTimer1();
  pinMode(BUTTON_OK, INPUT);
  pinMode(BUTTON_CANCEL, INPUT);
  pinMode(BUTTON_PREV, INPUT);
  pinMode(BUTTON_NEXT, INPUT);
  print_menu(0);
}

void loop() 
{
  if(menu == 19)
  {
    int chk = DHT.read11(DHT11_PIN);
    temperature[1] = DHT.temperature;
    print_menu(19);
  }
  uptime += 1;
  delay(1000);
}

void print_menu(unsigned int index)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  switch(index)
  {
    case 0: lcd.print("MAIN MENU"); break;
    case 1: lcd.print("PROGRAM MENU"); break;
    case 2: lcd.print("START PROGRAM"); break;
    case 3: lcd.print("CHANGE PROGRAM"); break;
    case 4: lcd.print("TSET="); lcd.print((int)temperature[0]); lcd.print("C "); 
            lcd.print("TMP="); lcd.print((int)temperature[1]); lcd.print("C");break;
    case 12: lcd.print("TEMPERATURE"); break;
    case 13: lcd.print("HEAT TIME"); break;
    case 14: lcd.print("MAINTAIN TIME"); break;
    case 15: lcd.print("COOL TIME"); break;
    case 16: lcd.print("PID: KD"); break;
    case 17: lcd.print("PID: KI"); break;
    case 18: lcd.print("PID: KP"); break;
    case 19: lcd.print("TSET="); lcd.print((int)temperature[0]); lcd.print("C "); 
             lcd.print("TMP="); lcd.print((int)temperature[1]); lcd.print("C"); break;
  }
  if((index >= 5) && (index <= 11)) lcd.print("CHANGE PARAMETER");
  lcd.setCursor(0, 1);
  switch(index)
  {
    case 0: lcd.print("PS 1 - 2020"); break;
    case 4: lcd.print("SYST DOWN: 00:00"); break;
    case 5: lcd.print("TEMPERATURE"); break;
    case 6: lcd.print("HEAT TIME"); break;
    case 7: lcd.print("MANTAIN TIME"); break;
    case 8: lcd.print("COOL TIME"); break;
    case 9: lcd.print("PID: KD"); break;
    case 10: lcd.print("PID: KI"); break;
    case 11: lcd.print("PID: KP"); break;
    case 12: lcd.print("TEMP = "); lcd.setCursor(7, 1); lcd.print(temperature[0]);break;
    case 13: lcd.print("TH = "); lcd.setCursor(7, 1); lcd.print(t2set[0]);break;
    case 14: lcd.print("TM = "); lcd.setCursor(7, 1); lcd.print(t2set[1]);break;
    case 15: lcd.print("TC = "); lcd.setCursor(7, 1); lcd.print(t2set[2]);break;
    case 16: lcd.print("KD = "); lcd.setCursor(5, 1); lcd.print(k2set[0]);break;
    case 17: lcd.print("KI = "); lcd.setCursor(5, 1); lcd.print(k2set[1]);break;
    case 18: lcd.print("KP = "); lcd.setCursor(5, 1); lcd.print(k2set[2]);break;
    case 19: print_time(); break;
  }
  if((index >= 1) && (index <= 3)) lcd.print("PROCCED PRESS OK");
}

void buttonSettingNAV(unsigned int mymenu)
{
  currentState[0] = digitalRead(BUTTON_NEXT);
  currentState[1] = digitalRead(BUTTON_PREV);
  if((currentState[0] && !lastState[0]))
  {
    switch(mymenu)
    {
      case 0: menu = 1; break;
      case 1: menu = 0; break;
      case 2: menu = 3; break;
      case 3: menu = 2; break;
      case 11: menu = 5; break;
      case 12: temperature[0] += 1; break;
    }
    if((mymenu >= 5) && (mymenu <= 10)) menu++;
    if((mymenu >= 13) && (mymenu <= 15)) t2set[mymenu - 13] += 1.0;
    if((mymenu >= 16) && (mymenu <= 18)) k2set[mymenu - 16] += 1.0;
    print_menu(menu);
  }
  else if((currentState[1] && !lastState[1]))
  {
    switch(mymenu)
    {
      case 0: menu = 1; break;
      case 1: menu = 0; break;
      case 2: menu = 3; break;
      case 3: menu = 2; break;
      case 5: menu = 11; break;
      case 12: temperature[0] -= 1; break;
    }
    if((mymenu <= 11) && (mymenu >= 6)) menu--;
    if((mymenu >= 13) && (mymenu <= 15)) t2set[mymenu - 13] -= 1.0;
    if((mymenu >= 16) && (mymenu <= 18)) k2set[mymenu - 16] -= 1.0;
    print_menu(menu);
  }
  lastState[0] = currentState[0];
  lastState[1] = currentState[1];
}

void buttonSettingOK(unsigned int mymenu)
{
  currentState[2] = digitalRead(BUTTON_OK);
  if(currentState[2] && !lastState[2])
  {
    switch(mymenu)
    {
      case 1: menu = 2; break;
      case 2: menu = 4; break;
      case 3: menu = 5; break;
      case 4: menu = 19; uptime = 0; break;
    }
    if((mymenu >= 5) && (mymenu <= 11)) menu += 7;
    print_menu(menu);
  }
  lastState[2] = currentState[2];
}

void buttonSettingCancel(unsigned int mymenu)
{
  currentState[3] = digitalRead(BUTTON_CANCEL);
  if(currentState[3] && !lastState[3])
  {
    if((mymenu >= 2) && (mymenu <= 3)) menu -= 1;
    else if(mymenu == 4) menu = 2;
    else if((mymenu >= 5) && (mymenu <= 11)) menu = 3;
    else if((mymenu >= 12) && (mymenu <= 18)) menu -= 7;
    else if(mymenu == 19) {menu = 4; resetPID();}
    print_menu(menu);
  }
  lastState[3] = currentState[3];
}

void print_time()
{
  lcd.setCursor(0, 1);
  int minute = 0, second = 0, remaining = 0;
  float aux = 0;
  if(uptime == 1) temperature[2] = temperature[1];
  if(uptime <= t2set[0])
  {
    lcd.print("T HEAT : ");
    aux = (temperature[0] - temperature[1])/t2set[0];
    remaining = t2set[0] - uptime;
    moving_setpoint =  temperature[1] + aux * uptime;
  }
  else if (uptime <= (t2set[0] + t2set[1]))
  {
    lcd.print("T MNTN : ");
    remaining = (t2set[0] + t2set[1]) - uptime;
  }
  else if (uptime <= (t2set[0] + t2set[1] + t2set[2]))
  {
    lcd.print("T COOL : ");
    aux = (temperature[2] - temperature[1]) / (t2set[2]);   
    moving_setpoint = aux * (uptime - (t2set[0] + t2set[1])) + temperature[1];
    remaining = (t2set[0] + t2set[1] + t2set[2]) - uptime;
  }
  Serial.println(moving_setpoint);
  PID();
  minute = remaining / 60;
  second = remaining % 60;
  if(minute <= 9) lcd.print("0");
  lcd.print(minute);lcd.print(":");
  if(second <= 9) lcd.print("0");
  lcd.print(second);
}
void PID()
{
  Err = moving_setpoint - temperature[1];
  sumErr = (sumErr + Err) * dt;
  dErr = (Err - lastErr) / dt;
  output = (k2set[2] * Err) + (k2set[1] * lastErr) + (k2set[0] * dErr);
  output = constrain(output, 0, 255);
  analogWrite(HEAT_PIN, int(output));
  lastErr = Err;
}
void resetPID(){Err = 0.0; lastErr = 0.0; sumErr = 0.0; dErr = 0.0; moving_setpoint = 0.0; output = 0.0; analogWrite(HEAT_PIN, 0);}
ISR(TIMER1_COMPA_vect)
{
  buttonSettingNAV(menu);
  buttonSettingOK(menu);
  buttonSettingCancel(menu);
}
void setTimer1()
{
  cli();
  TCCR1A = 0; TCCR1B = 0; TCCR1C = 0;
  TCCR1B |= 1 << WGM12;
  TCCR1B |= ((1 << CS10) | (1 << CS12));
  OCR1A = 155;
  TIMSK1 |= 1 << OCIE1A;
  sei();
}
