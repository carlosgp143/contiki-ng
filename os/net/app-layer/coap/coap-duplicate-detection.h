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
 *        Header file for the detection of duplicated messages in CoAP
 * \author
 *         Carlos Gonzalo Peces <carlosgp143@gmail.com>
 */

#ifndef COAP_DUPLICATE_DETECTION_H_
#define COAP_DUPLICATE_DETECTION_H_

#include "coap-endpoint.h"
#include <stdbool.h>

typedef struct coap_duplicate_detection_info {
	struct coap_duplicate_detection_info *next;
	coap_endpoint_t *endpoint;
	uint16_t mid;
	uint64_t time;
} coap_duplicate_detection_info_t;

typedef struct coap_duplicate_detection_endpoint {
	struct coap_duplicate_detection_endpoint *next;
	coap_endpoint_t endpoint;
} coap_duplicate_detection_endpoint_t;

void coap_duplicate_detection_init(void);

void coap_duplicate_detection_add(const coap_endpoint_t *src, uint16_t mid);
bool coap_duplicate_detection_is_duplicated(const coap_endpoint_t *src, uint16_t mid);


#endif /* COAP_DUPLICATE_DETECTION_H_ */
/** @} */