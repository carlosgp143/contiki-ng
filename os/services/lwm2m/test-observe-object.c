/*
 * Copyright (c) 2017, RISE SICS AB.
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
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \addtogroup lwm2m
 * @{
 */

/**
 * \file
 *         Implementation of the Contiki OMA LWM2M Queue Mode object
 		   to manage the parameters from the server side
 * \author
 *         Carlos Gonzalo Peces <carlosgp143@gmail.com>
 */

#include "lwm2m-object.h"

/* Log configuration */
#include "coap-log.h"
#define LOG_MODULE "lwm2m-test-observe-object"
#define LOG_LEVEL  LOG_LEVEL_LWM2M

#define RESOURCE_CNT 20

#define TEST_OBSERVE_OBJECT_ID 40000
#define RESOURCE_1_ID 40000
#define RESOURCE_2_ID 41000
#define RESOURCE_3_ID 42000
#define RESOURCE_4_ID 43000
#define RESOURCE_5_ID 44000
#define RESOURCE_6_ID 45000
#define RESOURCE_7_ID 46000
#define RESOURCE_8_ID 47000
#define RESOURCE_9_ID 48000
#define RESOURCE_10_ID 49000
#define RESOURCE_11_ID 50000
#define RESOURCE_12_ID 51000
#define RESOURCE_13_ID 52000
#define RESOURCE_14_ID 53000
#define RESOURCE_15_ID 54000
#define RESOURCE_16_ID 55000
#define RESOURCE_17_ID 56000
#define RESOURCE_18_ID 57000
#define RESOURCE_19_ID 58000
#define RESOURCE_20_ID 59000


static uint32_t rscs[RESOURCE_CNT] = {RESOURCE_1_ID,RESOURCE_2_ID,RESOURCE_3_ID, RESOURCE_4_ID,RESOURCE_5_ID,RESOURCE_6_ID,RESOURCE_7_ID,RESOURCE_8_ID,RESOURCE_9_ID,RESOURCE_10_ID,
                            RESOURCE_11_ID,RESOURCE_12_ID,RESOURCE_13_ID, RESOURCE_14_ID,RESOURCE_15_ID,RESOURCE_16_ID,RESOURCE_17_ID,RESOURCE_18_ID,RESOURCE_19_ID,RESOURCE_20_ID};


static const lwm2m_resource_id_t resources[] =
{ RO(RESOURCE_1_ID),
  RO(RESOURCE_2_ID),
  RO(RESOURCE_3_ID),
  RO(RESOURCE_4_ID),
  RO(RESOURCE_5_ID),
  RO(RESOURCE_6_ID),
  RO(RESOURCE_7_ID),
  RO(RESOURCE_8_ID),
  RO(RESOURCE_9_ID),
  RO(RESOURCE_10_ID),
  RO(RESOURCE_11_ID),
  RO(RESOURCE_12_ID),
  RO(RESOURCE_13_ID),
  RO(RESOURCE_14_ID),
  RO(RESOURCE_15_ID),
  RO(RESOURCE_16_ID),
  RO(RESOURCE_17_ID),
  RO(RESOURCE_18_ID),
  RO(RESOURCE_19_ID),
  RO(RESOURCE_20_ID)
};

/*---------------------------------------------------------------------------*/
static lwm2m_status_t
lwm2m_callback(lwm2m_object_instance_t *object, lwm2m_context_t *ctx)
{
  if(ctx->operation == LWM2M_OP_READ) {
    lwm2m_object_write_int(ctx, (int32_t)rscs[(ctx->resource_id-40000)/1000]);
    return LWM2M_STATUS_OK;
  } 
  return LWM2M_STATUS_OPERATION_NOT_ALLOWED;
}
/*---------------------------------------------------------------------------*/
static lwm2m_object_instance_t test_observe_object = {
  .object_id = TEST_OBSERVE_OBJECT_ID,
  .instance_id = 0,
  .resource_ids = resources,
  .resource_count = sizeof(resources) / sizeof(lwm2m_resource_id_t),
  .resource_dim_callback = NULL,
  .callback = lwm2m_callback,
};
/*---------------------------------------------------------------------------*/
static void
increment_counts(void)
{
  int i;
  for(i=0; i<RESOURCE_CNT; i++){
    rscs[i]++;
  }
}
/*---------------------------------------------------------------------------*/
void
lwm2m_test_observe_object_notify_all(void)
{
  increment_counts();
  int i;
  int rsc = 40000;
  char url [20];
  for(i = 0; i<RESOURCE_CNT; i++) {
    snprintf(url,sizeof(url) , "40000/0/%d", rsc);
    lwm2m_notify_observers(url);
    rsc+=1000;
  }
}
/*---------------------------------------------------------------------------*/
void
lwm2m_test_observe_object_init(void)
{
  lwm2m_engine_add_object(&test_observe_object);
}
/** @} */