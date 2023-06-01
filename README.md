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
            
Arduino LEONARDO 
  - may22_transmitter : envoie via le port serie des lignes "test de communication k"
