#include <RH_RF95.h>
#include <RHDatagram.h>

#define CLIENT_ADDRESS 111
#define SERVER_ADDRESS 222

RH_RF95  rf95;
RHDatagram rhdat(rf95, CLIENT_ADDRESS);

boolean FINISHED = false;
int CR[] = {5, 6, 7, 8};
int SF[] = { 6, 7, 8, 9, 10, 11, 12, 13};
int NBPACKET = 100;
int i = 6;


void setup() {
  Serial.begin(9600);
  
  if(!rhdat.init())
    Serial.println("init failed");

  rf95.setFrequency(868.0);    //Rentrez la bonne valeur en MHz. Cherchez sur la carte l'information
  //rf95.printRegisters();
  rf95.setTxPower(7);
  
}

void loop() {
  
  if(FINISHED==false){
    rf95.setSpreadingFactor(SF[i]);
      for(byte j=0; j < 3; j++){
        rf95.setCodingRate4(CR[j]);
        
        for (byte k=1; k<=NBPACKET; k++){
          //rf95.printRegisters();
          uint8_t data[3];
          Serial.println(SF[i]);
          Serial.println(CR[j]);
          data[0]=SF[i];
          data[1]=CR[j];
          rhdat.sendto( data , sizeof(data), SERVER_ADDRESS );
          rhdat.waitPacketSent();
          delay(100);
          Serial.println("Message sent!");
      }
    }
    i+=1;
    delay(1000);
    long t=millis();
    while (millis()< t + 10000) {
      uint8_t changement[2];
      changement[0]= SF[i];
      Serial.println(changement[0]);
      rhdat.sendto( changement , sizeof(changement), SERVER_ADDRESS );
      rhdat.waitPacketSent();
    }
  }
    if (SF[i]==13) {
    FINISHED = true;
    uint8_t fin[2];
    fin[0]= 13;
    rhdat.sendto( fin , sizeof(fin), SERVER_ADDRESS );
    rhdat.waitPacketSent();
    Serial.println("finished");
  }
    }

    
