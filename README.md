# Arduino-uno-ds1307-rtc-i2c

Proyecto para poner en fecha/hora un módulo de reloj en tiempo real con pila
Tiny RTC desde un Arduino UNO

Simplemente, establecer los valores y subir el sketch para ejecutarlo.
Esto añadirá la hora en el arduino y parará la ejecución de forma que tras
encender esperamos unos segundos hasta encender la luz integrada de forma fija
en el arduino ya que eso indicará el final del proceso.

Mediante Serial se puede depurar el proceso de establecer fecha/hora ya que se
prepara el código para este fin teniendo aproximadamente 1 segundo cada
iteración.

El proceso de subida puede tardar entre 7-10 segundos, recomiendo tenerlo en
cuenta visitando alguna web que muestre el reloj en tiempo real para
establecerlo y darle a subir sketch ese tiempo antes de forma que nos aseguramos
estar totalmente sincronizados con el tiempo exacto.

Este código está preparado para utilizarse mediante platformio.
