typedef enum {
  CS_NONE,
  CS_RED, 
  CS_GREEN, 
  CS_BLUE, 
  CS_YELLOW,
} sensor_colour;

typedef struct {
  sensor_colour current_colour;
  sensor_colour last_colour;
  bool colour_updated;  // if there is a new colour detected
} SM_colour_sensor;


typedef struct {
  int red_min;
  int red_max;
  int green_min;
  int green_max;
  int blue_min;
  int blue_max;
} sensor_range;





void colour_sensor_init();
bool check_colour_sensor(SM_colour_sensor *);
int read_colour();
