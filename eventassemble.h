#ifndef EVENTASSEMBLE_H
#define EVENTASSEMBLE_H

#include <xsim_config>

#include "event.h"

namespace xsim {

class Assembly;
class Entity;
  
class XSIM_EXPORT EventAssemble : public Event {
 public:
     EventAssemble(Assembly *assembly, Entity *entity,
             int priority = PRIORITY_ASSEMBLY);

     /* Documented in event.h */
     void process() override;
     std::string sender() override;
     std::string receiver() override;
     std::string name() override;

     void set_entity(Entity *entity);

 private:
     Assembly *assembly_;
     Entity *entity_;
};

} // namespace xsim

#endif // EVENTASSEMBLE_H
