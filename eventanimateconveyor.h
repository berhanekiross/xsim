#ifndef EVENTANIMATECONVEYOR_H
#define EVENTANIMATECONVEYOR_H

#include <xsim_config>
#include <vector>

#include "event.h"

namespace xsim {

class Conveyor;

class XSIM_EXPORT EventAnimateConveyor: public Event {
 public:
     EventAnimateConveyor(Conveyor *conveyor_,
             int priority = PRIORITY_ANIMATE_CONVEYOR);

     /* Documented in event.h */
     void process() override;
     std::string sender() override;
     std::string receiver() override;
     std::string name() override;

 private:
     Conveyor *conveyor_;
};

} // namespace xsim

#endif // EVENTANIMATECONVEYOR_H
