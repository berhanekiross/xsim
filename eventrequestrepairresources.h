#ifndef EVENTREQUESTREPAIRRESOURCES_H
#define EVENTREQUESTREPAIRRESOURCES_H

#include <xsim_config>
#include <vector>

#include "event.h"

namespace xsim {

class Failure;
class Node;
class NodeResource;
class Entity;
class Simulation;

class XSIM_EXPORT EventRequestRepairResources : public Event {
 public:
     EventRequestRepairResources(
            NodeResource *resource,
            Node *node,
            Failure *failure,
            int priority,
            int sub_priority);

     /* Documented in event.h */
     void process() override;
     std::string sender() override;
     std::string receiver() override;
     std::string name() override;

 private:
     NodeResource *resource_;
     Node *node_;
     Failure *failure_;
};

} // namespace xsim

#endif // EVENTREQUESTREPAIRRESOURCES_H
