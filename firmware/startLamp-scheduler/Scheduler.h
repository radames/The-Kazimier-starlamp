#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <Arduino.h>

#define MAX_EVENTS (5)

class Scheduler;
class SchedulerEvent
{
    friend class Scheduler;
  public:
    //SchedulerEvent();
    bool update(uint32_t now);

  protected:
    int _id;
    uint32_t _lastEventTime;
    uint32_t _period;
    uint32_t _startTime;
    uint32_t _endTime;
    void (*_callback)(int eventId);
};


class Scheduler
{
  public:
    Scheduler();

    void setEvent(int id, const char* startTime, const char* endTime, const char* period, void (*callback)(int eventId));

    void update(uint8_t hour, uint8_t min, uint8_t sec);
    void setStart(const char* time);
    void setEnd(const char* time);


  protected:
    static int _eventID;
    int _trackeventid;
    uint32_t _now_seconds;
    uint32_t _n_startTime;
    uint32_t _n_endTime;

    SchedulerEvent  _events[MAX_EVENTS];
};



#endif /* SCHEDULER_H_ */

