// Данный код будет работать только на платах с чипом ATmega328P например
// Arduino UNO, NANO
// Он использует интрефейс SPI на пинах
// 13 - SCK, 12 - MISO, 11 - MOSI, 10 - SS

// Массивы могут иметь произвольную длинну
// Но должны совпадать по количеству байт
// Не использованные байты можно оставлять пустыми

// Массив который отправляем ведомому
uint8_t master_arr [7];
// Массив корторый получаем от ведомого
uint8_t slave_arr  [7];

String string_from_rasp = "+000;+000;+000;+000;00;00"; // srvo1;servo2;step1;step2;fuel_type;fuel_volume

int delta_srv1 = 0;
int delta_srv2 = 0;
int delta_step1 = 0;
int delta_step2 = 0;
uint8_t fuel_typ = 0;
uint8_t fuel_vol = 0;


void setup (void)
{
  // отладка
  Serial.begin (115200);
  Serial.println ();
  master_arr [0] = 92;
  master_arr [1] = 46;
  master_arr [2] = 0;
  master_arr [3] = 1;
  master_arr [4] = 0;
  master_arr [5] = 0;
  master_arr [6] = 0;
  
  // Настройка SPI как мастера
  DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB5);   // Настроить выводы MOSI,SS,SCK на выход            
  PORTB |= (1 << PB2);                            // Установить "1" на линии SS
  /* Enable SPI, Master, set clock rate fck/16 */
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}


void refreshSPI ()                        // Процедура обмена массивами между платами
{
  PORTB &= ~(1 << PB2);                   // Установить "0" на линии SS1
  SPDR = 0xFF;                            // Отправляем "пустышку" для загрузки байта slave_arr[0]
  while (!(SPSR & (1 << SPIF))) ;         // Дождаться окончания передачи
  delayMicroseconds (20);                 // Пауза чтобы успел подгрузиться байт [0] на ведомом
  // цикл по master_arr[]
  for (uint8_t i = 0; i < sizeof(master_arr); i++) {
    SPDR = master_arr[i];                 // Отправить байт
    while (!(SPSR & (1 << SPIF))) ;       // Дождаться окончания передачи
    slave_arr[i] = SPDR;                  // Принять байт
    delayMicroseconds (20);               // Пауза чтобы успел подгрузиться байт на ведомом
  }
  PORTB |= (1 << PB2);                    // Установить "1" на линии SS
} 


void message_parser(String input, int *delta_srv1, int *delta_srv2, int *delta_step1, int *delta_step2, uint8_t *fuel_typ, uint8_t *fuel_vol) {
  if (input[0] == '+'){
    *delta_srv1 = (input[1]-48)*100 + (input[2]-48)*10 + (input[3]-48);
  }
  else {
    *delta_srv1 = -(input[1]-48)*100 - (input[2]-48)*10 - (input[3]-48);   
  }

  if (input[5] == '+'){
    *delta_srv2 = (input[6]-48)*100 + (input[7]-48)*10 + (input[8]-48);
  }
  else {
    *delta_srv2 = -(input[6]-48)*100 - (input[7]-48)*10 - (input[8]-48);   
  }

  if (input[10] == '+'){
    *delta_step1 = (input[11]-48)*100 + (input[12]-48)*10 + (input[13]-48);
  }
  else {
    *delta_step1 = -(input[11]-48)*100 - (input[12]-48)*10 - (input[13]-48);   
  }

  if (input[15] == '+'){
    *delta_step2 = (input[16]-48)*100 + (input[17]-48)*10 + (input[18]-48);
  }
  else {
    *delta_step2 = -(input[16]-48)*100 - (input[17]-48)*10 - (input[18]-48);   
  }

  *fuel_typ = (input[20]-48)*10 + (input[21]-48);
  *fuel_vol = (input[23]-48)*10 + (input[24]-48);
}                                        


void loop (void)
{
  delay (1000);
  refreshSPI ();

  if (Serial.available() > 0) {
    string_from_rasp = Serial.readStringUntil('\n');
    message_parser(string_from_rasp, &delta_srv1, &delta_srv2, &delta_step1, &delta_step2, &fuel_typ, &fuel_vol);
  }
  

  master_arr [0] = fuel_typ;
  master_arr [1] = fuel_vol;

  Serial.println(delta_step2);

}
