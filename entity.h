#ifndef ENTITY_H
#define ENTITY_H

#include <xsim_config>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "entitytime.h"
#include "propertycontainer.h"
#include "signal.hpp"

namespace xsim {

class BlockItem;
class MoveStrategy;
class Node;
class EnterLogic;
class EventRequestProcessingResources;
class EventRequestSetupResources;
class EventProcessingResourceReady;
class EventSetupResourceReady;
class PropertyContainer;
class Store;
class Variant;

/**
 * @brief A unit that can move around among nodes.
 */
class XSIM_EXPORT Entity {
 public:
     Signal<void (Entity*)> deleted;

     /**
      * @brief Constructor.
      */
     Entity();

     /**
      * @brief Destructor.
      */
     ~Entity();

     /**
      * @brief Initialized the entity.
      *
      * @param variant The variant the entity is.
      * @param id A unique identifier.
      * @param node The node that created the entity.
      * @param units Specifies the number of units this part represents.
      */
     void init(Variant *variant, unsigned int id, Node *node, int units);

     /**
      * @brief Set which node this entity currently is on.
      *
      * @param  node The node to set.
      */
     void set_departure(Node *node);

     /**
      * @brief Get which departure node this entity currently is on.
      *
      * @return The departure node.
      */
     Node* departure() const;

     /**
      * @brief Set which node this entity is heading to.
      *
      * @param  node The node to set.
      */
     void set_destination(Node *node);

     /**
      * @brief Get which destination node this entity is heading to.
      *
      * @return The destination node.
      */
     Node* destination() const;

     /**
      * @brief Perform action when this entity leaves a node.
      *
      * @param node The new destination node of this entity.
      */
     void leave(Node *node);

     /**
      * @brief Get the unique identifier.
      *
      * @return The id.
      */
     unsigned int id() const;

     /**
      * @brief Get the name of the entity.
      *
      * @return The name.
      */
     std::string name() const;

     /**
      * @brief Get which variant this entity is.
      *
      * @return The variant.
      */
     Variant* variant() const { return variant_; }

     /**
      * @brief Set the simulation time this entity entered the model.
      *
      * @param time The time it entered.
      */
     void set_model_enter_time(simtime time) { model_enter_time_ = time; }

     /**
      * @brief Get the simulation time this entity entered the model.
      *
      * @return The time it entered.
      */
     simtime model_enter_time() const { return model_enter_time_; }

     /**
      * @brief Set the entity that should be the assembly identity.

      * The assembly identify is the entity that is used for routing,
      * determining setup and processing times.
      *
      * @param entity The new assembly identity.
      */
     void set_assembly_identity(Entity *entity)
     {
         assembly_identity_ = entity;
     }

     /**
      * @brief Get the assembly identity.
      *
      * The assembly identity is found by following the assembly identities
      * until a entity is found that does not have a assembly
      * identity, then that entity is the assembly identity.
      *
      * @return The assembly identity.
      */
     Entity* assembly_identity();
     const Entity* assembly_identity() const;

     /**
      * @brief Get the first assembly identity.
      *
      * Get the assembly identity of this entity, without recursion.
      *
      * @return The assembly identity.
      */
     Entity* first_assembly_identity() const;

     /**
      * @brief Clear the assembly identity.
      */
     void clear_assembly_identity() { assembly_identity_ = 0; }

     /**
      * @brief Record that a entity left the model.
      *
      * @param time The time the entity left the model.
      */
     void exit(simtime time);

     /**
      * @brief Add time this entity have contributed to the work in
      * process time of a node.
      *
      * @param now The current simulation time.
      * @param warmup The simulation time when statistics gathering started.
      */
     void add_wip_time(simtime now, simtime warmup);

     /**
      * @brief Add another entity has a part.
      *
      * @param entity The entity to add.
      */
     void add_part(Entity *entity);

     /**
      * @brief Remove an assembled entity.
      *
      * @param entity The entity to remove.
      *
      * @return True if a part was removed.
      */
     bool remove_part(Entity *entity);

     /**
      * @brief Get all assembled parts.
      *
      * @param entities The container that will be filled with the parts.
      * @param recursive If true any parts that also have assembled parts will
      * also be returned.
      */
     void get_parts(std::vector<Entity*> &entities,
             bool recursive = true);

     /**
      * @brief Clear any assembled parts.
      *
      * @param recursive If true any parts that also have assembled parts will
      * also be cleared.
      */
     void clear_parts(bool recursive = true);

     /**
      * @brief Get how many parts that are assembled onto this entity, it
      * is recursivly applied to all parts.
      *
      * @return The total number of entities on this entity.
      */
     unsigned int get_total_parts() const;

     /**
     * @brief Increment out event counter.
     */
     void increment_out_event_counter();

     /**
     * @brief Decrement out event counter.
     */
     void decrement_out_event_counter();

     /**
      * @brief Get if an out event is associated with this entity.
      *
      * @return value True if an out event is scheduled.
      */
     bool is_out_event_scheduled() const;

     /**
      * @brief Set which batch this entity belongs to.
      *
      * @param batch_id The batch id.
      */
     void set_batch_id(unsigned int batch_id);

     /**
      * @brief Get which batch this entity belongs to.
      *
      * @return The batch id.
      */
     unsigned int batch_id() const;

     /**
      * @brief Call all delete subscribers.
      */
     void on_delete();

     /**
      * @brief Add a destination that currently blocks this entity from
      * entering.
      *
      * @param node The node that blocks this entity.
      * @param iterator The iterator to the entity on the destination
      * block list.
      * @param time The current simulation time.
      */
     void add_forward_blocking(
            Node *node,
            const std::list<Entity*>::iterator &iterator,
            simtime time);

     /**
      * @brief Add an enter logic where this entity is blocked.
      *
      * @param enter_logic The enter logic where this entity is blocked.
      * @param iterator The iterator to the entity on the enter logics
      * block list.
      */
     void add_logic_forward_blocking(
            EnterLogic *enter_logic,
            const std::list<BlockItem*>::iterator &iterator);

     /**
      * @brief Clear this entity from all forward blocking lists.
      */
     void clear_forward_blocking();

     /**
      * @brief Check if this entity is blocked from entering.
      *
      * @return True if blocked.
      */
     bool is_forward_blocked() const;

     /**
      * @brief Set that this entity is blocked from leaving.
      */
     void set_exit_blocking();

     /**
      * @brief Set that this entity is no longer blocked from leaving.
      */
     void clear_exit_blocking();

     /**
      * @brief Check if this node is blocked from leaving.
      *
      * @return True if blocked.
      */
     bool is_exit_blocked() const;

     /**
      * @brief Get the simulation time this entity where blocked from
      * entering.
      *
      * @return The simulation time when it got forward blocked.
      */
     simtime start_blocked() const;

     /**
      * @brief Getter for amount of units on this entity.
      *
      * @return The amount of units.
      */
     int units() const;

     /**
      * @brief Add units.
      *
      * @param value The number of units to add.
      */
     void add_units(int value);

     /**
      * @brief Decrease the amount of units on this entity.
      *
      * @param value The amount of units to subtract.
      */
     void decrement_units(int value);

     /**
      * @brief Set the number of units.
      *
      * @param value The number of units.
      */
     void set_units(int value);

     /**
      * @brief Set if this entity should be able to overtake.
      *
      * @param value True if it is allowed to overtake.
      */
     void set_overtake(bool value);

     /**
      * @brief Getter for if the entity is allowed to overtake.
      *
      * @return True if the entity is allowed to overtake.
      */
     bool overtake() const;

     /**
      * @brief Set if this entity requires disassembly.
      *
      * @param value True if requires disassembly.
      */
     void set_require_disassembly(bool value);

     /**
      * @brief Getter for if the entity requires disassembly.
      *
      * @return True if the entity requires disassembly.
      */
     bool require_disassembly() const;

     /**
      * @brief Addition of Node to the map used to keep track of
      * which (Processing) NodeResources are added to
      * the block lists of connected ResourceManagers.
      *
      * @param node The node whose (Processing) NodeResource
      * are on the block lists of connected
      * ResourceManagers.
      */
     void add_to_processing_resource_block_list(Node *node);

     /**
      * @brief Remove all (Processing) NodeResources associated with this
      * entity from all ResourceManagers.
      *
      * @param exception_node If other than 0 NodeResources
      * of this Node will be not be
      * emoved from the block lists.
      */
     void remove_from_processing_resource_block_list(Node *exception_node);

     /**
      * @brief Addition of Node to the map used to keep track of
      * which (Setup) NodeResources are added to
      * the block lists of connected ResourceManagers.
      *
      * @param node The node whose (Setup) NodeResource
      * are on the block lists of connected ResourceManagers.
      */
     void add_to_setup_resource_block_list(Node *node);

     /**
      * @brief Remove all (Setup) NodeResources associated with this
      * entity from all ResourceManagers.
      *
      * @param exception_node If other than 0 NodeResources
      * of this Node will be not be removed from the block lists.
      */
     void remove_from_setup_resource_block_list(Node *exception_node);

     /**
      * @brief Addition of processing resource request event to member
      * variable used for internal record keeping.
      *
      * @param evt The event to keep track of.
      */
     void add_request_processing_resources_event(EventRequestProcessingResources *evt);

     /**
      * @brief Removes request reosurce event form internal list. Used when
      * the event is processed and Simulation handles deletion of
      * the event and also when events are freed by a node
      * and its events reference list.
      *
      * @param evt  Event to remove.
      */
     void remove_request_processing_resources_event(EventRequestProcessingResources *evt);

     /**
      * @brief Removes and deletes any lingering request resource events.
      * Used when request is satisfied, i.e. all needed skills
      * are allocated and ready, and addiontal requests aren't
      * needed.
      *
      * References to these events are also stored on nodes,
      * and any freed event is also removed from list on
      * the node.
      *
      * @param node Pointer to node who was assigned resources.
      * @param sim Pointer to simulation from which the events should be
      * removed.
      */
     void remove_request_processing_resources_events(Node *node);

     /**
      * @brief Trigger undo of request processing resource events and
      * processing resource ready events connected to this entity.
      */
     void undo_request_processing_resource();

     /**
      * @brief Addition of setup resource request event to member
      * variable used for internal record keeping.
      *
      * @param evt The event to keep track of.
      */
     void add_request_setup_resources_event(EventRequestSetupResources *evt);

     /**
      * @brief Removes request reosurce event form internal list. Used when
      * the event is processed and Simulation handles deletion of
      * the event and also when events are freed by a node
      * and its events reference list.
      *
      * @param evt  Event to remove.
      */
     void remove_request_setup_resources_event(EventRequestSetupResources *evt);

     /**
      * @brief Removes and deletes any lingering request resource events.
      * Used when request is satisfied, i.e. all needed skills
      * are allocated and ready, and addiontal requests aren't
      * needed.
      *
      * References to these events are also stored on nodes,
      * and any freed event is also removed from list on
      * the node.
      *
      * @param node Pointer to node who was assigned resources.
      * @param sim Pointer to simulation from which the events should be
      * removed.
      */
     void remove_request_setup_resources_events(Node *node);

     /**
      * @brief Trigger undo of request setup resource events and
      * setup resource ready events connected to this entity.
      */
     void undo_request_setup_resource();

     /**
      * @brief Addition of processing resource ready event to member
      * variable used for internal record keeping.
      *
      * @param evt The event to keep track of.
      */
     void add_processing_resource_ready_event(EventProcessingResourceReady *evt);

     /**
      * @brief Removes request reosurce event form internal list. Used when
      * the event is processed and Simulation handles deletion of
      * the event and also when events are freed by a node
      * and its events reference list.
      *
      * @param evt  Event to remove.
      */
     void remove_processing_resource_ready_event(EventProcessingResourceReady *evt);

     /**
      * @brief Removes and deletes any lingering request resource events.
      * Used when request is satisfied, i.e. all needed skills
      * are allocated and ready, and addiontal requests aren't
      * needed.
      *
      * References to these events are also stored on nodes,
      * and any freed event is also removed from list on
      * the node.
      *
      * @param node Pointer to node who was assigned resources.
      * @param sim Pointer to simulation from which the events should be
      * removed.
      */
     void remove_processing_resource_ready_events(Node *node);

     /**
      * @brief Addition of setup resource ready event to member
      * variable used for internal record keeping.
      *
      * @param evt The event to keep track of.
      */
     void add_setup_resource_ready_event(EventSetupResourceReady *evt);

     /**
      * @brief Removes request reosurce event form internal list. Used when
      * the event is processed and Simulation handles deletion of
      * the event and also when events are freed by a node
      * and its events reference list.
      *
      * @param evt  Event to remove.
      */
     void remove_setup_resource_ready_event(EventSetupResourceReady *evt);

     /**
      * @brief Removes and deletes any lingering request resource events.
      * Used when request is satisfied, i.e. all needed skills
      * are allocated and ready, and addiontal requests aren't
      * needed.
      *
      * References to these events are also stored on nodes,
      * and any freed event is also removed from list on
      * the node.
      *
      * @param node Pointer to node who was assigned resources.
      * @param sim Pointer to simulation from which the events should be
      * removed.
      */
     void remove_setup_resource_ready_events(Node *node);

     /**
      * @brief Can be used to store the iterator to a contents list.
      *
      * @param iterator The iterator to store.
      */
     void set_contents_iterator(std::list<EntityTime>::iterator iterator);

     /**
      * @brief Retrieve the iterator to a contents list.
      *
      * @return The iterator that was stored.
      */
     std::list<EntityTime>::iterator contents_iterator() const;

     /**
      * @brief Used to tell wheter or not this entity should be deleted
      * before it leaves a node.
      *
      * @return True if this entity should be deleted once it leaves a node.
      */
     bool delete_entity() const;

     /**
      * @brief Setter for delete_entity_.
      *
      * @param value The value to set.
      */
     void set_delete_entity();

     /**
      * @brief Boolean stating if move of entity was triggered from a
      * block list or not.
      *
      * @return If true the current call originated from a
      * triggered block list.
      */
     bool block_list_call() const;

     /**
      * @brief Set if move of entity is triggered from block list or not.
      *
      * @param value The value to set.
      */
     void block_list_call(bool value);

     /**
      * @brief Get the property container.
      *
      * @return The property container.
      */
     PropertyContainer* const properties() const;

     /**
      * @brief Finalize statistics and remove the entity from the model, if no one else share
      * ownership of this entity it is deleted.
      */
     void exit_model();

     /**
      * @brief Add a shared ownership of this entity.
      *
      * An entity is normally only owned by the object where it is currently located. This function
      * can be used to share ownership, in order to guarantee that the object is not destroyed
      * until all ownerships are released.
      *
      */
     void add_shared();

     /**
      * @brief Release a shared ownership of this entity.
      *
      * When there are no more owners of an entity it is destroyed.
      */
     void release_shared();

     void* operator new(size_t size);
     void operator delete(void* ptr, size_t size);
     void* operator new[](size_t size) = delete;
     void* operator new[](size_t size, size_t n) = delete;

 private:
     /**
      * @brief The variant this entity is.
      */
     Variant *variant_;

     /**
      * @brief A unique identifier.
      */
     unsigned int id_;

     /**
      * @brief The departure node that this entity is currently on.
      */
     Node *departure_;

     /**
      * @brief The simulation time the entity entered the model.
      */
     simtime model_enter_time_;

     /**
      * @brief The amount of units present on this entity.
      */
     int units_;

     /**
      * @brief All destinations that blocks this entity from entering.
      */
     std::vector<std::pair<Node*, std::list<Entity*>::iterator>> forward_blocking_;

     /**
      * @brief All enter logics that this entity is blocked on.
      */
     std::vector<std::pair<EnterLogic*, std::list<BlockItem*>::iterator>> logic_forward_blocking_;

     /**
      * @brief The departure that blocks the entity from leaving.
      */
     Node *exit_blocking_;

     /**
      * @brief All assembled parts.
      */
     std::vector<Entity*> parts_;

     /**
      * @brief Counter for scheduled out events.
      */
     unsigned int out_event_counter_;

     /**
      * @brief The batch id that this entity belongs to, if zero it does
      * not belong to any.
      */
     unsigned int batch_id_;

     /**
      * @brief The simulation time the entity was forward blocked.
      */
     simtime start_blocked_;

     /**
      * @brief The entity used for determining which route to use and
      * which setup and processsing time to use. The default is this entity.
      */
     Entity *assembly_identity_;

     /**
      * @brief Used to keep track of which node ordered this entity.
      */
     Node *destination_;

     /**
      * @brief True if this entity is allowed to overtake.
      */
     bool overtake_;

     /**
      * @brief True if this entity requires disassembly.
      */
     bool require_disassembly_;

     /**
     * @brief True if wip time is added for this entity.
     */
     bool wip_time_added_;

     /**
      * @brief Map use to keep track of all outstanding processing resource
      * request events associated with this entity.
      */
     std::map<EventRequestProcessingResources*, bool> request_processing_resources_events_;

     /**
      * @brief Map use to keep track of all outstanding setup resource
      * request events associated with this entity.
      */
     std::map<EventRequestSetupResources*, bool> request_setup_resources_events_;

     /**
      * @brief Map use to keep track of all outstanding processing resource
      * ready events associated with this entity.
      */
     std::map<EventProcessingResourceReady*, bool> processing_resource_ready_events_;

     /**
      * @brief Map use to keep track of all outstanding setup resource
      * ready events associated with this entity.
      */
     std::map<EventSetupResourceReady*, bool> setup_resource_ready_events_;

     /**
      * @brief Map used to keep track of all Nodes whose processing
      * NodeResoruce are on block lists of connected ResourceManagers.
      */
     std::map<Node*, bool> processing_resource_block_lists_;

     /**
      * @brief Map used to keep track of all Nodes whose setup
      * NodeResoruce are on block lists of connected ResourceManagers.
      */
     std::map<Node*, bool> setup_resource_block_lists_;

     /**
      * @brief Can be used to store the iterator to a contents list.
      */
     std::list<EntityTime>::iterator contents_iterator_;

     /**
      * @brief True if the entity should be deleted once it leaves a node.
      *
      * Used by e.g. logics to flag this entity for deletion once it
      * leaves a node.
      */
     bool delete_entity_;

     /**
      * @brief Flag used to indicate if current call originated
      * from the triggering of a block list.
      */
     bool block_list_call_;

     /**
      * @brief A container for all properties associated with this entity.
      */
     PropertyContainer *properties_;

     /**
      * @brief The shared ownership reference counter.
      */
     int refs_;
};

} // namespace xsim

#endif // ENTITY_H
