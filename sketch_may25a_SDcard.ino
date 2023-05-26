#include <SPI.h>
#include <SD.h>

char value=0;
int k=0;

// ATTENTION : il faut bien choisir le pin. Il semble que l'utilisation du pin 10 pose problème.
const int chipSelectPin = 4; // Broche CS du shield
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// MKRZero SD: SDCARD_SS_PIN

void setup() {
  // Initialisation de la communication SPI
  //SPI.begin();
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("\n");
  Serial.println("Initialisation de la carte SD...");

  // Vérification de la présence de la carte SD
  if (!SD.begin(chipSelectPin)) {
    Serial.println("Erreur lors de l'initialisation de la carte SD. Vérifiez que la carte est bien insérée.");
    while (1);
  }
    Serial.println("Carte SD initialisée avec succès.");

    Serial.println("------------------------------");
}

void loop() {
  
  // // make a string for assembling the data to log:
  // k=k+1;
  // String dataString = "test" + String(k);

  // Assemblage des donnees a logger:
  String dataString = "";
  
  // si on recoit quelque chose de LEONARDO:
  //for (int i=0; i<27; i++){
    if(Serial.available()>0){
      value=Serial.read();

      //-----------------------------------------------------------------
      //Serial.print("data recues : "); // Affichage dans le Serial Monitor
      //Serial.print(value);            
      //-----------------------------------------------------------------

      dataString += String(value); //Ajout à la string de données a enregistrer
    
  //}

  

      // Ouverture du fichier sur la carte SD en mode écriture
      // Rq: 1 seul fichier peut être ouvert à la fois, il faudra fermer celui-là si on veut en ouvrir un autre.
      File dataFile = SD.open("data001.txt", FILE_WRITE);
      //ATTENTION : la taille du nom de fichier ne peut pas exceder 8 caractères
      
      //Debuggage
      // if (SD.exists("data_test.txt")) {
      //   Serial.println("data_test exists. ");
      // }

      // Vérification si le fichier a pu être ouvert
      if (dataFile) {
        // Écriture des données dans le fichier
        //dataFile.println(dataString);
        dataFile.print(value);

        //-----------------------------------------------------------------
        // Serial.print("data ecrite : "); // Affichage dans le Serial Monitor
        // Serial.println(dataString);
        Serial.print(value);
        //-----------------------------------------------------------------

        // Fermeture du fichier
        dataFile.close();
        
        //Serial.println("Donnees ecrites avec succes !");
      } 
      else {
        Serial.println("Erreur lors de l'ouverture du fichier.");
        while(1);
      }
  }
  
}
