/*
 * my_gui.h
 *
 *  Created on: 25. 2. 2018
 *      Author: apollo
 */

#ifndef OS_LINUX_MY_GUI_H_
#define OS_LINUX_MY_GUI_H_

#include "gui.h"

void gui_loop(void);
void gui_add_line(const guiCommand& cmd, eColor color);


#endif /* OS_LINUX_MY_GUI_H_ */
