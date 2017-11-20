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
//#include "gui_types.h"

void parser_loop(safe_queue<std::string> &queueInput);//, safe_queue<moveCommand> &queueOutput);
void reader_loop(safe_queue<std::string> &queueOutput);
void gui_loop();

// TODO Move next declaration to separate file
void gui_add_line(const moveCommand& cmd);

//TODO Remove threads (13.10.2017)
void gui_parseCommand(armCommand inputCmd);

void reader_init(void);
void parser_init(void);
void stepperControl_init(void);
void movementControl_init(void);

void movementControl_createLine(const moveCommand& cmd);

void debug_loop(void);

#endif /* APP_THREADS_H_ */
