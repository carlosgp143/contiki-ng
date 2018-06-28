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
 * \addtogroup coap
 * @{
 */

/**
 * \file
 *         Manage the detection of duplicated messages in CoAP
 * \author
 *         Carlos Gonzalo Peces <carlosgp143@gmail.com>
 */
/*---------------------------------------------------------------------------*/
#include "coap-duplicate-detection.h"
#include "coap-timer.h"
#include "lib/list.h"
#include "lib/memb.h"


/* Log configuration */
#include "coap-logx.h"
#define LOG_MODULE "coap-dup"
#define LOG_LEVEL  LOG_LEVEL_DBG

#define COAP_DUPLICATE_DETECTION_MAX_ENDPOINTS 3
#define COAP_DUPLICATE_DETECTION_MAX_INFO 10

#define COAP_DUPLICATE_DETECTION_LIFETIME 20000 /* 145 sec (taken from draft-ietf-lwig-coap-05 section 2.3.1 */
#define UPDATE_INTERVAL 1000 /* ms */

/*---------------------------------------------------------------------------*/
MEMB(endpoint_memb, coap_duplicate_detection_endpoint_t, COAP_DUPLICATE_DETECTION_MAX_ENDPOINTS);
LIST(endpoint_list);
MEMB(info_memb, coap_duplicate_detection_info_t, COAP_DUPLICATE_DETECTION_MAX_INFO);
LIST(info_list);
/*---------------------------------------------------------------------------*/
static void
remove_info(coap_duplicate_detection_info_t *info)
{
  list_remove(info_list, info);
  memb_free(&info_memb, info);
  LOG_DBG("Removing packet with MID: %d, for endpoint: ", info->mid);
  coap_endpoint_log(info->endpoint);
  LOG_DBG_("\n");
}
/*---------------------------------------------------------------------------*/
static void
remove_endpoint(coap_duplicate_detection_endpoint_t *endp)
{
  /* First remove all message info associated with the endpoint */
  coap_duplicate_detection_info_t *info = (coap_duplicate_detection_info_t*) list_head(info_list);
  coap_duplicate_detection_info_t *info_aux = info;
  while(info != NULL) {
    info_aux = info->next;
    if(coap_endpoint_cmp(&endp->endpoint, info->endpoint)) {
      remove_info(info);
    }
    info = info_aux;
  }
  LOG_DBG("Removing endpoint: ");
  coap_endpoint_log(&endp->endpoint);
  LOG_DBG("\n");

  list_remove(endpoint_list, endp);
  memb_free(&endpoint_memb, endp);
}
/*---------------------------------------------------------------------------*/
static void
remove_first_endpoint(void)
{
  coap_duplicate_detection_endpoint_t *endp = (coap_duplicate_detection_endpoint_t*) list_head(endpoint_list);
  coap_duplicate_detection_endpoint_t *endp_aux = endp->next;
  remove_endpoint(endp);
  *endpoint_list = (struct list *) endp_aux;
}
/*---------------------------------------------------------------------------*/
static void
remove_first_info(void)
{
  coap_duplicate_detection_info_t *info = (coap_duplicate_detection_info_t*) list_head(info_list);
  coap_duplicate_detection_info_t *info_aux = info->next;
  remove_info(info);
  *info_list = (struct list *) info_aux;
}
/*---------------------------------------------------------------------------*/
static coap_duplicate_detection_info_t*
remove_obsolete_info(void)
{
  uint64_t now = coap_timer_uptime();
  coap_duplicate_detection_info_t *info = (coap_duplicate_detection_info_t*) list_head(info_list);
  coap_duplicate_detection_info_t *info_aux = info;
  while(info != NULL) {
    info_aux = info->next;
    if(now - info->time > COAP_DUPLICATE_DETECTION_LIFETIME) {
      LOG_DBG("Removing packet with expired lifetime\n");
      remove_info(info);
    }
    info = info_aux;
  }
  return memb_alloc(&info_memb);
}
/*---------------------------------------------------------------------------*/
static bool
endpoint_has_message(coap_endpoint_t *src)
{
  coap_duplicate_detection_info_t *info = (coap_duplicate_detection_info_t*) list_head(info_list);
  while(info != NULL) {
    if(coap_endpoint_cmp(info->endpoint, src)) {
      
      return true;
    } 
    info = info->next;
  }
  return false;
}
/*---------------------------------------------------------------------------*/
static coap_duplicate_detection_endpoint_t*
endpoint_with_no_messages(void)
{
  coap_duplicate_detection_endpoint_t *endp = (coap_duplicate_detection_endpoint_t*) list_head(endpoint_list);
  while(endp != NULL) {
    if(!endpoint_has_message(&endp->endpoint)) {
      return endp;
    } 
    endp = endp->next;
  }
  return NULL;
}
/*---------------------------------------------------------------------------*/
static coap_duplicate_detection_endpoint_t* 
get_endpoint(const coap_endpoint_t *src)
{
  coap_duplicate_detection_endpoint_t *endp = (coap_duplicate_detection_endpoint_t*) list_head(endpoint_list);
  coap_duplicate_detection_endpoint_t *endp_aux = NULL;
  while(endp != NULL) {
    if(coap_endpoint_cmp(&endp->endpoint, src)) {
      
      return endp;
    } 
    endp = endp->next;
  }
  /*Endpoint not already present, try to allocate a new one */
  endp = memb_alloc(&endpoint_memb);
  if(endp) {
    coap_endpoint_copy(&endp->endpoint, src);
    list_add(endpoint_list, endp);
    return endp;
  } else {
    endp_aux = endpoint_with_no_messages();
    if(endp_aux) {
      remove_endpoint(endp_aux);
      endp = memb_alloc(&endpoint_memb);
      coap_endpoint_copy(&endp->endpoint, src);
      list_add(endpoint_list, endp);
      return endp;
    } else {
      remove_first_endpoint();
      endp = memb_alloc(&endpoint_memb);
      coap_endpoint_copy(&endp->endpoint, src);
      list_add(endpoint_list, endp);
      return endp;
    }
  }
}
/*---------------------------------------------------------------------------*/
static coap_duplicate_detection_info_t*
get_info(void)
{
  coap_duplicate_detection_info_t *info = memb_alloc(&info_memb);
  if(!info) {
    info = remove_obsolete_info();
    if(!info) {
      remove_first_info();
      info = memb_alloc(&info_memb);
    }
  }
  return info;
}
/*---------------------------------------------------------------------------*/
void
coap_duplicate_detection_add(const coap_endpoint_t *src, uint16_t mid)
{
  coap_duplicate_detection_endpoint_t *endp = get_endpoint(src);
  coap_duplicate_detection_info_t *info = get_info();
  /* Check just in case */
  if(info && endp) {
    info->endpoint = &endp->endpoint;
    info->mid = mid;
    info->time = coap_timer_uptime();
    list_add(info_list, info);
    LOG_DBG("Added packet to endpoint: ");
    coap_endpoint_log(info->endpoint);
    LOG_DBG_(" with mid: %u\n", info->mid);
  }
}
/*---------------------------------------------------------------------------*/
bool
coap_duplicate_detection_is_duplicated(const coap_endpoint_t *src, uint16_t mid)
{
  uint64_t now = coap_timer_uptime();
  coap_duplicate_detection_info_t *info = (coap_duplicate_detection_info_t*) list_head(info_list);
  while(info != NULL) {
    if(coap_endpoint_cmp(info->endpoint, src) && (mid == info->mid)){
      if(now - info->time <= COAP_DUPLICATE_DETECTION_LIFETIME){
        LOG_DBG("Duplicated packet detected\n");
        return true;
      } else {
        LOG_DBG("Duplicated packet detected but out of lifetime -> removing\n");
        remove_info(info);
      }
    }
    info = info->next;
  }
  return false;
}
/*---------------------------------------------------------------------------*/
void
coap_duplicate_detection_init(void)
{
  list_init(endpoint_list);
  list_init(info_list);
}
/*---------------------------------------------------------------------------*/
/** @} */