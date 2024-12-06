#ifndef ENTERLOGIC_H
#define ENTERLOGIC_H

#include <xsim_config>
#include <string>
#include <list>

#include "logic.h"

namespace xsim {

class Node;
class Entity;
class Simulation;

class BlockItem {
public:
    Node *node;
    Entity *entity;
    simtime time;
    unsigned int sequence;
    int sucessor_order;
    int exits;
};

struct BlockItemSorter {
    bool operator() (const BlockItem *item1, const BlockItem *item2) const
    {
        if (item1->time < item2->time)
            return true;
        else if (item1->time == item2->time &&
                item1->sucessor_order < item2->sucessor_order)
            return true;
        else if (item1->time == item2->time &&
                item1->sucessor_order == item2->sucessor_order &&
                item1->sequence < item2->sequence)
            return true;

        return false;
    }
};

/**
 * @brief Determine if a block item has a particular entity.
 */
class SameEntity {
 public:
     /**
      * @brief Constructor.
      *
      * @param entity The entity to to compare against.
      */
     SameEntity(Entity *entity) { entity_ = entity; }

     /**
      * @brief Check if a block items entity matches the entity.
      *
      * @param item The block item.
      *
      * @return True if it matches.
      */
     bool operator() (const BlockItem *item) { return item->entity == entity_; }
 private:
     /**
      * @brief The entity that should be compared against block items.
      */
     Entity *entity_;
};

/**
 * @brief Base class for logics that want to prevent entity from entering
 * nodes.
 */
class XSIM_EXPORT EnterLogic : public virtual Logic {
 public:
     /**
      * @brief Constructor.
      */
     EnterLogic();

     ~EnterLogic();

     void init() override;
     void reset_stats() override;
     void finalize() override;

     /**
      * @brief Check if a entity are allowed to enter a node.
      *
      * @param node The node the entity wants to enter on.
      * @param entity The entity that wants to enter.
      *
      * @return True if it is allowed to enter.
      */
     virtual bool allow_enter(Node *node, Entity *entity) = 0;

     /**
      * @brief Add a entity to the forward blocking list.
      *
      * @param node The node the entity wanted to enter.
      * @param entity The entity that was blocked.
      */
     virtual void add_forward_blocking(Node *node, Entity *entity);

     /**
      * @brief Remove a entity from the forward blocking list.
      *
      * @param iterator The iterator to the entity on the block list.
      */
     virtual void remove_forward_blocking(
            const std::list<BlockItem*>::iterator &iterator);

 protected:
     typedef std::list<BlockItem*> BlockList;
     typedef BlockList::const_iterator BlockListIterator;

     /**
      * @brief Check if any forward blocked entity can now move.
      *
      * @param allow_move_to_all_succssors If true all successors are tried,
      * not only the original destination.
      */
     void check_forward_blocking(bool allow_move_to_all_successors = false);

     /**
      * @brief Check if the sorted block list is valid.
      *
      * @return True if the block list is still valid.
      */
     bool is_sorted_block_list_valid() const;

     /**
      * @brief All entities that have been blocked.
      */
     BlockList block_list_;

     /**
      * @brief A list of all free block items.
      */
     BlockList free_block_list_;

     /**
      * @brief Used to determine the block list order of entities.
      */
     unsigned int sequence_;

 private:
     /**
      * @brief Used for invalidating the sorted block list.
      */
     bool is_sorted_block_list_valid_;

     /**
      * @brief A sorted version of the block list. It contains only one item for
      * each destination.
      */
     BlockList sorted_block_list_;

     /**
      * @brief Used to sort the block list.
      */
     BlockItemSorter sorter_;
};

} // namespace xsim

#endif // ENTERLOGIC_H
