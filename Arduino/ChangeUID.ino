/*
• SPI.h:           Librería que nos permitirá implementar un protocolo de datos en serie síncrono que es utilizado por microcontroladores para comunicarse 
                   con uno o más dispositivos periféricos rápidamente a distancias cortas.
• MFRC522.h:       Librería que se utiliza para poder comunicarse con el Módulo Lector RFID. 
                   Mediante esta librería será posible identificar las tarjetas RFID al momento de leerlas.
• LiquidCrystal.h: Esta librería le permite al Arduino utilizar pantallas LCD. 
                   Básicamente se utilizará esta librería para desplegar en la pantalla mensajes para interactuar con el mundo físico.
• Servo.h:         Esta librería permite a un Arduino controlar los servomotores. 
                   Los servos tienen engranajes integrados y un eje que puede ser controlado con precisión. 
                   Los servos estándar permiten que el eje se posicione en varios ángulos, generalmente entre 0 y 180 grados. 
                   Mediante esta librería se controlará el eje del mismo para abrir una cerradura de una puerta, con el fin de simular la abertura de una cerradura.
*/
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <Servo.h>

#define RST_PIN   D1//Representa uno de los pines que se conectará al Módulo Lector RFID
#define SS_PIN    D8 //Representa uno de los pines que se conectará al Módulo Lector RFID

byte readCard[4];// Es un arreglo tipo byte que se encargará de guardar una tarjeta RFID que se acaba de leer.
char* myTags[100] = {};// Es el arreglo en el que se guardarán todas las tarjetas RFID que se agreguen.
int tagsCount = 0;// Es una variable que nos servirá para llevar el control de cuantas tarjetas RFID se han leído.
String tagID = "";// Es una variable que se encargará de guardar la tarjeta RFID leída como un string.
boolean successRead = false;// Es una variable que se usará como bandera para saber si una lectura fue exitosa o no.
boolean correctTag = false;// Es una variable que se usará como bandera para saber si una tarjeta RFID es válida o no.


MFRC522 mfrc522(SS_PIN, RST_PIN);// Se crea la instancia para el Módulo Lector RFID con los pines definidos anteriormente.
//Parametros: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // Se crea la instancia para el LCD con los pines 2-7 del Arduino.
Servo myServo; // Se crea la instancia para el Servo motor

void setup() {
  Serial.begin(115200); //Iniciamos la comunicación serial a 9600 baudios
  SPI.begin();        // Se inicializa el protocolo SPI
  mfrc522.PCD_Init(); // Se inicializa el módulo lector RFID: MFRC522
  lcd.begin(16, 2);   // Se inicializa la pantalal lcd indicando que es de 16x2 caracteres
  myServo.attach(8);  // Se indica que el servo motor esta conectado al pin 8
  myServo.write(10); // Se establece la posición inicial(posición Lock) del servo motor
  //A continuación se establece la primera tarjeta RFID (Tarjeta Maestra)
  myTags[tagsCount] = strdup("BE7E89AB"); 
  tagsCount++; //Se aumenta en 1 tagsCount
  successRead = false;//Se establece la bandera en falso
  printNormalModeMessage();//Se manda llamar la función (Se explicará más adelante)
}

void loop() {
    // Se Prepara para leer los PICCs
    if ( ! mfrc522.PICC_IsNewCardPresent()) { 
      return;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) {   
      return;
    }
    tagID = "";
    for ( uint8_t i = 0; i < 4; i++) {  // Las MIFARE PICCs que usamos tienen 4 bytes UID
      readCard[i] = mfrc522.uid.uidByte[i]; // Se lee cada byte de la tarjeta RFID
      tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); //Añade cada byte a una sola variable String
    }
    tagID.toUpperCase();//Modifica los caracteres a mayúsculas
    for (byte i = 0; i < mfrc522.uid.size; i++) { // Se manda a imprimir la Tarjeta en el Serial
                          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                          Serial.print(mfrc522.uid.uidByte[i], HEX);   
    }
    mfrc522.PICC_HaltA(); // Deja de leer
    correctTag = false; // La bandera se iguala a falso
    // Checa si la tarjeta escaneada es la Tarjeta Maestra
    if (tagID == myTags[0]) {
      //Se imprimen mensajes en el LCD
      lcd.clear();
      lcd.print("Program mode:");
      lcd.setCursor(0, 1);
      lcd.print("Add/Remove Tag");
      while (!successRead) {//Mientras no haya una lectura
        successRead = getID();//Si existe la lectura exitosa getID retornará un 1 que puede ser un True
        if ( successRead == true) {//Si la lectura es exitosa
            //Aquí verificamos si la Tarjeta que se leyó es la Tarjeta Maestra
            //Es importante verificar esto, ya que, si se entra al modo "Programa"
            //No tiene porque borrarse la Tarjeta Maestra o volverla añadir
            //Así que, simplemente se imprime un mensaje y se llega a un return para volver al loop()
            if(tagID == myTags[0]){
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("This is the");
                lcd.setCursor(0 ,1);
                lcd.print("    Master Tag!");
                return;
            }
            //Si la tarjeta leída no es la Maestra, entonces nunca se llega al return 
            //A continuación se verifica si la tarjeta leída ya existe en el sistema
            for (int i = 1; i < 100; i++) {//Se recorren todas las tarejtas
              if (tagID == myTags[i]) {//Si la tarjeta leída ya existe
                //La tarjeta se borrará y con el return volveremos al loop()
                myTags[i] = "";//La tarjeta se borra
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("  Tag Removed!");
                //Se imprime en el serial la tarjeta borrada
                for (byte i = 0; i < mfrc522.uid.size; i++) {
                  Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                  Serial.print(mfrc522.uid.uidByte[i], HEX);   
                } 
                delay(1000);        // ...por un 1 segundo
                printNormalModeMessage();
                successRead = false;// La bandera se iguala a falso
                return;
              }
            }
          //Si no existe la tarjeta, entonces se tiene que agregar.
          //Llegará a esta parte del proceso ya que si no existe nunca llega al return anterior
          myTags[tagsCount] = strdup(tagID.c_str());//Se agrega la tarjeta con todos los caracteres en mayúsculas
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("   Tag Added!");
          delay(1000);        // ...por 1 segundo
          //Se imprime en el serial la tarjeta añadida
          for (byte i = 0; i < mfrc522.uid.size; i++) {
            Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
            Serial.print(mfrc522.uid.uidByte[i], HEX);   
          } 
          delay(1000);        // ...por 1 segundo
          printNormalModeMessage();
          tagsCount++;//Se aumenta la cantidad de tarjetas que ya existen
          successRead = false;// La bandera se iguala a falso
          return;
        }
      }
    }
    successRead = false;// La bandera se iguala a falso
    // Checa si la tarjeta leída es autorizada
    for (int i = 1; i < 100; i++) {//Se recorren todas las tarejtas
      if (tagID == myTags[i]) {//Si la tarjeta leída ya existe entonces está autorizada
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" Access Granted!");
        delay(1000);        // ...por 1 segundo
        myServo.write(100); // Abre la puerta
        delay(5000); //Se espera 5 segundos
        myServo.write(10); // Cierra la puerta
        printNormalModeMessage();
        correctTag = true; // Si existe hacemos la bandera igual a true
      }
    }
    if (correctTag == false) {//Si la bandera es igual a falso, no existe
      //Se imprime un mensaje de acceso denegado
      lcd.clear();
      lcd.setCursor(0, 0);
      delay(1000);        
      lcd.print(" Access Denied!");
      printNormalModeMessage();
    }
}

/*
Función que sirve para leer una tarjeta RFID
 */
uint8_t getID() {
  // Se Preparar para leer los PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { 
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   
    return 0;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) {  // Las MIFARE PICCs que usamos tienen 4 bytes UID
    readCard[i] = mfrc522.uid.uidByte[i]; // Se lee cada byte de la tarjeta RFID
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); //Añade cada byte a una sola variable String
  }
  tagID.toUpperCase();//Modifica los caracteres a mayúsculas
  mfrc522.PICC_HaltA(); // Detiene la lectura
  return 1;
}

/*
Esta función solamente imprime en el LCD un par de palabras para que se comunique con el mundo exterior 
y sirve para ahorrar espacio en el código. Básicamente imprime “Access Control”.
*/
void printNormalModeMessage() {
  delay(1500);
  lcd.clear();
  
  lcd.print("-Access Control-");
  lcd.setCursor(0, 1);
  
  lcd.print(" Scan Your Tag!");
}

