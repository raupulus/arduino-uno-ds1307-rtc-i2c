#include <Arduino.h>
#include <Wire.h> // i2c

// Pausa entre loops
const int DELAYVAL = 999;

// RTC DS1307 (RELOJ)
#define DS1307_I2C_ADDRESS 0x68

byte second, minute, hour, dayOfWeek, dayOfMonth, month, year; // Devuelve un int preparado para ser mostrado por la pantalla directamente

/**
 * Convierte un numero decimal a su codificación en binario.
 */
byte decToBcd(byte val)
{
  return ((val / 10 * 16) + (val % 10));
}

/**
 * Convierte un binario (decimal codificado) en un número decimal puro.
 */
byte bcdToDec(byte val)
{
  return ((val / 16 * 10) + (val % 16));
}

/**
 * Agrega timestamp al módulo RTC DS1307 (RELOJ)
 */
bool setDateTimeRTC(byte second, byte minute, byte hour, byte dayOfWeek,
                    byte dayOfMonth, byte month, byte year)
{
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0x00);                    // set next input to start at the seconds register
  Wire.write(decToBcd(second) & 0x7F); // set seconds
  Wire.write(decToBcd(minute));        // set minutes
  Wire.write(decToBcd(hour));          // set hours
  Wire.write(decToBcd(dayOfWeek));     // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth));    // set date (1 to 31)
  Wire.write(decToBcd(month));         // set month
  Wire.write(decToBcd(year));          // set year (0 to 99)

  // Cierra la transmisión e indica si se hizo correctamente.
  if (Wire.endTransmission() != 0)
  {
    Serial.print("DS1307 → La transmisión para establecer hora ha fallado.");
    return false;
  }

  Serial.print("DS1307 → Se ha establecido la hora correctamente.");

  return true;
}

/**
 * Lee el módulo RTC DS1307 (RELOJ)
 */
void getDateTimeRTC(byte *second, byte *minute, byte *hour, byte *dayOfWeek,
                    byte *dayOfMonth, byte *month, byte *year)
{
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0); // Set DS1307 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

// Muestra datos por consola del módulo RTC DS1307 (RELOJ)
void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  getDateTimeRTC(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  // send it to the serial monitor
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");

  // TENER EN CUENTA LOS cambios de horario:
  // sabado 28 de marzo al domingo 29 (2:00) → UTC + 2h
  // Sabado 25 de Octubre al domingo 26 (3:00) → UTC + 1h

  if (minute < 10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second < 10)
  {
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Día de la semana: ");

  switch (dayOfWeek)
  {
  case 1:
    Serial.println("Domingo");
    break;
  case 2:
    Serial.println("Lunes");
    break;
  case 3:
    Serial.println("Martes");
    break;
  case 4:
    Serial.println("Miércoles");
    break;
  case 5:
    Serial.println("Jueves");
    break;
  case 6:
    Serial.println("Viernes");
    break;
  case 7:
    Serial.println("Sábado");
    break;
  }
}

void setup()
{
  // Inicio Serial
  Serial.begin(9600);

  Serial.println("Iniciando Código");

  // Apago led integrado en arduino
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Preparar la librería Wire (I2C)
  Serial.println("Iniciando librería I2C");
  Wire.begin();
  delay(1000);

  // RTC DS1307 (RELOJ) - Inicialización en hora UTC.
  Serial.println("Seteando Reloj");
  setDateTimeRTC(00, 43, 17, 6, 31, 12, 21); // seconds, minutes, hours, day, date, month, year

  delay(1000);

  // Enciendo led integrado en arduino indicando que se ha cargado
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
  Serial.println("Iteración en bucle");
  // Leo DateTime del módulo RTC.
  getDateTimeRTC(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

  // RTC
  displayTime();

  // Pausa entre iteraciones
  delay(DELAYVAL);
}