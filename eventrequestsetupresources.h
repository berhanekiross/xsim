#ifndef EVENTREQUESTSETUPRESOURCES_H
#define EVENTREQUESTSETUPRESOURCES_H

#include <xsim_config>
#include <vector>

#include "event.h"

namespace xsim {

class Node;
class Entity;
class Simulation;
class NodeResource;

class XSIM_EXPORT EventRequestSetupResources : public Event {
 public:
     EventRequestSetupResources(
            NodeResource *resource,
            Entity *entity,
            Node *node,
            bool schedule_event_out,
            int priority,
            int sub_priority);

     /* Documented in event.h */
     void process() override;
     std::string sender() override;
     std::string receiver() override;
     std::string name() override;

     /**
      * @brief Getter for the Node associated with this event.
      *
      * @return Pointer to the node.
      */
     Node* node() const;

     /**
      * @brief Getter for the Entity associated with this event.
      *
      * @return Pointer to the entity.
      */
     Entity* entity() const;

 private:
     NodeResource *resource_;
     Entity *entity_;
     Node *node_;
     bool schedule_event_out_;
};

} // namespace xsim

#endif // EVENTREQUESTSETUPRESOURCES_H
