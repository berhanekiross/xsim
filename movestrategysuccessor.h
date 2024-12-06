#ifndef MOVESTRATEGYSUCCESSOR_H
#define MOVESTRATEGYSUCCESSOR_H

#include <xsim_config>

#include "movestrategy.h"

namespace xsim {

class Entity;

/**
 * @brief Always tries the first destination, then the second and so on.
 */
class XSIM_EXPORT MoveStrategySuccessor : public MoveStrategy {
 public:
     /**
      * @brief Constructor.
      */
     MoveStrategySuccessor();

     /* Documented in movestrategy.h */
     MoveStrategySuccessor* clone() const override;
     Node* get_next_destination(Entity *entity, bool ignore_full) override;
     int successor_order(Node *node, Entity* entity) override;
};

} // namespace xsim

#endif // MOVESTRATEGYSUCCESSOR_H
