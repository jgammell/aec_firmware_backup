/*
 * errors.h
 *
 *  Created on: Nov 13, 2020
 *      Author: jgamm
 */

#ifndef FUNCTIONS_HEADERS_ERRORS_H_
#define FUNCTIONS_HEADERS_ERRORS_H_

#include "global.h"

#define ASSERT_perm(CONDITION) if(!(CONDITION)) _assert_failure(#CONDITION, __FILE__, __LINE__)

#if DEBUG_ASSERTS == true
#define ASSERT_debug(CONDITION) if(!(CONDITION)) _assert_failure(#CONDITION, __FILE__, __LINE__)
#elif DEBUG_ASSERTS == false
#define ASSERT_debug(CONDITION)
#else
#error "Invalid value for DEBUG_ASSERTS in global.h"
#endif

void _assert_failure(char * expression, char * file, uint16_t line);

#endif /* FUNCTIONS_HEADERS_ERRORS_H_ */
