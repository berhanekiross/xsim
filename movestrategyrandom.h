#ifndef MOVESTRATEGYRANDOM_H
#define MOVESTRATEGYRANDOM_H

#include <xsim_config>
#include <random>

#include "movestrategy.h"

namespace xsim {

class Node;
class Entity;

/**
 * @brief Moves the entity to random destination.
 */
class XSIM_EXPORT MoveStrategyRandom : public MoveStrategy {
 public:
     /** @brief Default constructor */
     MoveStrategyRandom();

     /* Documented in movestrategy.h */
     MoveStrategyRandom* clone() const override;
     Node* get_next_destination(Entity *entity, bool ignore_full) override;
     int successor_order(Node *node, Entity* entity) override;
     void add_forward_blocking(Entity *entity) override;
};

} // namespace xsim

#endif // MOVESTRATEGYRANDOM_H
