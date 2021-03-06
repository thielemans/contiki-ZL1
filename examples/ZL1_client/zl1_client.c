/*
 * Copyright (c) 2007, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */
/*---------------------------------------------------------------------------*/
/**
 * \file
 *         An example of how to use the ZL1 shield
 * \author
 *         Quentin Quevy <quentin.alain.quevy@vub.be>
 */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "contiki.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "dev/z1-phidgets.h"
#include "sys/etimer.h"
#include "lib/RN2483.h"
#include "dev/lora-send.h"
#define RELAY_INTERVAL (CLOCK_SECOND)
/*---------------------------------------------------------------------------*/
PROCESS(send_sensor_process, "process sending photocell value");
AUTOSTART_PROCESSES(&send_sensor_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(send_sensor_process, ev, data)
{
  static struct etimer et;
  PROCESS_BEGIN();
  etimer_set(&et, 2*CLOCK_SECOND);
  lora_initialize();
  SENSORS_ACTIVATE(phidgets);
  SENSORS_ACTIVATE(button_sensor);
  set_radio_settings("pwr","14","pwr","14",(char *)NULL);
  static int i=0;
  while(1) {
	  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	  leds_toggle(LEDS_GREEN);
	  if(i==1){
 	  lora_send("mac pause\r\n");}
	  else if(i==2){
   	  char illuminance[5];
	  float lux=phidgets.value(PHIDGET3V_2)/4.095;// voltage=value*3.3/4095 and lux=voltage*1000/3.3 (approximation)
	  itoa((int)lux, illuminance, 10);
	  printf("illuminance:%s\n", illuminance);
	  //ptp_send(illuminance);
	  lora_send("radio tx 5000\r\n");
	  }
	  else if(i==6){
   	  i=0;}
	  i++;
	  etimer_reset(&et);
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
