/*
 * Copyright (c) 2018, RISE SICS AB.
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
 * \addtogroup oma-lwm2m
 * @{
 */

/**
 * \file
 *        Header file for the API to manage the resource attributes for notifications
 * \author
 *         Carlos Gonzalo Peces <carlosgp143@gmail.com>
 */

#ifndef LWM2M_NOTIFICATION_ATTRIBUTES_H_
#define LWM2M_NOTIFICATION_ATTRIBUTES_H_

#include "lwm2m-object.h"
#include "lwm2m-engine.h"
#include <stdbool.h>

typedef struct lwm2m_notification_periodic_attr lwm2m_notification_periodic_attr_t;

struct lwm2m_notification_periodic_attr {
  lwm2m_notification_periodic_attr_t *next;
  lwm2m_object_instance_t *obj;
  uint16_t rsc_id;
  lwm2m_attribute_type_t type;
  uint16_t value;
  uint64_t next_expiration_time;
};

bool lwm2m_notification_attributes_add_pmax(lwm2m_object_instance_t *object, uint16_t resource, uint16_t value);
void lwm2m_notification_attributes_update_pmax(lwm2m_object_instance_t *object, uint16_t resource);


bool lwm2m_notification_attributes_add(lwm2m_object_instance_t *object, uint16_t resource, lwm2m_attribute_type_t type, uint16_t value);

bool lwm2m_notification_attributes_get(uint16_t *value, lwm2m_object_instance_t *object, uint16_t resource, lwm2m_attribute_type_t type);

bool lwm2m_notification_attributes_remove(lwm2m_object_instance_t *object, uint16_t resource, lwm2m_attribute_type_t type);

void lwm2m_notification_attributes_print(lwm2m_object_instance_t *object);
void  lwm2m_notification_pmax_print();

void lwm2m_notification_attributes_init(void);

#endif /* LWM2M_NOTIFICATION_ATTRIBUTES_H_ */
/** @} */