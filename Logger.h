/*
 *  Logger.h
 *  tractionedge
 *
 *  Created by Steven Hamilton on 4/10/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

#ifndef LOGGER_H__
#define LOGGER_H__

#if defined(NDEBUG)
    #define BLAH_DISABLE_TRANSIENT_LOGS
#endif
#include "blah.h"

#define LOG BLAH_TRANSIENT_LOG("dbg")

#endif
