
class Motor{

    struct state
    {
        /* data */
    };

typedef enum {
  STOP, 
  START,
  EAST, 
  WEST,
  OPEN,
  CLOSE,
  EMERGENCY
}trainstate;
    



    public:
    Motor();
    void Stop();
    void Setspeed();
    void SetDistance();
    void Forward();
    void Reverse();



    private:

    long motorOut;
    long setPoint;


};