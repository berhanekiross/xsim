#ifndef MOVESTRATEGYCYCLIC_H
#define MOVESTRATEGYCYCLIC_H

#include <xsim_config>
#include <random>

#include "movestrategy.h"

namespace xsim {

class EnterPort;
class Entity;
class Variant;

/**
 * @brief Tries to spread entities among all destinations by remembering
 * which destination that a entity last moved to.
 */
class XSIM_EXPORT MoveStrategyCyclic : public MoveStrategy {
 public:
     /**
      * @brief Constructor.
      */
     MoveStrategyCyclic();

     /**
      * @brief Destructor.
      */
     virtual ~MoveStrategyCyclic();

     void init() override;

     /* Documented in movestrategy.h */
     MoveStrategyCyclic* clone() const override;
     Node* get_next_destination(Entity *entity, bool ignore_full) override;
     int successor_order(Node *node, Entity* entity) override;
     void entity_exited_node(Node *node, Entity *entity) override;
     void add_forward_blocking(Entity *entity) override;

 private:
     ConstNodeIterator node_itr_;
};

} // namespace xsim

#endif // MOVESTRATEGYCYCLIC_H
