#ifndef CRITICALWIP_H
#define CRITICALWIP_H

#include <xsim_config>
#include <string>

#include "maxwip.h"
#include "int.h"

namespace xsim {

class Node;
class Entity;
class Simulation;

/**
 * @brief A logic that can limit the amount of entities in an area.
 */
class XSIM_EXPORT CriticalWip : public MaxWip {
 public:

     /**
      * @brief Constructor.
      */
     CriticalWip();

     void simulation_init() override;

     /* Documented in enterlogic.h */
     bool allow_enter(Node *node, Entity *entity) override;

     /**
      * @param  limit Set the maximum number of entities in this critical wip as percentage [0, 100] of
      *               the size of the connected store or buffer.
      */
     void set_limit(Int limit);

     /**
      * @returns The percent limit, [0, 100], on the maximum number of entities.
      */
     Int limit() const;

     /**
      * @brief Do the necessary actions when a entity front exits the max wip.
      *
      * @param  departure   The node the entity exited.
      * @param  destination The node the entity entered.
      * @param  entity      The entity that left.
      */
     void exiting(Node* departure, Node* destination, Entity* entity) override;

     /**
      * @brief Do the necessary actions when a entity rear exits the max wip.
      *
      * @param  departure   The node the entity exited.
      * @param  destination The node the entity entered.
      * @param  entity      The entity that left.
      */
     void exited(Node *departure, Node* destination, Entity *entity) override;

 private:
     /** @brief The percent limit, [0, 100] */
     Int limit_;
};

} // namespace xsim

#endif // CRITICALWIP_H
