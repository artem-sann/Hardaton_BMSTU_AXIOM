#include <LiquidCrystalRus.h>
#include<SPI.h>

constexpr uint8_t PIN_RS = 3;
constexpr uint8_t PIN_EN = 4;
constexpr uint8_t PIN_DB4 = 5;
constexpr uint8_t PIN_DB5 = 6;
constexpr uint8_t PIN_DB6 = 7;
constexpr uint8_t PIN_DB7 = 8;

#define RED A3
#define GREEN A2
#define BLUE A1
#define Light_LED A6

// Массив корторый приходит от мастера
volatile uint8_t master_arr [7]; // ex: "95[0] 45[1]" - тип топлива и количество
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
  //---------------------------------LED setup----------------------------------------------->
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(Light_LED, OUTPUT);
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
  digitalWrite(Light_LED, HIGH);
  if (start_fuel) {
    digitalWrite(Light_LED, LOW);
    if (master_arr[0]==92){
        digitalWrite(BLUE, HIGH);
    }
    else if(master_arr[0]==95) {
        digitalWrite(RED, HIGH);
    }
    else if(master_arr[0]==80) {
        digitalWrite(GREEN, HIGH);
    }
    else if(master_arr[0]==98) {
        digitalWrite(GREEN, HIGH);
        digitalWrite(RED, HIGH);
        digitalWrite(BLUE, HIGH);  
    }

    lcd.clear();
    char mess_1[16] = "Typ AI00 Cst00p";
    char mess_2[16] = "00/00..........";

    mess_2[3] = (master_arr[1]/10)+48;
    mess_2[4] = (master_arr[1]%10)+48;
    mess_1[6] = (master_arr[0]/10)+48;
    mess_1[7] = (master_arr[0]%10)+48;
    int cost = master_arr[0]/2;
    mess_1[12] = (cost/10)+48;
    mess_1[13] = (cost%10)+48;

    Serial.println(mess_1);
    Serial.println(mess_2);
    lcd.setCursor(0, 0);
    lcd.print(mess_1);
    lcd.setCursor(0, 1);
    lcd.print(mess_2);

    delay(100);
    for (int i = 0; i < 10; i++){
      mess_2[5+i] = '#';
      float to_load = master_arr[1]/10 + master_arr[1]/10*i;
      int to_indicate = to_load;

      mess_2[0] = (to_indicate/10)+48;
      mess_2[1] = (to_indicate%10)+48;

      Serial.println(mess_2);
      lcd.setCursor(0, 1);
      lcd.print(mess_2);

      delay(400);
    }
    mess_2[0] = (master_arr[1]/10)+48;
    mess_2[1] = (master_arr[1]%10)+48;

    Serial.println(mess_2);
    lcd.setCursor(0, 1);
    lcd.print(mess_2);

    Serial.println("done");
    delay(100);
  }
  start_fuel = false;
  digitalWrite(GREEN, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(BLUE, LOW);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Заправлено");
  lcd.setCursor(0, 1);
  lcd.print("Доброго пути");

  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Отскан. QR код");
  lcd.setCursor(0, 1);
  lcd.print("Поиск бензобака");
}
