#ifndef MOVESTRATEGYWEIGHTED_H
#define MOVESTRATEGYWEIGHTED_H

#include <xsim_config>
#include <random>

#include "movestrategy.h"

namespace xsim {

class Entity;
class Variant;

/**
 * @brief Always tries the first destination, then the second and so on.
 */
class XSIM_EXPORT MoveStrategyWeighted : public MoveStrategy {
 public:
     /**
      * @brief Constructor.
      *
      * @param blocking If true only the first node will be considered for a move.
      */
     MoveStrategyWeighted(bool blocking);
     MoveStrategyWeighted(const MoveStrategyWeighted& move_strategy);

     void init() override;

     /* Documented in movestrategy.h */
     MoveStrategyWeighted* clone() const override;
     Node* get_next_destination(Entity *entity, bool ignore_full) override;
     int successor_order(Node *node, Entity* entity) override;
     void entity_exited_node(Node *node, Entity *entity) override;
     void add_forward_blocking(Entity *entity) override;

 private:
     std::discrete_distribution<> distribution_;

     /* If true only try the first node, resulting a blocking behaviour. */
     bool blocking_;

     Link *selected_node_;
};

} // namespace xsim

#endif // MOVESTRATEGYWEIGHTED_H
