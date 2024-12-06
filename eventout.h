#ifndef EVENTOUT_H
#define EVENTOUT_H

#include <xsim_config>
#include <vector>

#include "event.h"

namespace xsim {

class Node;
class EnterPort;
class Entity;

class XSIM_EXPORT EventOut : public Event {
 public:
     EventOut(Entity *entity,
              Node *departure,
              Node *destination = 0,
              int priority = PRIORITY_OUT);

     /* Documented in event.h */
     void process() override;
     std::string sender() override;
     std::string receiver() override;
     std::string name() override;

     void set_schedule_enter_port(EnterPort *enter_port) { schedule_enter_port_ = enter_port; }
     bool schedule_entity() const { return schedule_enter_port_ != 0; }
     void set_out_time(double time) { out_time_ = time; }
     double out_time() const { return out_time_; }
     Entity* entity() const { return entity_; }

 private:
     Entity *entity_;
     Node *departure_;
     Node *destination_;
     EnterPort *schedule_enter_port_;
     double out_time_;
};

} // namespace xsim

#endif // EVENTOUT_H
