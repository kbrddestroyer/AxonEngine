#ifndef BACKEND_H
#define BACKEND_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(USE_BERKELEY)
#include "berkeley/basic_networking.h"
#else
	/* DEFAULT */
#endif

#include "berkeley/basic_networking.h"
#ifdef __cplusplus
}
#endif

#endif