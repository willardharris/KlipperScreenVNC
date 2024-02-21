
#define TOUCH_MODULES_GT911 
#define TOUCH_MODULE_ADDR GT911_SLAVE_ADDRESS1
#define TOUCH_SCL 9
#define TOUCH_SDA 8
#define TOUCH_RES -1
 #define TOUCH_INT -1

// Please fill below values from Arduino_GFX Example - TouchCalibration
bool touch_swap_xy = false;
int16_t touch_map_x1 = -1;
int16_t touch_map_x2 = -1;
int16_t touch_map_y1 = -1;
int16_t touch_map_y2 = -1;

int16_t touch_max_x = 0, touch_max_y = 0;
int16_t touch_raw_x = 0, touch_raw_y = 0;
int16_t touch_last_x = 0, touch_last_y = 0;

#include <Wire.h>
#include <TouchLib.h>
TouchLib touch(Wire, TOUCH_SDA, TOUCH_SCL, TOUCH_MODULE_ADDR);


void touch_init(int16_t w, int16_t h, uint8_t r)
{
  touch_max_x = w - 1;
  touch_max_y = h - 1;
  if (touch_map_x1 == -1)
  {
    switch (r)
    {
    case 3:
      touch_swap_xy = true;
      touch_map_x1 = touch_max_x;
      touch_map_x2 = 0;
      touch_map_y1 = 0;
      touch_map_y2 = touch_max_y;
      break;
    case 2:
      touch_swap_xy = false;
      touch_map_x1 = touch_max_x;
      touch_map_x2 = 0;
      touch_map_y1 = touch_max_y;
      touch_map_y2 = 0;
      break;
    case 1:
      touch_swap_xy = true;
      touch_map_x1 = 0;
      touch_map_x2 = touch_max_x;
      touch_map_y1 = touch_max_y;
      touch_map_y2 = 0;
      break;
    default: // case 0:
      touch_swap_xy = false;
      touch_map_x1 = 0;
      touch_map_x2 = touch_max_x;
      touch_map_y1 = 0;
      touch_map_y2 = touch_max_y;
      break;
    }
  }


  Wire.begin(TOUCH_SDA, TOUCH_SCL);
  touch.init();

}

bool touch_has_signal()
{

  return true;

}

void translate_touch_raw()
{
  if (touch_swap_xy)
  {
    touch_last_x = map(touch_raw_y, touch_map_x1, touch_map_x2, 0, touch_max_x);
    touch_last_y = map(touch_raw_x, touch_map_y1, touch_map_y2, 0, touch_max_y);
  }
  else
  {
    touch_last_x = map(touch_raw_x, touch_map_x1, touch_map_x2, 0, touch_max_x);
    touch_last_y = map(touch_raw_y, touch_map_y1, touch_map_y2, 0, touch_max_y);
  }
  // Serial.printf("touch_raw_x: %d, touch_raw_y: %d, touch_last_x: %d, touch_last_y: %d\n", touch_raw_x, touch_raw_y, touch_last_x, touch_last_y);
}

bool touch_touched()
{

  if (touch.read())
  {
    TP_Point t = touch.getPoint(0);
    touch_raw_x = t.x;
    touch_raw_y = t.y;

    touch_last_x = touch_raw_x;
    touch_last_y = touch_raw_y;

    translate_touch_raw();
    return true;
  }

  return false;
}

bool touch_released()
{

  return true;

}
