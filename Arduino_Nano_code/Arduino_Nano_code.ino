#include <LiquidCrystalRus.h>
#include<SPI.h>

constexpr uint8_t PIN_RS = 3;
constexpr uint8_t PIN_EN = 4;
constexpr uint8_t PIN_DB4 = 5;
constexpr uint8_t PIN_DB5 = 6;
constexpr uint8_t PIN_DB6 = 7;
constexpr uint8_t PIN_DB7 = 8;

// Массив корторый приходит от мастера
volatile uint8_t master_arr [7]; // ex: "0001000" - тип топлива и количество
// Массив который отдаем от мастеру
volatile uint8_t slave_arr  [7];
// счетчик входящих байт
volatile int16_t countSPIb = -1;

bool start_fuel = false;

LiquidCrystalRus lcd(PIN_RS, PIN_EN, PIN_DB4, PIN_DB5, PIN_DB6, PIN_DB7);

void setup() {
  //---------------------------------LCD setup---------------------------------------------->
  lcd.begin(16, 2);
  // Устанавливаем курсор в колонку 0 и строку 0
  lcd.setCursor(0, 0);
  // Печатаем первую строку
  lcd.print("Отскан. QR код");
  // Устанавливаем курсор в колонку 0 и строку 1
  lcd.setCursor(0, 1);
  // Печатаем вторую строку
  lcd.print("Поиск бензобака");
  //----------------------------------------------------------------------------------------->
  Serial.begin(9600);
  //---------------------------------SPI setup----------------------------------------------->
  // Настройка SPI как SLAVE
  DDRB|=(1<<PB4);                        // Настроить вывод MISO на выход
  SPCR |= (1 << SPIE)|(1 << SPE);        // Прерывание включено и сам SPI как SLAVE

  // отладка
  slave_arr [0] = 100;
  slave_arr [1] = 200;
  slave_arr [2] = 250;
  //----------------------------------------------------------------------------------------->
}


ISR (SPI_STC_vect)                       // Прерывание SPI - пришел байт
{
  if (countSPIb < 0) {                   // пришла "пустышка"
    countSPIb++;                         // увеличивам счетчик
    SPDR = slave_arr [countSPIb];        // подгружаем нулевой байт массива ведомого
    return;                              // выходим из процедуры
  }

  master_arr [countSPIb] = SPDR;         // получаем байт от мастера
  countSPIb++;                           // увеличиваем счетчик
  SPDR = slave_arr [countSPIb];          // отдаем байт ведомого (+1 индекс)

  if (countSPIb >= sizeof(master_arr)) { // если кончился массив
    countSPIb = -1;                      // обнуляем счетчик и ждем следующий обмен
  }
  start_fuel = true;
}                                        // Конец SPI - пришел байт


void loop() {
  if (start_fuel) {
    lcd.setCursor(0, 1);
    lcd.clear();
    lcd.print("Начинаем запр.");
    Serial.println("done");
    delay(100);
  }
  
}
