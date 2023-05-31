//#include <SPI.h>
#include <SD.h>

//#define debug 0   //mode debuggage avec affichages: 1 si activé, 0 sinon
const bool debug=0; //mode debuggage avec affichages: 1 si activé, 0 sinon

const int baudrate = 9600; 
const int chipSelectPin = 4; // Broche CS du shield
/* ATTENTION : il faut bien choisir le pin. Il semble que l'utilisation du pin 10 pose problème.
  // Arduino Ethernet shield: pin 4
  // Adafruit SD shields and modules: pin 10
  // Sparkfun SD shield: pin 8
  // MKRZero SD: SDCARD_SS_PIN
*/
char value=0;
int k=0;

/* Fonction initSDcard
  // Paramètres :
  // - chipSelect : le numéro de broche utilisé pour la communication avec la carte SD
  // Retour :
  // - booléen : true si la carte SD est initialisée avec succès, false sinon
*/
bool initSDcard(chipSelect){
  if (debug) {
    Serial.begin(baudrate);
    while (!Serial) {
      ; }// wait for serial port to connect. Needed for native USB port only
  
    Serial.print("\n");
    Serial.println("Initialisation de la carte SD...");
  }
  // Vérification de la présence de la carte SD
  if (!SD.begin(chipSelectPin)) {
    if (debug) {
      Serial.println("Erreur lors de l'initialisation de la carte SD. Vérifiez que la carte est bien insérée.");
    }
    
    return (0); //Carte SD non reconnue
  }
  if (debug) {
    Serial.println("Carte SD initialisée avec succès.");
    Serial.println("------------------------------"); 
  }
  return(1);  //Carte SD initialisée
}

void setup() {
  // Initialisation de la communication SPI
  //SPI.begin();

  initSDcard(chipSelectPin);
  
}

// void readTTL(){

// }

void loop() {
  
   // Assemblage des donnees a logger:
  //String dataString = "";
  
  // si on recoit quelque chose de LEONARDO:
  
    if(Serial.available()>0){
      value=Serial.read();

      
      //dataString += String(value); //Ajout à la string de données a enregistrer

      // Ouverture du fichier sur la carte SD en mode écriture
      File dataFile = SD.open("data002.txt", FILE_WRITE);
      //ATTENTION : la taille du nom de fichier ne peut pas exceder 8 caractères
      
      // Vérification si le fichier a pu être ouvert
      if (dataFile) {
        // Écriture des données dans le fichier
        //dataFile.println(dataString);
        dataFile.print(value);

        // Fermeture du fichier
        dataFile.close();
        
      } 
      else {
        Serial.println("Erreur lors de l'ouverture du fichier.");
        while(1);
      }
  }
  
}
