#ifndef MOVECONTROLLER_H
#define MOVECONTROLLER_H

#include <xsim_config>
#include <unordered_map>

#include "object.h"

namespace xsim {

class EnterPort;
class Entity;
class Variant;
class Node;
class MoveStrategy;

/**
 * @brief The MoveController handles how entities move from one node to another.
 *        
 */
class XSIM_EXPORT MoveController : public Object {
 public:

     void simulation_init() override;

     /**
      * @brief Makes a deep copy of this object
      *
      * @returns Null if it fails, else a copy of this object.
      */
     virtual MoveController* clone() = 0;

     /**
      * @brief Gets the destination node for an entity.
      *        
      *        A destination node hint can be given on where to send the entity, it is up to the
      *        specific MoveController to use this information as it wants.
      *
      * @param  entity      The entity for which to get the destination.
      * @param  destination A hint on where to send the entity.
      * @param  ignore_full True to ignore if a destination is full.
      *
      * @returns The destination node for an entity, or a nullptr if there are none.
      */
     virtual Node* get_destination(Entity *entity, Node* destination, bool ignore_full = false) = 0;

     /**
      * @brief Gets the successor order of a given node for a particular entity.
      *        
      *        Each move controller has an order in which successors are tried when moving an entity.
      *        With respect to a entity this function returns how many other destinations should be
      *        tried before it is the given destination's turn.
      *
      * @param  node   The node to the get the successor order for.
      * @param  entity The entity that is moving.
      *
      * @returns An integer greater or equal to zero if a successor order can be determined, otherwise
      *          -1.
      */
     virtual int successor_order(Node* node, Entity* entity) = 0;

     /**
      * @brief Add an entity to all successor nodes forward block lists.
      *        
      *        When an entity is unable to move it can be added to the successor nodes forward block
      *        lists. So that the move can be retried when the blockage clears.
      *
      * @param  entity The entity that is blocked.
      */
     virtual void forward_block(Entity *entity) = 0;

     /**
      * @brief Query if a particular node, for a given entity, is among the successors.
      *
      * @param  entity The entity that is moving.
      * @param  node   The node that should be checked against the successors.
      *
      * @returns True if a destination is found, false if not.
      */
     virtual bool has_destination(Entity* entity, Node* node) const = 0;

     /**
      * @brief Gets all unique successor nodes.
      *
      * @param  nodes The successors nodes.
      */
     virtual void get_successors(std::vector<Node*>& nodes) const = 0;

     /**
      * @brief Override this function to perform actions when an entity exits an node.
      *
      * @param  departure   The node from which the entity left.
      * @param  destination The node to entity moved to.
      * @param  entity      The entity that moved.
      */
     virtual void entity_exited_node(Node* departure, Node* destination, Entity* entity);
};

} // namespace xsim

#endif // MOVECONTROLLER_H
