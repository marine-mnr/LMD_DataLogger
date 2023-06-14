# LMD_DataLogger

Arduino UNO
  - may22_receiver : Ecrit les data recues via le port série sur le Serial Monitor
  - may25_SDcard   : 
    -  v1 : Ecrit des lignes "test k" sur la carte SD
    -  v2 : Ecrit des lignes "test de communication K" recues de Leonardo
    -  v3 : Permet de continuer dès lors que la carte est inserée (changement du "if not begin" en while)
             
  - may31_SDcard_fonctions   :
    -  v1 : seulement init SD card en fonction
    -  v2 : initSDcar et save2SD en fonctions
    -  v3 : init modifié avec while et begin status (possibilité d'ajouter la SD en cours de fonctionnement
    -  v4 : correction de bugs
    -  v5 : (marche pas: essai d'enregistrement par ligne avec readStringUntil)
    -  v6 : YES! enregistrement de trame TSEN ! (ligne par ligne)
    -  v7 : v6 avec le buffer en version fonction
    -  v8 : permet d'interroger TSEN 
            
Arduino LEONARDO 
  - may22_transmitter : envoie via le port serie des lignes "test de communication k"

MKR 
- MKR_v1_test_ecriture : Ecrit des lignes "test k" sur la carte SD
- MKR_v2_receiver_copy : Lis des trames TSEN (autotrigger) et affiche sur Monitor
- MKR_v3_recep_autotrig_save : Lis et écrit des trames TSEN (autotrigger) sur la carte SD
- v4 : monitor et UART si differentes baudrates + while du while(!Serial)
- v5 : verifie la validite de la trame
- v6 : permet d'interroger TSEN (marche pas a tous les coup.. a cause du simulator ?)
- v7 : fichiers dupliques
- v8 : ajout timestamp + possibilité de réinserer SD sans rester bloqué dans "erreur ouverture fichier"
- v9 : utilise la librairie RTC_marine


RTC : 
- SimpleRTC_Marine : a partir de l'exemple, serRTC et get_timestamp dans meme programme
- Get_timestamp : get_timestamp uniquement, pour pas reset le temps a chaque fois
- RTC_test_my_library : permet de tester l'ajout de ma propre librairie (marche pas)
- ma librairie : 
  - RTC_marine.cpp
  - RTC_marine.h


Simulateur Python:
Utiliser le plus recent : Simule_TSEN_RACHUTS_RX_REALLY_NEED_TRIGGER_readquery_charbychar.py
