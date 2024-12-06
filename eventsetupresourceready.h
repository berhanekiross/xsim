#ifndef EVENTSETUPRESOURCEREADY_H
#define EVENTSETUPRESOURCEREADY_H

#include <xsim_config>
#include <vector>

#include "event.h"

namespace xsim {

class Node;
class Entity;
class Simulation;
class LogicResource;

class XSIM_EXPORT EventSetupResourceReady : public Event {
 public:
     EventSetupResourceReady(Entity *entity, Node *node,
             LogicResource *resource, bool schedule_event_out,
             std::string sender, int priority = PRIORITY_RESOURCE_READY);

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

     /**
      * @brief Getter for the LogicResource associated with this event.
      *
      * @return Pointer to the logic resource.
      */
     LogicResource* logic_resource() const;

 private:
     Entity *entity_;
     Node *node_;
     LogicResource *resource_;
     bool schedule_event_out_;
     std::string sender_;
};

} // namespace xsim

#endif // EVENTSETUPRESOURCEREADY_H
