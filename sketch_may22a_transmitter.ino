int k=0;

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  }

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  
  k = k+1;
  Serial1.write("test de communication ");
  Serial1.println(k);
  Serial1.write("\n");
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  }
