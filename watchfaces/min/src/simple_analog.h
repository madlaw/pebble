// 144x168

#pragma once

#include "pebble.h"

#define NUM_CLOCK_TICKS 4 
#define NUM_BITS 6
#define NUM_BATT 5

static const struct GPathInfo ANALOG_BG_POINTS[] = {
  { 4, (GPoint []){
      {70, 0},
      {73, 0},
      {73, 6},
      {70, 6}
    }
  },
  { 4, (GPoint []){
      {0, 75},
      {6, 75},
      {6, 78},
      {0, 78}
    }
  },
  { 4, (GPoint []){
      {143, 75},
      {137, 75},
      {137, 78},
      {143, 78}
    }
  },
  { 4, (GPoint []){
      {70, 161},
      {73, 161},
      {73, 167},
      {70, 167}
    }
  },
};

static const GPathInfo MINUTE_HAND_POINTS =
{
  4,
  (GPoint []) {
    { -3, 20 },
    { 3, 20 },
    { 3, -70 },
    { -3, -70 }
  }
};

static const GPathInfo HOUR_HAND_POINTS = {
  4, (GPoint []){
    {-2, 20},
    {2, 20},
    {2, -50},
    {-2, -50}
  }
};

static const GPathInfo BINARY_SECONDS[] = {
  { 4, (GPoint []) {
      {88, 130},
      {91, 130},
      {91, 134},
      {88, 134}
    }
  },
  { 4, (GPoint []) {
      {81, 130},
      {84, 130},
      {84, 134},
      {81, 134}
    }
  },
  { 4, (GPoint []) {
      {74, 130},
      {77, 130},
      {77, 134},
      {74, 134}
    }
  },
  { 4, (GPoint []) {
      {67, 130},
      {70, 130},
      {70, 134},
      {67, 134}
    }
  },
  { 4, (GPoint []) {
      {60, 130},
      {63, 130},
      {63, 134},
      {60, 134}
    }
  },
  { 4, (GPoint []) {
      {53, 130},
      {56, 130},
      {56, 134},
      {53, 134}
    }
  },
};

static const GPathInfo BATTERY_METER[] = {
  { 4, (GPoint []) {
      {1, 0},
      {1, 5},
      {4, 5},
      {4, 0}
    }
  },
  { 4, (GPoint []) {
      {6, 0},
      {6, 5},
      {9, 5},
      {9, 0}
    }
  },
  { 4, (GPoint []) {
      {11, 0},
      {11, 5},
      {14, 5},
      {14, 0}
    }
  },
  { 4, (GPoint []) {
      {16, 0},
      {16, 5},
      {19, 5},
      {19, 0}
    }
  },
  { 4, (GPoint []) {
      {21, 1},
      {21, 4},
      {24, 4},
      {24, 1}
    }
  },
};
