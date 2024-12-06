#ifndef EVENTDISASSEMBLE_H
#define EVENTDISASSEMBLE_H

#include <xsim_config>
#include <vector>

#include "event.h"

namespace xsim {

class Disassembly;
class Entity;

class XSIM_EXPORT EventDisassemble: public Event {
 public:
     EventDisassemble(Disassembly *disassembly,
             Entity *entity, int priority = PRIORITY_DISASSEMBLY);

     /* Documented in event.h */
     void process() override;
     std::string sender() override;
     std::string receiver() override;
     std::string name() override;

 private:
     Disassembly *disassembly_;
     Entity *entity_;
};

} // namespace xsim

#endif // EVENTDISASSEMBLE_H
