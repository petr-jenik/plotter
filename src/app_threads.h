/*
 * app_threads.h
 *
 *  Created on: 4. 2. 2017
 *      Author: apollo
 */

#ifndef APP_THREADS_H_
#define APP_THREADS_H_

#include <iostream>
#include <sstream>
#include <vector>
#include <thread>

#include "blocking_queue.h"

#include "global.h"
#include "gui_types.h"

void parser_loop(safe_queue<std::string> &queueInput, safe_queue<moveCommand> &queueOutput);
void reader_loop(safe_queue<std::string> &queueOutput);
void movementControl_loop(safe_queue<moveCommand> &queueInput, safe_queue<armCommand> &queueOutput);
void motorControl_loop(safe_queue<armCommand> &queueInput, safe_queue<stepperCommand> &queueOutput);
void GUI_loop(safe_queue<armCommand> &queueInput);

#endif /* APP_THREADS_H_ */
