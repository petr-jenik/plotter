#pragma once

#include "embedded_cout.h"

#define LOG(message) (dbgCout << message << dbgEndl)

#define DBG(message) (dbgCout << message << dbgEndl)
