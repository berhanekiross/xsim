#ifndef MOVESTRATEGYSEQUENCEMU_H
#define MOVESTRATEGYSEQUENCEMU_H

#include <xsim_config>
#include <map>

#include "movestrategy.h"

namespace xsim {

class ExitPort;
class Node;
class Entity;

/**
 * @brief Tries to spread entities among all destinations by remembering
 * which destination that a entity last moved to.
 */
class XSIM_EXPORT MoveStrategySequenceEntity : public MoveStrategy {
 public:
     /**
      * @brief Constructor.
      */
     MoveStrategySequenceEntity();

     /**
      * @brief Destructor.
      */
     virtual ~MoveStrategySequenceEntity();

     /* Documented in movestrategy.h */
     MoveStrategySequenceEntity* clone() const override;
     Node* get_next_destination(Entity *entity, bool ignore_full) override;
     int successor_order(Node *node, Entity* entity) override;
     void entity_exited_node(Node *node, Entity *entity) override;
     void add_forward_blocking(Entity *entity) override;

     void on_entity_delete(Entity *entity);

 private:
     struct Item {
         ConstNodeIterator node_itr;
         unsigned int count;
     };
     std::map<Entity*, Item*> items_;
};

} // namespace xsim

#endif // MOVESTRATEGYSEQUENCEMU_H
