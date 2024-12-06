#ifndef MOVESTRATEGY_H
#define MOVESTRATEGY_H

#include <xsim_config>
#include <vector>

#include "object.h"

namespace xsim {

class Entity;
class Node;

/**
 * @brief The base class for determining where a entity should go next.
 */
class XSIM_EXPORT MoveStrategy : public Object {
 public:
     struct Link {
         Node *node;
         int weight;
     };

    typedef std::vector<MoveStrategy::Link*>::const_iterator ConstNodeIterator;
    typedef std::vector<MoveStrategy::Link*>::iterator NodeIterator;

     /** @brief Constructor. */
     MoveStrategy();

     /**
      * @brief Copy constructor
      *
      * @param  move_strategy The move strategy to copy.
      */
     MoveStrategy(const MoveStrategy& move_strategy);

     /**
      * @brief Destructor.
      */
     virtual ~MoveStrategy() = 0;

     /**
     * @brief Clone the move strategy.
     *
     * @return A clone of the move strategy.
     */
     virtual MoveStrategy* clone() const = 0;

     /**
      * @brief Get the next node that accepts a particular entity.
      *
      * @param  entity      The entity that is about to move.
      * @param  ignore_full If true the destination should ignore the fact that it is full when
      *                     deciding if the entity can enter.
      *
      * @returns The first destination that can accept the entity.
      */
     virtual Node* get_next_destination(Entity *entity, bool ignore_full) = 0;

     /**
      * @brief Get how far from the current successor a destination is.
      *        
      *        Each move strategy has an order in which successors are tried when moving an entity.
      *        This function returns how many other destinations should be tried before it is the
      *        given destination's turn.
      *
      * @param  node   The node to get the successor order for.
      * @param  entity The entity that is being moved.
      *
      * @returns The successor order.
      */
     virtual int successor_order(Node *node, Entity* entity) = 0;

     /**
      * @brief If a move strategy need to know when a entity leaves a node it can override this
      *        function.
      *
      * @param  entity The entity that left.
      * @param  node   The node the entity is about to enter.
      */
     virtual void entity_exited_node(Node *node, Entity *entity) {}

     /**
      * @brief Add a entity to the forward block list of all destinations.
      *
      * @param  entity The entity to add.
      */
     virtual void add_forward_blocking(Entity *entity);

     /**
      * @brief Add a destination.
      *
      * @param  node    The node to add.
      * @param  weight  The weight of the link.
      */
     virtual void add_destination(Node *node, int weight);

     /**
      * @brief Remove a node as a successor destination.
      *
      * @param node The node to remove.
      */
     virtual void remove_destination(Node* node);

     /**
      * @brief Delete all successor destinations.
      *
      */
     virtual void clear_destinations();

     /**
      * @brief Query if a node is among the successors.
      *
      * @param  node    The node.
      *
      * @returns True if node is among the successors, otherwise false.
      */
     virtual bool has_destination(Node* node) const;

     /**
      * @brief Gets all the successors.
      *
      * @param  nodes   The successor nodes.
      */
     virtual void get_successors(std::vector<Node*>& nodes) const;

     /**
      * @brief Get const begin iterator for the nodes in this move strategy.
      *
      * @returns The begin iterator for nodes.
      */
     ConstNodeIterator node_begin() const;

     /**
      * @brief Get const end iterator for the nodes in this move strategy.
      *
      * @returns The begin iterator for nodes.
      */
     ConstNodeIterator node_end() const;

     /**
      * @brief Get begin iterator for the nodes in this move strategy.
      *
      * @returns The begin iterator for nodes.
      */
     NodeIterator node_begin();

     /**
      * @brief Get end iterator for the nodes in this move strategy.
      *
      * @returns The begin iterator for nodes.
      */
     NodeIterator node_end();

     /**
      * @return All destination nodes.
      */
     const std::vector<Link*>& nodes() const;

     /**
      * @brief Get the node at a particular index.
      *
      * @param  index The index of the node to get.
      *
      * @returns The Link at the index.
      */
     MoveStrategy::Link* get_node(int index);
  
 private:
     /** @brief All possible destination for this move strategy. */
     std::vector<Link*> nodes_;
};

} // namespace xsim

#endif // MOVESTRATEGY_H
