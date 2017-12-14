/*
 UsbReset - Программа перезагрузчик зависшего компа
 
 Программа получает данные из порта, как только данные перестают поступать в порт начинается обратный отсчет.
 Как только величина delta становится меньше нуля происходит замыкание реле которое замыкает контакты Reset на компьютерее

 created 13 December 2017
 by Nick Korbut (nick.korbut@gmail.com)
 
*/

#include <DigiCDC.h> 

bool resetState; // состояние перезагрузки
int delta;       // число циклов по истечению которого будет перезагрузка

void setup() {                

    resetState = false; // состояние - перезагрузки нет
    delta = 30000;      // это число примерно соответствует 27 - 29 секунд
      
    SerialUSB.begin();  // поднимаем порт
      
    // устанавливаем пины
    pinMode(1, OUTPUT); // светодиод на плате
    digitalWrite(1, LOW);  // гасим светодиод
    
    pinMode(0, OUTPUT); // нога для реле
    digitalWrite(0, HIGH); // HIGH - разомкнуто, LOW - замкнуто 
    

}

// the loop routine runs over and over again forever:
void loop() {
    
    if (SerialUSB.available()) { // если в компорт валят какие то данные
      
      // SerialUSB.write(SerialUSB.read()); // выводим обратон то что пришло в порт в рабочей версии надо закоментировать
      
      delta = 30000;             // устанавливаем 
      digitalWrite(1, LOW);      // гасим светодиод
      digitalWrite(0, HIGH);     // размыкаем реле
      resetState = false;        // устанавливаем статус - "нормальная работа не перегружаемся"
    
    } else { // если данные не поступают
      
      delta = delta - 1; 
      
      if(delta < 0){ // если дельта истекла
        
        if(!resetState){ // и статус "нормальная работа не перегружаемся" (FALSE)
            
            digitalWrite(1, HIGH); // зажигаем светодиод
            digitalWrite(0, LOW);  // замыкаем реле
            delay(500);            // чуть-чуть ждем
            digitalWrite(0, HIGH); // размыкаем реле
            // SerialUSB.write(" RESET : "); // выводим для тестированиея что мы ресетимся, в рабочей версии надо закоментировать
            resetState = true;     // устанавливаем статус "Перезагрузка"
             
        } //end if resetState
              
      } //end if delta 
         
    } //end else

} // end of loop


