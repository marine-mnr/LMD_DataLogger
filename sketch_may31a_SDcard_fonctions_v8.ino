//#include <SPI.h>
#include <SD.h>

#define TSEN_START #
#define TSEN_STOP 0x0D //=carriage return


const int baudrate = 9600;

//#define debug 0   //mode debuggage avec affichages: 1 si activé, 0 sinon
const bool debug=1; //mode debuggage avec affichages: 1 si activé, 0 sinon


const int chipSelectPin = 4; // Broche CS du shield
  /* ATTENTION : il faut bien choisir le pin. 
  // Arduino Ethernet shield, SEEDSTUDIO SD CARD SHIELD : pin 4
  // Adafruit SD shields and modules: pin 10
  // Sparkfun SD shield: pin 8
  // MKRZero SD: SDCARD_SS_PIN
  */

const char* file="data012.txt"; 
const char* phrase_interrogation="*01A?\r";


//inits
bool initSDcard_status = 0;
bool initUART_status = 0;
bool initSPI_status = 0;
//uart
char received_value=0;
char r=0;

unsigned char buffer_index=0;
char buffer[100]={};
unsigned long lastInterrogationTime = 0;


/* Fonction initSDcard********************************************************************************************
  // Paramètres :
  // - chipSelect : le numéro de broche utilisé pour la communication avec la carte SD
  // Retour :
  // - booléen : true si la carte SD est initialisée avec succès, false sinon */
bool initSDcard(){
  if (debug) {
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

/* Fonction initUART********************************************************************************************
  // Paramètres :
  // - baudrate : vitesse de transmission (en bit/s)
  // Retour :
  // - booléen : true si l'UART est initialisé avec succès, false sinon */
bool initUART(int baudrate){
  Serial.begin(baudrate);
    while (!Serial) {
    bool begin_status=0; }// wait for serial port to connect. Needed for  native USB port only
  bool begin_status=1;
  return(begin_status); 
}

/* Fonction save2SD : ouvre un fichier dans la carte SD, ecrit une valeur dedans puis le referme.*************************
  // Paramètre :
  // - filepath : le chemin d'accès et le nom du fichier à sauvegarder
  // - value : ce qu'il faut ecrire dans le fichier (ici un char)*/
void save2SD(const char* filepath, char* message){
  // Ouverture du fichier sur la carte SD en mode écriture
    File dataFile = SD.open(filepath, FILE_WRITE);
    //ATTENTION : la taille du nom de fichier ne peut pas exceder 8 caractères
  // Vérification si le fichier a pu être ouvert
    if (dataFile) {
      // Écriture des données dans le fichier
      
        //dataFile.println(dataString);
        dataFile.print(message);

      // Fermeture du fichier
        dataFile.close();
    }
    else {
      if (debug) {
        Serial.println("Erreur lors de l'ouverture du fichier.");
      }
      
    }
}


/* Fonction read_and_save_trame_TSEN********************************************************************************************
  // La fonction lis chaque caractere qui arrive. 
  // Si ce n'est pas le terminateur, elle l'ajoute dans un tableau.
  // Si c'est le terminateur (TSEN_STOP), elle ajoute un \n et enregistre la ligne dans un fichier avec la fonction save2SD */
void read_and_save_trame_TSEN(){
  r=Serial.read();
  if(r!=TSEN_STOP){
    buffer[buffer_index]=r;
    buffer_index++;
  }
  else{
    buffer[buffer_index]='\n';
    buffer_index=0;
    if(debug){Serial.print(buffer);}
    save2SD(file,buffer);
  }
}


void setup() {
  //Initialisation de la communication UART
  bool initUART_status = initUART(baudrate);
  
  // Initialisation de la communication SPI
  //SPI.begin();                                  //supprimer?

  // Initialisation de la SD Card
  bool initSDcard_status = initSDcard();
}

void interroge_TSEN(int interval_ms){
  // Calculer le temps écoulé depuis la dernière interrogation
    unsigned long elapsedTime = millis() - lastInterrogationTime;

    // Vérifier si une seconde s'est écoulée depuis la dernière interrogation
    if (elapsedTime >= interval_ms) {
      // Interroger TSEN avec *01A?\r
      Serial.print(phrase_interrogation);
    
      // Mettre à jour le moment de la dernière interrogation
      lastInterrogationTime = millis();
    }
}



void loop() {

  //interroger TSEN avec *01A?\r toutes les secondes
  interroge_TSEN(1000);
    


  // si l'UART de UNO recoit quelque chose:
  if(Serial.available()>0){
    //on traite la trame
    read_and_save_trame_TSEN(); 
  }
}
