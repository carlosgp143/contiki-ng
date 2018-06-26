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
 * \addtogroup lwm2m
 * @{
 */

/**
 * \file
 *         API to manage the resource attributes for notifications
 * \author
 *         Carlos Gonzalo Peces <carlosgp143@gmail.com>
 */
/*---------------------------------------------------------------------------*/
#include "lib/list.h"
#include "lib/memb.h"
#include "lwm2m-notification-attributes.h"

/* Log configuration */
#include "coap-log.h"
#define LOG_MODULE "lwm2m-attr"
#define LOG_LEVEL  LOG_LEVEL_LWM2M

#define LWM2M_MAX_NOTIFICATION_ATTRIBUTES 100

/*---------------------------------------------------------------------------*/
//MEMB(attribute_memb, lwm2m_notification_attribute_t, LWM2M_MAX_NOTIFICATION_ATTRIBUTES);
/*---------------------------------------------------------------------------*/
bool
lwm2m_notification_attributes_remove(lwm2m_object_instance_t *object, uint16_t resource, lwm2m_attribute_type_t type)
{
  if(object->notification_attrs_memb == NULL) {
    LOG_DBG("Memb block not provided, not posible to add attributes to this instance\n");
    return false;
  }
	lwm2m_notification_attribute_t *attr = (lwm2m_notification_attribute_t *)list_head(object->notification_attrs);
  while(attr != NULL) {

  	if((attr->resource_id) == resource && (attr->type == type)){
      LOG_INFO("Attribute %"PRIu8" removed for resource: %"PRIu16"/%"PRIu16"/%"PRIu16"\n", type, object->object_id, object->instance_id, resource);
  		list_remove(object->notification_attrs, attr);
  		memb_free(object->notification_attrs_memb, attr);
  		return true;
  	}
  	attr = attr->next;
  }
  return false;
}
/*---------------------------------------------------------------------------*/
bool
lwm2m_notification_attributes_add(lwm2m_object_instance_t *object, uint16_t resource,
								  lwm2m_attribute_type_t type, uint16_t value)
{

  if(object->notification_attrs_memb == NULL) {
    LOG_DBG("Memb block not provided, not posible to add attributes to this instance\n");
    return false;
  }
	/* First remove if the resource already has an attribute */
	lwm2m_notification_attributes_remove(object, resource, type);

	lwm2m_notification_attribute_t *attr = memb_alloc(object->notification_attrs_memb);
	if(attr) {

		attr->resource_id = resource;
		attr->type = type;
		attr->value = value;
		list_add(object->notification_attrs, attr);
		LOG_INFO("Attribute %"PRIu8" added for resource: %"PRIu16"/%"PRIu16"/%"PRIu16" with value: %"PRIu16"\n", type, object->object_id, object->instance_id, resource, value);
		return true;
	}
	LOG_DBG("Could not allocate new attribute\n");
	return false;
}
/*---------------------------------------------------------------------------*/
bool
lwm2m_notification_attributes_get(uint16_t *value, lwm2m_object_instance_t *object, uint16_t resource, lwm2m_attribute_type_t type)
{
  if(object->notification_attrs_memb == NULL) {
    LOG_DBG("Memb block not provided, not posible to add attributes to this instance\n");
    return false;
  }
	lwm2m_notification_attribute_t *attr = (lwm2m_notification_attribute_t *)list_head(object->notification_attrs);
  while(attr != NULL) {

  	if((attr->resource_id) == resource && (attr->type == type)){
  		*value = attr->value;
  		LOG_INFO("Returning value: %"PRIu16" for attribute %"PRIu8" from resource: %"PRIu16"/%"PRIu16"/%"PRIu16"\n", *value, type, object->object_id, object->instance_id, resource);
  		return true;
  	}
  	attr = attr->next;
  }
  return false;
}
/*---------------------------------------------------------------------------*/
uint64_t
lwm2m_notification_attributes_get_extra_data(lwm2m_object_instance_t *object, uint16_t resource, lwm2m_attribute_type_t type)
{
  if(object->notification_attrs_memb == NULL) {
    LOG_DBG("Memb block not provided, not posible to add attributes to this instance\n");
    return false;
  }
  lwm2m_notification_attribute_t *attr = (lwm2m_notification_attribute_t *)list_head(object->notification_attrs);
  while(attr != NULL) {

    if((attr->resource_id) == resource && (attr->type == type)){
      LOG_INFO("Returning extra data: %"PRIu64" for attribute %"PRIu8" from resource: %"PRIu16"/%"PRIu16"/%"PRIu16"\n", attr->extra_data, type, object->object_id, object->instance_id, resource);
      return attr->extra_data;
    }
    attr = attr->next;
  }
  return -1;
}
/*---------------------------------------------------------------------------*/
void 
lwm2m_notification_attributes_print(lwm2m_object_instance_t *object)
{
  if(object->notification_attrs_memb == NULL) {
    return;
  }
  printf("----Notification attr-------\n");
  lwm2m_notification_attribute_t *attr = (lwm2m_notification_attribute_t *)list_head(object->notification_attrs);
  while(attr != NULL) {
    printf("RSC_ID:%"PRIu16", type: %"PRIu8", value:%"PRIu16"\n", attr->resource_id, attr->type, attr->value);
    attr = attr->next;
  }
}
/*---------------------------------------------------------------------------*/
/** @} */