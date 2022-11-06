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

void setup (void)
{
  // отладка
  Serial.begin (115200);
  Serial.println ();
  master_arr [0] = 0;
  master_arr [1] = 0;
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
}                                         // Конец процедуры обмена


void loop (void)
{
  delay (1000);
  refreshSPI ();
  // цикл по slave_arr[]
  for (int i = 0; i < sizeof(slave_arr); i++) {
    Serial.println(slave_arr[i], DEC);
  }
  Serial.println('-');
}
