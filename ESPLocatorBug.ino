#include "ESP8266WiFi.h"
extern "C" {
#include "user_interface.h"
//#include "user_config.h"
}

#define user_procTaskPrio        0
#define user_procTaskQueueLen    1
#define CHANNEL_HOP_INTERVAL   5000


int count = 0;

static os_timer_t channelHop_timer;

static void promisc_cb(uint8 *buf, uint16 len);

void channelHop(void *arg)
{
  if(count == 0){
      digitalWrite(BUILTIN_LED, HIGH);
      //Serial.println("led off");
  }else{
    count--;    
  }
}

static void ICACHE_FLASH_ATTR
promisc_cb(uint8 *buf, uint16 len)
{
   if(buf[12]==0x40){ //Por alguna razon ieee802.11 empieza apartir de el 12 .. 0x40 este es el probe request
    //Serial.println("detected probe");
    if(buf[22]==0xf8 && buf[23]==0xf8 && buf[24]==0xff && buf[25]==0xff && buf[26]==0xff && buf[27]==0xff ){ //mac a detectar 
      //Serial.println("led on");
      digitalWrite(BUILTIN_LED, LOW);
      count = 5;
      }
   }
 }

//Init function 
void setup ()
{
    delayMicroseconds(100);
    //Serial.begin(115200);
    pinMode(BUILTIN_LED, OUTPUT);     // Initializa BUILTIN_LED como output
    wifi_set_promiscuous_rx_cb(promisc_cb);
    wifi_promiscuous_enable(1);


    os_timer_disarm(&channelHop_timer);
    os_timer_setfn(&channelHop_timer, (os_timer_func_t *) channelHop, NULL);
    os_timer_arm(&channelHop_timer, CHANNEL_HOP_INTERVAL, 1);

    wifi_set_channel(6);

    wifi_set_opmode( 0x1 );

    //Start os task
    //system_os_task(loop, user_procTaskPrio,user_procTaskQueue, user_procTaskQueueLen);

 
}

void loop() {
  delay(100);
}
