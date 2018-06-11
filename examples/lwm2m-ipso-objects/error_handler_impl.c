/*
 * Copyright (c) 2018, RISE SICS.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *    3. Neither the name of the copyright holder nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

/**
 * \file
 *      Error handler example for LwM2M, CoAP and IPSO Objects
 * \author
 *      Carlos Gonzalo <carlosgp143@gmail.com>
 */

#include "contiki.h"
#include "sys/logx.h"
#include "dev/watchdog.h"
#include "services/lwm2m/lwm2m-engine.h"
#include "services/lwm2m/lwm2m-device.h"
#include "sys/ctimer.h"
#include <string.h>

#define MAX_FAILIURE_SPAN 100 /* sec */
#define MAX_REGISTRATION_FAILIURES 1
#define RECOVER_TIME 30 /* sec */
#define MAX_ALLOCATION_FAILIURES 10
#define MAX_HW_FAILIURES 10

/* RD client control */
static uint8_t registration_failiures = 0;
static clock_time_t previous_failiure = 0;
static clock_time_t now = 0;

/* CoAP control */
static uint8_t allocation_failiures = 0;

/* IPSO Objects control */
static uint8_t hw_failiures = 0;

static struct ctimer timer;

static void
restart_lwm2m(void *ptr)
{
  lwm2m_engine_init(true);
}
void
lwm2m_error_handler(logx_error_type_t type, const char *module, int level)
{

  if(strcmp(module, "lwm2m-rd") == 0) {
    switch(type) {
    case LOGX_LWM2M_SERVER_NOT_RESPONDING:
      now = clock_time();
      if((previous_failiure == 0) ||
         (now - previous_failiure) / CLOCK_SECOND <= MAX_FAILIURE_SPAN) {
        registration_failiures++;
      } else {
        /* If there is a new failiure in more than MAX_FAILIURE_SPAN
           it means that there has been a succesful registration */
        registration_failiures = 0;
      }

      if(registration_failiures == MAX_REGISTRATION_FAILIURES) {
        /* Take a decision */
        registration_failiures = 0;
        lwm2m_engine_stop();
        ctimer_set(&timer, RECOVER_TIME * CLOCK_SECOND, restart_lwm2m, NULL);
        previous_failiure = 0;
      } else {
        previous_failiure = now;
      }
      break;
    default:
      break;
    }
  } else if(strcmp(module, "coap") == 0) {
    switch(type) {
    case LOGX_COAP_TRANSACTION_ALLOCATION_FAIL:
      allocation_failiures++;
      if(allocation_failiures == MAX_ALLOCATION_FAILIURES) {
        /* Memory problem -> reboot */
        allocation_failiures = 0;
        watchdog_reboot();
      }
      break;
    case LOGX_COAP_OBSERVER_ALLOCATION_FAIL:
      allocation_failiures++;
      if(allocation_failiures == MAX_ALLOCATION_FAILIURES) {
        /* Memory problem -> reboot */
        allocation_failiures = 0;
        watchdog_reboot();
      }
      break;
    default:
      break;
    }
  } else if(strcmp(module, "ipso-obj") == 0) {
    switch(type) {
    case LOGX_IPSO_SENSOR_READ_FAIL:
      hw_failiures++;
      if(allocation_failiures == MAX_HW_FAILIURES) {
        /* Hardware problem -> reboot */
        hw_failiures = 0;
        watchdog_reboot();
      }
      break;
    case LOGX_IPSO_ACTUATOR_WRITE_FAIL:
      hw_failiures++;
      if(allocation_failiures == MAX_HW_FAILIURES) {
        /* Hardware problem -> reboot */
        hw_failiures = 0;
        watchdog_reboot();
      }
      break;
    default:
      break;
    }
  }
}