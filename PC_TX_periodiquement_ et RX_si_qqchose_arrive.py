#----------------------------------------------------------------------
#  SIMPLE QWERY test for TSEN-RACHUTS
#----------------------------------------------------------------------

# utilisation de pyserial : https://pyserial.readthedocs.io/en/latest/index.html

# ATTENTION : pySerial demande un \n en fin de ligne pour sa routine readline()
# ser.read() ne lit qu'un caractere


from time import time, gmtime, strftime, localtime, sleep
import serial


#-----recupere de Py3-read-mouchard-movitoools-BIN---
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
#-----recupere de Py3-read-mouchard-movitoools-BIN---



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


#PATHs & PARAMETERS
addon = strftime("%d_%m_%Y_%Hh%Mm%Ss" , localtime())
my_file_path = "C:\\temp\\fichier_de_sauvegarde_eventuelle_"+addon+".dat" 
ficc = open(my_file_path, 'w')
#
#
measurement_duration_in_sec = 30
measurement_step_in_sec = 1

#inits
aa='jazzyjeffismybestfriend' ## have to start with something
elapsed_time = 0
time_of_start = time()
my_counter = 0



###----MAIN LOOP------QWERY WITH ser.write & ser.readline----------------------

while (elapsed_time <= measurement_duration_in_sec):
    
    # measure each measure_step_in_sec seconds 
    sleep(measurement_step_in_sec)    
    
    
    #Data write
    my_counter += 1
    ser.write('hello, I am writing to you every second :'.encode("ascii"))
    ser.write(str(my_counter).encode("ascii"))
    ser.write('\r'.encode("ascii"))
      
    # polling == Read data answer 
    aa=ser.readline()
    
    numbers = [ x for x in aa] 
 

    #***********************************************
    # show types
    #print (aa)
    # print (type(aa))
    #print (numbers)
    #print (aa.decode("ascii"))
    # print (type(numbers))
    # print (type(numbers[8]))
    #***********************************************
        
    
    #..and next...! 
    elapsed_time = time() - time_of_start
     
      
    #show snap time and data answer  ... 
    #snap = strftime("%a, %d %b %Y %H:%M:%S ", gmtime())
    snap = strftime("%d/%m/%Y %H:%M:%S" , localtime())
    microseconds = int (1000000 * (time() - int(time())))
    print (snap,' :',my_counter, ' :', aa.decode("ascii"))
    
    # add to datalogger               
    # save to file
    ficc.write(str(snap)+':'+str(aa.decode("ascii"))) 
     
    
###-------------------------------

ficc.close()
ser.close()             # close port
