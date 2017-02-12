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

const std::string cKillSignal = "KONEC";

#include "blocking_queue.h"

#include "gui_types.h"

void reader_loop(safe_queue<std::string> &queueOutput);
void parser_loop(safe_queue<std::string> &queueInput, safe_queue<guiCommand> &queueOutput);
//void movementControl_loop(safe_queue<std::string> &queueInput, safe_queue<std::string> &queueOutput);
//void motorControl_loop(safe_queue<std::string> &queueInput, safe_queue<std::string> &queueOutput);
void GUI_loop(safe_queue<guiCommand> &queueInput);

#endif /* APP_THREADS_H_ */
