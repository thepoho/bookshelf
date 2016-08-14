#ifndef _COMMON_DEFINES_H
#define _COMMON_DEFINES_H

#include "mongoose.h"

#include "rapidjson/writer.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

using namespace std;

using namespace rapidjson;
using namespace std;

#define SIZEOF(n) ((int)(sizeof(n) / sizeof(n[0])))
// #define NULL (0)
#define FALSE (0)

#define ROWS (5)
#define COLS (5)

#define TRUE  (1)

#define LIGHTS_PER_SHELF (2)


#define LIGHTPIN_CLOCK (8)
#define LIGHTPIN_DATA (9)

#endif
