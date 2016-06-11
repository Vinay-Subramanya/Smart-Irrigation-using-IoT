#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = {169, 254, 176, 111 };
byte server[] = { 169, 254, 176, 106};
EthernetClient client;

int TemperatureValue;
int MoistureValue;
int MotorRelay = 10;
int LED1 = 11;
int LED2 = 12;
boolean MotorState = false;

void setup()
{
      pinMode(LED1, OUTPUT);
      pinMode(LED2, OUTPUT);
      pinMode(MotorRelay, OUTPUT);
      Serial.begin(9600); 
      Ethernet.begin(mac,ip);
 }


void loop()
{
  
    if (client.connect(server, 80))
   {
    Serial.println("HELLO");
    MotorState = false;
    MoistureValue = analogRead(A0);
    TemperatureValue = analogRead(A1);
    float mv = ( TemperatureValue/1024.0)*5000; 
    float celsius = mv/10;
    
    Serial.print("MoistureValue =  "); 
    Serial.print(MoistureValue);
    Serial.print(" , TemperatureValue(deg C) = ");
    Serial.println(celsius);
    
    client.print("GET /arduino/add.php?");
    client.print("moisture=");
    client.print(MoistureValue);
    client.print("&&");
    client.print("temperature=");
    client.print(celsius);
    client.print("&&");
    client.print("motorstate=");
    client.print(MotorState);
    client.println();

    digitalWrite(MotorRelay, HIGH);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);

    if (MoistureValue > 800)
    {
          digitalWrite(LED1, HIGH);
    //      delay(100);
          
          digitalWrite(MotorRelay, LOW);
          MotorState = true;
          Serial.println("Watering");
          
          delay(3000);
          
          // run pump for 3 seconds
          
          Serial.println("Finished watering");
     }

       if (celsius > 20 )
      {
            digitalWrite(LED2, HIGH);
      //      delay(100);
      //      digitalWrite(LED2, LOW);;
      //      delay(100);
      }
    delay(10000);
    // delay 10 second between reads
    
    client.stop();
  } 
  else 
  {
      Serial.println("connection failed");
         
  }
 
 
}
