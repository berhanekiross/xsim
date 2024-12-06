#ifndef MOVECONTROLLERFLOW_H
#define MOVECONTROLLERFLOW_H

#include <xsim_config>
#include <unordered_map>

#include "movecontroller.h"

namespace xsim {

class EnterPort;
class Entity;
class Variant;
class Node;
class MoveStrategy;
class Flow;

/**
 * @brief The MoveControllerFlow moves entities based on the defined flows using move strategies.
 *        There are two modes. In 'Variant' mode each variant has its own move strategy and in
 *        'Flow' mode each flow has its own move strategy.
 */
class XSIM_EXPORT MoveControllerFlow : public MoveController {
 public:
     enum class Mode { VARIANT, FLOW };

     /** @brief Constructor. */
     MoveControllerFlow();

     /**
      * @brief Copy constructor
      *
      * @param  move_controller The move controller.
      */
     MoveControllerFlow(const MoveControllerFlow& move_controller);

     /** @brief Destructor */
     ~MoveControllerFlow();

     /** @brief Create connections based on the flows */
     void pre_simulation_init();

     MoveControllerFlow* clone() override;

     /**
      * @brief Gets the destination node for an entity.
      *        
      *        If a destination node is given the MoveControllerFlow will not try to send the entity
      *        elsewhere, even if the given node is not open.
      *
      * @param  entity      The entity for which to get the destination.
      * @param  destination A hint on where to send the entity.
      * @param  ignore_full True to ignore if a destination is full.
      *
      * @returns The destination node for an entity, or a nullptr if there are none.
      */
     Node* get_destination(Entity *entity, Node* destination, bool ignore_full = false) override;

     int successor_order(Node *node, Entity* entity) override;
     void entity_exited_node(Node* departure, Node* destination, Entity* entity) override;
     void forward_block(Entity *entity) override;
     bool has_destination(Entity* entity, Node* node) const override;
     void get_successors(std::vector<Node*>& nodes) const override;

     /**
      * @brief Sets the mode
      *        
      *        The mode can either be 'VARIANT' in which each variant get its own move strategy, or
      *        'FLOW' where each flow gets its own move strategy.
      *
      * @param  mode The mode to set.
      */
     virtual void set_mode(Mode mode);

     /**
      * @brief Select a move strategy based on a variant.
      *
      * @param  variant The variant to get the move strategy for.
      *
      * @returns The move strategy if successful, otherwise nullptr. 
      */
     virtual MoveStrategy* select_move_strategy(Variant* variant) const;

     /**
      * @brief Sets the move strategy for a variant.
      *
      * @param  variant       The variant for which to set the move strategy.
      * @param  move_strategy The move strategy to set.
      */
     virtual void set_move_strategy(Variant* variant, MoveStrategy* move_strategy);

     /**
      * @brief Adds a destination node to a move strategy.
      *
      * @param  flow        The flow the link belongs to, can be nullptr in 'VARIANT' mode.
      * @param  departure   The departure node.
      * @param  destination The destination node.
      * @param  variant     The variant this link is valid for.
      * @param  weight      The weight of the link.
      */
     virtual void add_destination(Flow* flow, Node* departure, Node* destination, Variant* variant, int weight);

 protected:
     /** @brief The current mode */
     Mode mode_;

     /** @brief All move strategies */
     std::unordered_map<Variant*, MoveStrategy*> move_strategies_;

     /** @brief The move strategies per flow, used for 'FLOW' mode */
     std::unordered_map<Flow*, MoveStrategy*> move_strategies_per_flow_;

     /** @brief The nodes this move controller belongs to */
     std::unordered_map<Node*, bool> nodes_;

     /** @brief The master move strategy */
     MoveStrategy* move_strategy_;
};

} // namespace xsim

#endif // MOVECONTROLLERFLOW_H
