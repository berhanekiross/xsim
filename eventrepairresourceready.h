#ifndef EVENTREPARRESOURCEREADY_H
#define EVENTREPARRESOURCEREADY_H

#include <xsim_config>
#include <vector>

#include "event.h"

namespace xsim {

class Node;
class Entity;
class Simulation;
class LogicResource;
class Failure;

class XSIM_EXPORT EventRepairResourceReady : public Event {
 public:
     EventRepairResourceReady(
            Node *node,
            LogicResource *resource,
            Failure *failure,
            std::string sender,
            int priority = PRIORITY_RESOURCE_READY);

     /* Documented in event.h */
     void process() override;
     std::string sender() override;
     std::string receiver() override;
     std::string name() override;

 private:
     Node *node_;
     LogicResource *resource_;
     Failure *failure_;
     std::string sender_;
};

} // namespace xsim

#endif // EVENTREPARRESOURCEREADY_H
