#ifndef EVENTUPDATECONVEYOR_H
#define EVENTUPDATECONVEYOR_H

#include <xsim_config>
#include <vector>

#include "event.h"

namespace xsim {

class Conveyor;
class Entity;

class XSIM_EXPORT EventUpdateConveyor: public Event {
 public:
     EventUpdateConveyor(Conveyor *conveyor_, Entity *entity,
             int priority = PRIORITY_UPDATE_CONVEYOR);

     /* Documented in event.h */
     void process() override;
     std::string sender() override;
     std::string receiver() override;
     std::string name() override;

 private:
     Conveyor *conveyor_;
     Entity *entity_;
};

} // namespace xsim

#endif // EVENTUPDATECONVEYOR_H
