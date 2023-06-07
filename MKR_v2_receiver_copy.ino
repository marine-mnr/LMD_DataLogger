char value=0;

void setup() {
  // start serial port at 9600 bps and wait for port to open:
  Serial.begin(19200); //monitor
  Serial1.begin(9600); //uart
  //pinMode(LED_BUILTIN, OUTPUT);
  //Serial.print("je vous écoute...");
  //Serial.println(); //retour à la ligne
}

//RECEPTEUR//
void loop() {
  // si on recoit quelque chose :
  if(Serial1.available()>0){
    //digitalWrite(LED_BUILTIN, HIGH);
    value=Serial1.read();
    Serial.print(value);

    if(value=='\r'){    //13=0x0D=\r=carriage return
      Serial.print('\n');
    }

    //delay(500);  // Délai pour visualiser la réception sur la LED
    //digitalWrite(LED_BUILTIN, LOW);
 };
}
