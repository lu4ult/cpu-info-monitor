//LCD
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);


String inputString = "";               // a String to hold incoming data
bool stringComplete = false;          // whether the string is complete
unsigned long lastDataReceived = 0;
bool backlighEncendido = 1;

void setup() {
  Serial.begin(9600);
  inputString.reserve(200);      // reserve 200 bytes for the inputString:

  //LCD:
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.print("Hola mundo!");
}

void loop() {
  if(stringComplete) {

    /* Para evitar tener que formatear los textos aquí en Arduino, lo que hacemos es recibir directamente un texto de 80 caracteres (4 líneas por 20 caracteres)
     * luego lo dividimos en sus cuatro líneas y publicamos directamente en el displya. Tambien, posicionamos el cursor y no usamos lcd.clear() y de esta forma evitamos el parpadeo clásico.
     * De esta forma podemos escribir directamente en python cómo se verán los textos (centrados, a la izquierda, etc) y es más fácil modificarlos allí que estar reprogramando el arduino.
     */

    lcd.setCursor(0,0);
    lcd.print(inputString.substring(0,20));
    lcd.setCursor(0,1);
    lcd.print(inputString.substring(20,40));
    lcd.setCursor(0,2);
    lcd.print(inputString.substring(40,60));
    lcd.setCursor(0,3);
    lcd.print(inputString.substring(60,80));
    inputString = "";                               //Vaciamos el string antes de empezar a guardar los nuevos caracteres que están entrando.
    stringComplete = false;                         //Flag para que reciba el siguiente mensaje

    if(backlighEncendido == false) {
      backlighEncendido = true;
      lcd.backlight();
    }
  }

  //Si hace más de un minuto que no recibimos datos mostramos un mensaje indicándolo
  if(millis() - lastDataReceived >= 1*60*1000  and lastDataReceived) {
    lcd.clear();
    lcd.setCursor(5,1);
    lcd.print("Sin datos!");
    lcd.setCursor(2,3);
    lcd.print("lu4ult.github.io");
    delay(1000);
  }

  //Y a los tres minutos de estar sin recibir datos apagamos la retroiluminación (backlight) del display
  if(millis() - lastDataReceived >= 3*60*1000 and lastDataReceived) {
    backlighEncendido = false;
    lcd.noBacklight();
    lastDataReceived = 0;
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();

    if(inChar != '\n' and inChar != '\r')       //Vamos armando el texto recibido excepto los caracteres de salto de línea y retorno
      inputString += inChar;

    if (inChar == '\n') {                       //Cuando recibimos el salto de línea indicamos que el texto se recibió por completo y está lito para mostrarlo en pantalla
      stringComplete = true;
      lastDataReceived = millis();
    }
  }
}
