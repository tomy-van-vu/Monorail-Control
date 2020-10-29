typedef enum {
  DOOR_CLOSE,
  DOOR_OPEN
} door_position;

typedef struct {
  door_position current_state;
  door_position next_state;
} SM_door;



    void MetroDoor_init(int);
    bool open_door();
    bool close_door();
    void compute(int);
