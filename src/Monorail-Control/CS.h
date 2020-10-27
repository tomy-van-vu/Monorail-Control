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


void colour_sensor_init();
bool check_colour_sensor(SM_colour_sensor *);
int read_colour();
