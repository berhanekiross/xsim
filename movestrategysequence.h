#ifndef MOVESTRATEGYSEQUENCE_H
#define MOVESTRATEGYSEQUENCE_H

#include <xsim_config>
#include <map>

#include "movestrategy.h"

namespace xsim {

class Node;
class Entity;
class Variant;

/**
 * @brief Tries to spread entities among all destinations by remembering
 * which destination that a entity last moved to.
 */
class XSIM_EXPORT MoveStrategySequence : public MoveStrategy {
 public:
     /**
      * @brief Constructor.
      */
     MoveStrategySequence();

     /**
      * @brief Destructor.
      */
     virtual ~MoveStrategySequence();

     void finalize() override;

     /* Documented in movestrategy.h */
     MoveStrategySequence* clone() const override;
     Node* get_next_destination(Entity *entity, bool ignore_full) override;
     int successor_order(Node *node, Entity* entity) override;
     void entity_exited_node(Node *node, Entity *entity) override;
     void add_forward_blocking(Entity *entity) override;

 private:
     struct Item {
         ConstNodeIterator node_itr;
         unsigned int count;
     };
     std::map<Variant*, Item*> items_;
};

} // namespace xsim

#endif // MOVESTRATEGYSEQUENCE_H
