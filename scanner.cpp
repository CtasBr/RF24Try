#include <cstdlib>
#include <iostream>
/*
 * Подключаем файл настроек из библиотеки RF24
 */
#include <RF24/nRF24L01.h>
/*
 * Подключаем библиотеку для работы с nRF24L01+
 */
#include <RF24/RF24.h>

using namespace std;

/*
 * Конфигурация оборудования
 */
#define BCM_PIN 7
#define SPI_DEV 0
#define NUM_CHANNELS 126

/*
 * Информация о канале
 */
uint8_t values[NUM_CHANNELS];
const int numReps = 100;

int main() {
  /*
   * Создаём объект radio для работы с библиотекой RF24,
   * указывая номера вывода CE и SPI порта
   */
  RF24 radio(BCM_PIN, SPI_DEV);

  /*
   * Инициируем работу nRF24L01+
   */
  radio.begin();

  radio.setAutoAck(false);

  /*
   * Вход в режиме ожидания
   */
  radio.startListening();
  radio.stopListening();

  /*
   * Дамп конфигурации RF для отладки
   */
  radio.printDetails();

  /*
   * Распечатка заголовка, верхний и нижний разряд
   */
  for (int i = 0; i < NUM_CHANNELS; ++i) {
    printf("%x", i >> 4);
  }
  cout << endl;
  for (int i = 0; i < NUM_CHANNELS; ++i) {
    printf("%x", i & 0xf);
  }
  cout << endl;

  while (1) {
    /*
     * Очистка значений измерений
     */
    memset(values, 0, sizeof(values));

    /*
     * Сканирование всех каналов numReps раз
     */
    for (int k = 0; k < numReps; ++k) {
      for (int i = 0; i < NUM_CHANNELS; ++i) {

        /*
         * Выбор канала
         */
        radio.setChannel(i);

        /*
         * Послушать немного
         */
        radio.startListening();
        delayMicroseconds(128);
        radio.stopListening();

        /*
         * Проверка наличия несущей частоты на выбранном канале (частоте).
         */
        if (radio.testCarrier()) {
          ++values[i];
        }
      }
    }
    /*
     * Распечатка измерения канала в одну шестнадцатеричную цифру
     */
    for (int i = 0; i < NUM_CHANNELS; ++i) {
      printf("%x", min(0xf, (values[i] & 0xf)));
    }
    cout << endl;
  }
  return 0;
}