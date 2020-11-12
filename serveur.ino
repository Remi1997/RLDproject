#include <RHDatagram.h>
#include <RH_RF95.h>
#include <RH_ASK.h>
#include <Dictionary.h>

RH_RF95  rf95;

#define SERVER_ADDRESS 222
#define CLIENT_ADDRESS 111

RHDatagram  rhd(rf95,SERVER_ADDRESS);

Dictionary &dict = *(new Dictionary(6*16));

uint8_t CRC[] = {5, 6, 7, 8};
uint8_t array_SF[] = { 6, 7, 8, 9, 10, 11, 12, 13};

uint8_t SpreadingFactor=7;
int8_t TransmitPower=7;
boolean finished = false;

void setup() 
{
  Serial.begin(9600);  //Nous utilisons 9600bits/s pour la liaison série
  
  if (!rhd.init())
    Serial.println("Init failed.");
  rf95.setFrequency(868);    //Set the frequency in MHz.
  rf95.setTxPower(TransmitPower);      //Set Tx power in dBm*/
  rf95.setSpreadingFactor(SpreadingFactor);
   
}

void loop()
{
  
  if (rhd.available())
  {
    if(finished ==false){
      uint8_t address;
      uint8_t buffer_reception[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buffer_reception);
      if (rhd.recvfrom(buffer_reception, &len, &address))
      { 
        if(CLIENT_ADDRESS==address){
          uint8_t SF = buffer_reception[0];
          
          if( SF != 13){
            if (SF != SpreadingFactor)
             {  
                Serial.println("Results Intermediaires");
                for (int j=0; j < 4; j++) {
                  String key=CRC[j]+";"+SpreadingFactor;
                  String value = dict[key];
                  Serial.print(SpreadingFactor);
                  Serial.print(";");
                  Serial.print(CRC[j]);
                  Serial.print(";");
                  Serial.print(value);
                  Serial.println(";");
                }
                boolean flag_in=false;
                for(byte i = 0; i<8; i++)
                {
                  if (SF==array_SF[i])
                  {
                    flag_in =true;
                  }
                }

                if(flag_in==true){
                  rf95.setSpreadingFactor(SF);
                  SpreadingFactor=SF ;
                }
                
             }else
             {
              uint8_t CR = buffer_reception[1];
              Serial.println(CR);
              String key= CR+";"+SF;
              dict(key,String( (dict[key]).toInt()+1));
                 
             }
               s             
             }
              else
              {
                // Serial.println("Results");
                 for (int i=0; i < dict.count(); i++) {
                  String value =dict[i]+";"+dict(0)+";";
                  //Serial.println( value );
                  }
                  finished=true;
              }
           }
                    
      }
      else
      {
        Serial.println(" Not received "); //Remplir avec un message approprié
      }
    }
   
  }
  
}
