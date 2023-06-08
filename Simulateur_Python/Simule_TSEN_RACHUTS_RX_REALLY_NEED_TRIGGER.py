#----------------------------------------------------------------------
#  SIMPLE SIMULATOR for TSEN-RACHUTS
#
#  Envoie des trames typiques de TSEN-RACHUTS (sans besoin de trig)
#
#  Envoi périodique d'une trame comme :  #123 123456 246810\r
#
#----------------------------------------------------------------------


# utilisation de pyserial : https://pyserial.readthedocs.io/en/latest/index.html

# ATTENTION : pySerial demande un \n en fin de ligne pour sa routine readline()
# ser.read() ne lit qu'un caractere


from time import time, gmtime, strftime, localtime, sleep
import serial
from random import randint


def sixDrandom():
  n = 6
  nuum = ''.join(["{}".format(randint(0, 9)) for num in range(0, n)])
  return(nuum)
  
def threeDrandom():
  n = 3
  nuum = ''.join(["{}".format(randint(0, 9)) for num in range(0, n)])
  return(nuum)  
  
def trameTSENrandom():
  # typical : #123 123456 246810\r
  trame = '#'+threeDrandom()+' '+sixDrandom()+' '+sixDrandom()+'\r'   
  return(trame)


#-----conversions non utlisees ---
def my_bytes_to_2charHex(byte_data):
    if byte_data>9:
        result = hex(byte_data)[2:]
    else:
        result = '0'+hex(byte_data)[2:]
    return result

def my_bytes_to_int(byte_data):
    result = 0
    for b in byte_data:
        result = result * 256 + int(b)
    return result
#-----conversions non utlisees ---


# TRY TO OPEN COM PORT
try:
  ser = serial.Serial("COM1")
  ser.baudrate = 9600
  ser.port = "COM1"
  ser.bytesize=8
  #ser.parity=serial.PARITY_EVEN
  #ser.stopbits=2
  ser.parity=serial.PARITY_NONE
  ser.stopbits=1
  ser.timeout=0.01
  portname = ser.name
  print ("The port '%s' is available :\n'%s'" % (portname , ser))

except serial.serialutil.SerialException:
  print ("The port is at use, u have to reset it ")
  exit()


#PARAMETERS
measurement_duration_in_sec = 180
measurement_step_in_sec = 1

#inits
aa='jazzyjeff' ## have to start with something
elapsed_time = 0
time_of_start = time()
trame_sten_simulee = '#123 123456 246810\r'



###----MAIN LOOP------envoi des trames random----------------------

while (elapsed_time <= measurement_duration_in_sec):
    
    # measure each measure_step_in_sec seconds 
    #sleep(measurement_step_in_sec)    
       
      
    # Read if something comes from serial
    aa=ser.readline()
    
    
    #..and next...! 
    elapsed_time = time() - time_of_start
    
    
    #this is the way to stop the polling in case of simulation      
    if aa.startswith(b'quit') or aa.startswith(b'q') or aa.startswith(b'Init'):
        print('OK')
        print(aa)
        ser.close()             # close port
        break
    elif aa==b'*01A?\r':
        # tsen respond to the query he was waiting for
        # write une trame typique TSEN)
        trame_sten_simulee = trameTSENrandom()
        ser.write(trame_sten_simulee.encode("ascii"))
        print("sent to COM1 :  "+trame_sten_simulee)     
    elif aa==b'':
        pass
    else:
        print(aa)
     

     
    
###-------------------------------

ser.close()             # close port
