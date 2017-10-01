
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire dc(12);
DallasTemperature sen_1(&dc);

int ch = 0;
String ts;
String val = "";
void sms(String , String );

void setup() {
  delay(2000);  //время на инициализацию модуля
  sen_1.begin();
  
  Serial.begin(9600);  //скорость порта
  Serial.println("GSM tester v1.0");
  Serial1.begin(9600);
  Serial1.println("AT+CLIP=1");  //включаем АОН
  delay(100);
  Serial1.println("AT+CMGF=1");  //режим кодировки СМС - обычный (для англ.)
  delay(100);
  Serial1.println("AT+CSCS=\"GSM\"");  //режим кодировки текста
  delay(100);
}

void loop() {
  if (Serial1.available()) {  //если GSM модуль что-то послал нам, то
    while (Serial1.available()) {  //сохраняем входную строку в переменную val
      ch = Serial1.read();
      val += char(ch);
      delay(10);
      
    }
    Serial.println(val);
    if (val.indexOf("RING") > -1) {  //если звонок обнаружен, то проверяем номер
      if (val.indexOf("9159485333") > -1) {  //если номер звонящего наш. Укажите свой номер без "+"
        Serial.println("--- MASTER RING DETECTED ---");
        Serial1.println("ATH0");  //разрываем связь 
        delay(1000);
       
        sen_1.requestTemperatures();
        ts="Temperatura "+String(int(sen_1.getTempCByIndex(0)),DEC)+" C.";
      sms(ts, String("+79159485333"));
       Serial.println(ts);
      
        val = "";
       
      };
      
      if (val.indexOf("9202528996") > -1) {  //если номер звонящего наш. Укажите свой номер без "+"
       // Serial.println("--- MASTER RING DETECTED ---");
        Serial1.println("ATH0");  //разрываем связь
        sen_1.requestTemperatures();
        ts="Temperatura "+String(int(sen_1.getTempCByIndex(0)),DEC)+" C.";
      sms(ts, String("+79202528996"));
       Serial.print(ts);
       Serial1.flush();
        val = "";
      };


      
    };
    val = "";
  }
  if (Serial.available()) {  //если в мониторе порта ввели что-то
    while (Serial.available()) {  //сохраняем строку в переменную val
     ch = Serial.read();
      val += char(ch);
      delay(10);
    }
Serial1.println(val);  //передача всех команд, набранных в мониторе порта в GSM модуль
//    if (val.indexOf("sendsms") > -1) {  //если увидели команду отправки СМС
//      sms(String("hello world"), String("+79159485333"));  //отправляем СМС на номер +71234567890
//    }
//    val = "";  //очищаем
  }
} 

void sms(String text, String phone)  //процедура отправки СМС
{
  Serial.println("SMS send started");
  Serial1.println("AT+CMGS=\"" + phone + "\"");
  delay(500);
  Serial1.println(text);
  delay(500);
  Serial1.print((char)26);
  delay(500);
  Serial.println("SMS send complete");
  delay(2000);
}
