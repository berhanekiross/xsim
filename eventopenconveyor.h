#ifndef EVENTOPENCONVEYOR_H
#define EVENTOPENCONVEYOR_H

#include <xsim_config>
#include <vector>

#include "event.h"

namespace xsim {

class Conveyor;
class Entity;

class XSIM_EXPORT EventOpenConveyor: public Event {
 public:
     EventOpenConveyor(Conveyor *conveyor_,
             Entity *entity, int priority = PRIORITY_CONVEYOR_OPEN);

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

#endif // EVENTOPENCONVEYOR_H
