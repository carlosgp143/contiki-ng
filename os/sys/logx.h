/*
 * Copyright (c) 2018, RISE SICS.
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

/**
 * \file
 *         Header file for the extended logging system with error handler
 * \author
 *         Carlos Gonzalo <carlosgp143@gmail.com>
 */

/** \addtogroup sys
 * @{ */

/**
 * \defgroup extended log system with error handler
 * @{
 *
 * The extended log module performs error handling appart from per-module, per-level logging
 *
 */
#ifndef LOGX_H_
#define LOGX_H_

#include "contiki.h"
#include "sys/log.h"
#include "sys/logx_error_types.h"
#include <stdbool.h>


void logx_enable_error_handler(void);
void logx_disable_error_handler(void);
bool logx_is_error_handler_enabled(void);

#ifdef LOGX_ERROR_HANDLER
void LOGX_ERROR_HANDLER(logx_error_type_t type, const char *module,  int level);
#endif

static inline void 
logx_handler(logx_error_type_t type, const char *module, int level)
{
#ifdef LOGX_ERROR_HANDLER
  if(logx_is_error_handler_enabled()) {
    LOGX_ERROR_HANDLER(type, module, level);
  }
#endif
}

#define LOGX_WARN(T, ...) do {  \
                            LOG_WARN(__VA_ARGS__); \
                            logx_handler(T, LOG_MODULE, LOG_LEVEL_WARN); \
                          } while (0)

#define LOGX_ERR(T, ...)  do {  \
                            LOG_ERR(__VA_ARGS__); \
                            logx_handler(T, LOG_MODULE, LOG_LEVEL_ERR); \
                          } while (0)
#endif /* LOGX_H_ */

/** @} */
/** @} */