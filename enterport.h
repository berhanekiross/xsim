#ifndef ENTERPORT_H
#define ENTERPORT_H

#include <xsim_config>
#include <list>
#include <string>
#include <map>

#include "object.h"

namespace xsim {

class Node;
class Dispatch;
class EnterLogic;
class EventOut;
class Entity;
class Order;
class SetupTable;
class NumberGenerator;
class Variant;

/**
 * @brief A destination handles how entities enter nodes.
 */
class XSIM_EXPORT EnterPort: public Object {
 public:
     typedef std::pair<EnterLogic*, Variant*> EnterItem;

     /**
      * @brief Constructor.
      *
      * @param node The node this destination belongs to.
      */
     EnterPort(Node *node);

     void init() override;
     void finalize() override;
     void reset_stats() override;

     /**
      * @brief Set if the node should ignore that is full when accepting new
      * entities.
      *
      * @return True if the node should ignore that is full when accepting new
      * entities.
      */
     void set_ignore_full(bool value);

     /**
      * @brief Check if the node should ignore that is full when accepting new
      * entities.
      *
      * @return True if the node should ignore that is full when accepting new
      * entities.
      */
     bool ignore_full() const;

     /**
      * @brief Get the number of entities on the block list.
      *
      * @return The number of entities on the block list.
      */
     size_t num_entities_on_block_list() const;

     /**
      * @brief Check if this node is open and ready to recive entities.
      *
      * NOTE: This only indicates that this node can accept entities, entites
      * must still be checked individually with 'is_open' if they can enter.
      *
      * @param ignore_full Usually when a node is full the entity is
      * denied, but if 'ignore_full' is true that aspect is ignored. It could
      * still deny the entity on other grounds though.
      *
      * @return True if it is open.
      */
     bool can_accept_entites(bool ignore_full = false);

     /**
      * @brief Check if the node can accept a entity.
      *
      * @param entity The entity that wants to enter.
      * @param ignore_full Usually when a node is full the entity is
      * denied, but if 'ignore_full' is true it ignore that part. It could
      * still deny the entity on other grounds though.
      *
      * @return True if the entity is allowed to enter.
      */
     virtual bool is_open(Entity *entity, bool ignore_full = false);

     /**
      * @brief Open or closes the node.
      *
      * @param value True if it should be open.
      */
     void open(bool value);

     /**
      * @brief Get the number of entity that have entered this
      * destination.
      *
      * @return The number of entries.
      */
     unsigned int entries() const;

     /**
      * @brief Set the assembly identity of the last entered entity.
      *
      * @param entity The entity that last entered.
      */
     void set_last_assembly_identity(Entity *entity);

     /**
      * @brief Get the assembly identity of the last entered entity.
      *
      * @return The entity that last entered.
      */
     Entity* last_assembly_identity() const;

     /**
      * @brief Add a entity to the forward block list.
      *
      * @param entity The entity.
      */
     void add_forward_blocking(Entity *entity);

     /**
      * @brief Remove a entity from the the forward block list.
      *
      * @param entity The entity.
      * @param iterator The iterator to the entity on the block list.
      */
     void remove_forward_blocking(
            Entity *entity,
            std::list<Entity*>::iterator &iterator);

     /**
      * @brief Check the forward blocking list and schedule out events on those
      * entities that are allowed to enter.
      *
      * It only ever schedules one out event for each variant. This is done to
      * increase the performance, since most of the out events will be blocked
      * anyway. This though requires that subsequent out events are placed in
      * the right order in the event list.
      *
      * @param node The node that is displayed as the sender of the out
      * event, does not affect the behaviour of the function.
      *
      * @param allow_move_to_all_successors Every entity on the forward
      * block list have a destination. The default behaviour is to only check
      * that destination, but with this option it possible to check all
      * possible successors.
      */
     void check_forward_blocking(Node *node,
             bool allow_move_to_all_successors = false);

     /**
      * @brief Schedule an unblocked out event for a entity on the
      * forward block list.
      *
      * Only one event is scheduled at a time, but there is a mechanism in the
      * out event that allows the scheduling to resume where it left off. This
      * is so because we don't want to schedule an out event for all entities
      * on the block list, because this would be slow since most of the
      * out events would be blocked anyway. So instead we just schedule one out
      * event and after it has been processed we pick up the scheduling where
      * it left off.
      *
      * @param schedule_now If true schedules any entity immediately
      * after the current event, if false it schedules it at time zero but it
      * could happen after other events.
      * @param block_list_call True if scheduling is triggered from blocklist
      * otherwise False.
      */
     void schedule_entity(bool schedule_now, bool block_list_call);

     /**
      * @brief Add enter logic that must allow all entity that wants to
      * enter this destination.
      *
      * @param logic The logic.
      * @param variant The variant that this enter logic should apply too, if
      * zero it applies to all variants.
      */
     void add_enter_logic(EnterLogic *logic, Variant *variant);

     /**
      * @brief Set the dispatcher that determines the order that entities
      * in the block list are sorted after.
      *
      * @param dispatcher The dispatcher.
      */
     void set_dispatcher(Dispatch *dispatcher);

     /**
      * @brief Set a order that is connected to this destination.
      *
      * @param order The order.
      */
     void set_order(Order *order);

     /**
      * @brief Get the order connected to this destination. Returns null if no
      * orders are connected.
      *
      * @return The order that is connected to this destination.
      */
     Order* order() const;

     /**
      * @brief Increase the entries by one.
      */
     void entry();

     /**
      * @brief Adds a predecessor node for a specific variant.
      *
      * @param node    The predecessor node.
      * @param variant The variant.
      */
     void add_predecessor_node(Node* node, Variant *variant);

     /**
      * @brief Gets predecessors by variant
      *
      * @param variant The variant for which to get the predecessors.
      *
      * @returns All predecessors for specific variant.
      */
     std::vector<Node*> get_predecessors_by_variant(Variant *variant) const;

 private:
     /**
      * @brief Sort block list according to the dispatching rule.
      *
      * @return The sorted block list.
      */
     std::list<Entity*>* sort_block_list();

     /**
      * @brief If true this destination can accept new entities.
      */
     bool open_;

     /**
      * @brief The number of entities that have entered this destination.
      */
     unsigned int entries_;

     /**
      * @brief The block list with all entities that wants to enter this
      * destination, sorted by the order they tried to enter.
      */
     std::list<Entity*> forward_blocking_;

     /**
      * @brief The enter logics that can prevent a entity from entering
      * this destination.
      */
     std::list<EnterItem> logics_;

     /**
      * @brief True if at least one of the enter logics is a batch logic.
      */
     bool have_batch_logic_;

     /**
      * @brief True if the destination should ignore that it is full when
      * accepting new entities.
      */
     bool ignore_full_;

     /**
      * @brief The last variant processed.
      */
     Entity *last_entity_;

     /**
      * @brief Used to sort the forward block list, if it is null then fifo is
      * used.
      */
     Dispatch *dispatcher_;

     /**
      * @brief The last event out to be scheduled from the block list.
      */
     EventOut *block_list_event_out_;

     /**
      * @brief The next entity to schedule from the sorted block list.
      */
     std::list<Entity*>::iterator block_list_it_;

     /**
      * @brief The currently active sorted block list.
      */
     std::list<Entity*> *sorted_block_list_;

     /**
      * @brief The destination that should be used when scheduling unblocked
      * out events from the sorted block list.
      */
     Node *block_list_destination_;

     /**
      * @brief The departure that should be used when scheduling unblocked
      * out events from the sorted block list.
      */
     Node *block_list_departure_;

     /**
      * @brief The order that is connected to this destination. null if no
      * orders are connected.
      */
     Order *order_;

     /**
      * @brief The node that owns this destination.
      */
     Node *node_;

     /** @brief The predecessors of this destination */
     std::map<Variant*, std::vector<Node*>> predecessors_;
};

} // namespace xsim

#endif // ENTERPORT_H
