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
char file="data003.txt";

char value=0;
int k=0;

/* Fonction initSDcard
  // Paramètres :
  // - chipSelect : le numéro de broche utilisé pour la communication avec la carte SD
  // Retour :
  // - booléen : true si la carte SD est initialisée avec succès, false sinon
*/
bool initSDcard(chipSelectPin){
  if (debug) {
    Serial.begin(baudrate);
    while (!Serial) {
      ; }// wait for serial port to connect. Needed for native USB port only
  
    Serial.print("\n");
    Serial.println("Initialisation de la carte SD...");
  }
  // Vérification de la présence de la carte SD
  
  bool begin_status=0;
    
  while(!SD.begin(chipSelectPin)) {
    if (debug) {
      Serial.println("Erreur lors de l'initialisation de la carte SD. Vérifiez que la carte est bien insérée.");
    }
    begin_status=0; //Carte SD non reconnue
  }
  begin_status=1; //Carte SD trouvée
  if (debug) {
    Serial.println("Carte SD initialisée avec succès.");
    Serial.println("------------------------------"); 
  }
  return(begin_status);  

}

/* Fonction save2SD : ouvre un fichier dans la carte SD, ecrit une valeur dedans puis le referme.
  // Paramètre :
  // - filepath : le chemin d'accès et le nom du fichier à sauvegarder
  // - value : ce qu'il faut ecrire dans le fichier (ici un char)
*/
void save2SD(filepath,value){
  // Ouverture du fichier sur la carte SD en mode écriture
    File dataFile = SD.open(filepath, FILE_WRITE);
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
      if (debug) {
        Serial.println("Erreur lors de l'ouverture du fichier.");
      }
      while(1); //a modifier ?
    }
        

}


void setup() {
  // Initialisation de la communication SPI
  //SPI.begin();                                  //supprimer?

  initSDcard(chipSelectPin);
}



void loop() {
  
  // Assemblage des donnees a logger:
  //String dataString = "";                        //supprimer?
  
  // si on recoit quelque chose de LEONARDO:
    if(Serial.available()>0){
    
    received_value=Serial.read();

    //dataString += String(value); //Ajout à la string de données a enregistrer

    save2SD(file,received_value);
      
  }
  
}
