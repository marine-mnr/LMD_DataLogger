#include <SD.h>
#include <RTC_marine.h>

#define TSEN_START 0x23 //#
#define TSEN_STOP 0x0D //=carriage return
#define TSEN_LENGTH 19 

#define TRAME_INSTR_LENGTH TSEN_LENGTH

const int UART_baudrate = 9600;
const int monitor_baudrate = 19200;

const bool debug=1; //mode debuggage avec affichages: 1 si activé, 0 sinon


const int chipSelectPin = 4; // Broche CS du shield
  /* ATTENTION : il faut bien choisir le pin. 
  // Arduino Ethernet shield, SEEDSTUDIO SD CARD SHIELD, MKR MEM SHIELD : pin 4
  // Adafruit SD shields and modules: pin 10
  // Sparkfun SD shield: pin 8
  // MKRZero SD: SDCARD_SS_PIN
  */

const char* file="mkrdat36.txt";                      ///////*********************A REMPLIR AVANT LANCEMENT*****************//////////
const char* file_detail="detail36.txt";
const char* phrase_interrogation="*01A?\r";


//inits
bool initSDcard_status = 0;
bool initMonitor_status = 0;
bool initUART_status = 0;
bool initSPI_status = 0;
//uart
char received_value=0;
char r=0;
//variables
unsigned int buffer_index=0;
char buffer[2*TSEN_LENGTH]={};
bool trame_valide=0;
unsigned long lastInterrogationTime = 0;

// Create an rtc object 
RTC_marine rtc;


/* Fonction initMonitor********************************************************************************************
  // Paramètres :
  // - baudrate : vitesse de transmission (en bit/s), differente de l'UART
  // Retour :
  // - booléen : true si le monitor est initialisé avec succès, false sinon */
bool initMonitor(int monitor_baudrate){
  Serial.begin(monitor_baudrate);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  bool begin_status=1;
  return(begin_status); 
}

/* Fonction initUART**************************************************************************************************
  // Paramètres :
  // - baudrate : vitesse de transmission (en bit/s), differente du monitor
  // Retour :
  // - booléen : true si l'UART est initialisé avec succès, false sinon */
bool initUART(int UART_baudrate){
  Serial1.begin(UART_baudrate);
  bool begin_status=1;
  return(begin_status); 
}

/* Fonction initSDcard**************************************************************************************************
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


/* Fonction save2SD : ouvre un fichier dans la carte SD, ecrit une valeur dedans puis le referme.*************************
  // Paramètre :
  // - filepath : le chemin d'accès et le nom du fichier à sauvegarder
  // - value : ce qu'il faut ecrire dans le fichier (ici un char)
  // NB: utilise la globale TRAME_INSTR_LENGTH (longueur de la trame prévue)
  */
void save2SD(const char* filepath, char* message, char my_size){
  // Ouverture du fichier sur la carte SD en mode écriture
    File dataFile = SD.open(filepath, FILE_WRITE);
    //ATTENTION : la taille du nom de fichier ne peut pas exceder 8 caractères

  // Vérification si le fichier a pu être ouvert
    if (dataFile) {
      // Écriture des données dans le fichier
      for (int i=0; i<my_size; i++){
      dataFile.print(message[i]);
      }

      // Fermeture du fichier
        dataFile.close();
    }
    else {
      if (debug) {
        Serial.println("Erreur lors de l'ouverture du fichier.");
        SD.begin(chipSelectPin);
      }
      
    }
}


/* Fonction read_and_save_trame_TSEN********************************************************************************************
  // La fonction lis chaque caractere qui arrive. 
  // Si ce n'est pas le terminateur, elle l'ajoute dans un tableau.
  // Si c'est le terminateur (TSEN_STOP), elle ajoute un \n et enregistre la ligne dans un fichier avec la fonction save2SD */
void read_and_save_trame_TSEN(){
  //init
  trame_valide=0;

  //lecture
  r=Serial1.read();

  //tant que le caractere de fin n'est pas atteint
  if(r!=TSEN_STOP){
    buffer[buffer_index]=r;
    buffer_index++;
  }
  else{ //STOP trouvé, la trame est complète
    buffer[buffer_index]=r;
    // test validite de la trame TSEN
    if((buffer[0]==TSEN_START) && (buffer[TSEN_LENGTH-1]==TSEN_STOP)){
      trame_valide=1; 
    }
    
    //reinit 
    buffer_index=0; 
    
    // enregistrement(s) dans la carte SD 
    // (a)raw + \n
    save2SD(file,buffer,TRAME_INSTR_LENGTH);
    save2SD(file,"\n",1);

    // (b)timestamp + validity + raw + \n
    save2SD(file_detail,rtc.get_timestamp(),17);
    if(trame_valide){save2SD(file_detail,";1",2);}
      else{save2SD(file_detail,";0",2);}
    save2SD(file_detail,";",1);  
    save2SD(file_detail,buffer,TRAME_INSTR_LENGTH);
    save2SD(file_detail,"\n",1);

    //affichage
    if(debug){
      
      Serial.print("\n");
      Serial.print(rtc.get_timestamp());
      if(trame_valide){Serial.print(" ; 1 ; ");}
      else{Serial.print(" ; 0 ; ");}
      for (int i=0; i<TSEN_LENGTH; i++){
        Serial.print(buffer[i]);
      }
    }
  }
}

/* Fonction interroge_TSEN********************************************************************************************
  // La fonction compte le temps écoulé depuis la derniere interrogation.
  // Lorqu'il atteint 1s, elle envoie à l'instrument la phrase d'interrogation pour qu'il envoie ses données 
  // puis met à jour le moment de la dernière interrogation */
void interroge_TSEN(int interval_ms){
  // Calculer le temps écoulé depuis la dernière interrogation
    unsigned long elapsedTime = millis() - lastInterrogationTime;

  // Vérifier si une seconde s'est écoulée depuis la dernière interrogation
  if (elapsedTime >= interval_ms) {
    // Interroger TSEN avec *01A?\r
    Serial1.print(phrase_interrogation);
  
    // Mettre à jour le moment de la dernière interrogation
    lastInterrogationTime = millis();
  }
}
//////////////////////////////////////////////// SET UP ////////////////////////////////////////////////////////////////
void setup() {
  //Initialisation de la communication Serial (monitor)
  bool initMonitor_status = initMonitor(monitor_baudrate);
 
  //Initialisation de la communication UART
  bool initUART_status = initUART(UART_baudrate);
  
  // Initialisation de la SD Card
  bool initSDcard_status = initSDcard();

  //Initialisation de la date et l'heure
  rtc.setDateTime(13,06,23,12,16,00);                               ///////*********************A REMPLIR AVANT LANCEMENT*****************//////////
}

//////////////////////////////////////////////// LOOP ////////////////////////////////////////////////////////////////////
void loop() {
  ////interroger TSEN avec *01A?\r toutes les secondes                           
  interroge_TSEN(1000);

  // si l'UART recoit quelque chose:
  if(Serial1.available()>0){
    //on traite la trame
    read_and_save_trame_TSEN(); 
  }
}
