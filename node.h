#ifndef NODE_H
#define NODE_H

#include <xsim_config>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <functional>

#include "common.h"
#include "object.h"
#include "movestrategy.h"
#include "output.h"
#include "signal.hpp"

namespace xsim {

class ActivePeriod;
class ExitPort;
class EnterPort;
class EventDisruptionBegin;
class EventDisruptionEnd;
class EventRequestProcessingResources;
class EventRequestRepairResources;
class EventRequestSetupResources;
class EventProcessingResourceReady;
class EventRepairResourceReady;
class EventSetupResourceReady;
class Failure;
class LogicResource;
class Entity;
class ExitLogic;
class NodeResource;
class ResourceManager;
class Simulation;
class NodeSkill;
class NumberGenerator;
class Variant;

/**
 * @brief The base class for all nodes.
 */
class XSIM_EXPORT Node : public virtual Object
{
 public:
     enum State { UNPLANNED, PAUSED, FAILED, WAITING, WORKING, TRAVELLING, BLOCKED, SETUP, WAITING_FOR_RESOURCE };
     Signal<void (Node*, Node*, Entity*)> entity_entered;
     Signal<void (Node*, Node*, Entity*)> entity_exiting;
     Signal<void (Node*, Node*, Entity*)> entity_exited;
     Signal<void (Node*, Entity*)> entity_updated;
     Signal<void (Node*, State, State)> state_changed;
     Signal<void (Node*, Failure*)> disruption_began;
     Signal<void (Node*, Failure*)> disruption_ended;
     Signal<void (Node*)> unplanned_ended;
     Signal<void (Node*)> paused_ended;
     Signal<void (Node*, double, double, bool)> active_period_ended;

     /**
      * @brief Constructor.
      */
     Node();

     /**
      * @brief Destructor.
      */
     virtual ~Node() = 0;

     /**
      * @brief Initialize the node at the start of the simulation.
      */
     void simulation_init() override;

     /**
      * @brief Save outputs from this node, called at the end of the
      * simulation.
      */
     void finalize() override;

     /**
      * @brief Initialized the node, called at the beginning of the simulation.
      */
     void init() override;

     /**
      * @brief Reset any statistics that have been collected.
      */
     void reset_stats() override;

     /**
      * @brief Enter an entity to a node.
      *
      * @param  entity    The entity.
      * @param  departure The node the entity is leaving from.
      *
      * @returns True if it the entity was accepted, false if it was not.
      */
     virtual bool enter(Entity* entity, Node* departure);

     /**
      * @brief The node have entered the failed state.
      *
      * @param failure The failure that started the disruption.
      * @param visited All nodes that have been visited so far.
      * @param level How many levels (steps) this failure has propagated.
      * @param propagate_failure Option to prevent a failure from
      * propagating.
      */
     virtual void disruption_begin(Failure *failure,
         std::map<Node*, bool> &visited, int level, bool propagate_failure = true);

     /**
      * @brief The node have exited the failed state.
      *
      * @param failure The failure that ended the disruption.
      * @param visited All nodes that have been visited so far.
      * @param level How many levels (steps) this failure has propagated.
      */
     virtual void disruption_end(Failure *failure,
         std::map<Node*, bool> &visited, int level, bool propagate_failure = true);

     /**
      * @brief The node have entered the unplanned state.
      */
     virtual void unplanned_begin();

     /**
      * @brief The node have exited the unplanned state.
      */
     virtual void unplanned_end();

     /**
      * @brief The node have entered the paused state.
      */
     virtual void paused_begin();

     /**
      * @brief The node have entered the paused state.
      */
     virtual void paused_end();

     /**
      * @brief Get time this node have been empty.
      *
      * @return The empty time.
      */
     virtual double empty_time() const;

     /**
      * @brief Get, in percent, how much this node have been occupied.
      *
      * @return The occupation portion as a number between zero and one.
      */
     virtual double relative_occupation() const;

     /**
      * @brief Get the least amount of entities that have been present at
      * any given time on this node.
      *
      * @return The minimum occupation.
      */
     virtual size_t min_occupation() const;

     /**
      * @brief Get the most amount of entities that have been present at
      * any given time on this node.
      *
      * @return The maxiumum occupation.
      */
     virtual size_t max_occupation() const = 0;

      /**
      * @brief Get how many entities this node have.
      *
      * @return The number of entities.
      */
     virtual size_t content_size() const = 0;

     /**
      * @brief Get an optional text that can be used to describe, for example,
      * the current state of the node.
      *
      * @return A text describing the state of the node.
      */
     virtual std::string text() const;

     /**
      * @brief Get the time this node have been occupied.
      *
      * @return The time in seconds.
      */
     double occupied() const;

     /**
      * @brief Get the entity that is currently on this node.
      *
      * @return The entity, if there are none, a nullptr.
      */
     Entity* entity() const;

     /**
      * @brief Checks if this node is operational, i.e. its not unplanned,
      * paused or failed.
      *
      * @return True if operational.
      */
     bool is_operational() const;

     /**
     * @brief Get the state this node is in.
     *
     * @return The state.
     */
     State state() const;

     /**
      * @brief Get the state this node is in as a string.
      *
      * @return The state as a string.
      */
     std::string state_as_string() const;

     /**
      * @brief Returns if the node is in an active or inactive state.
      *
      * @return True if the node is an active state, false otherwise.
      */
     bool is_active() const;

     /**
      * @brief Check if node is waiting.
      *
      * @return True if node is waiting.
      */
     bool waiting() const;

     /**@brief Check if node is waiting for resource.
     *
     * @return True if node is waiting for resource.
     */
     bool waiting_for_resource() const;

     /**
      * @brief Check if node is working.
      *
      * @return True if node is working.
      */
     bool working() const;

     /**
      * @brief Check if node is travelling.
      *
      * @return True if node is travelling.
      */
     bool travelling() const;

     /**
      * @brief Check if node is blocking.
      *
      * @return True if node is blocking.
      */
     bool blocked() const;

     /**
      * @brief Check if node is setting up.
      *
      * @return True if node is setting up.
      */
     bool setup() const;

     /**
      * @brief Check if node is failed.
      *
      * @param state If false disregard state priorities and just
      * return if the node has any active failures or not.
      *
      * @return True if node is failed.
      */
     bool failed(bool state = true) const;

     /**
      * @brief Check if node is unplanned.
      *
      * @return True if node is unplanned.
      */
     bool unplanned() const;

     /**
      * @brief Check if node is paused.
      *
      * @return True if node is paused.
      */
     bool paused() const;

     /**
      * @brief Check if node is empty.
      *
      * @return True if node is empty.
      */
     bool empty() const;

     /**
      * @brief Convert a time to the portion of the total elapsed time.
      *
      * @param time The time to convert.
      *
      * @returns The portion of the total elapsed time.
      */
     double time_portion(double time) const;

     /**
      * @brief Get the waiting time.
      *
      * @return Waiting time.
      */
     simtime waiting_time() const;

     /**
      * @brief Get the waiting for resource time.
      *
      * @return Waiting for resource time.
      */
     simtime waiting_for_resource_time() const;

     /**
      * @brief Get the working time.
      *
      * @return Working time.
      */
     simtime working_time() const;

     /**
      * @brief Get the travelling time.
      *
      * @return Travelling time.
      */
     simtime travelling_time() const;

     /**
      * @brief Get the blocked time.
      *
      * @return Blocked time.
      */
     simtime blocked_time() const;

     /**
      * @brief Get the setup time.
      *
      * @return Setup time.
      */
     simtime setup_time() const;

     /**
      * @brief Get the failed time.
      *
      * @return Failed time.
      */
     simtime failed_time() const;

     /**
      * @brief Get the unplanned time.
      *
      * @return Unplanned time.
      */
     simtime unplanned_time() const;

     /**
      * @brief Get the paused time.
      *
      * @return Paused time.
      */
     simtime paused_time() const;

     /**
      * @brief Set the process time this node should use.
      *
      * @param process_time The new process time generator.
      */
     virtual void set_process_time_generator(NumberGenerator *process_time);

     /**
      * @brief Get the process time generator used by this node.
      *
      * @return The process time generator.
      */
     NumberGenerator* process_time_generator() const;

      /**
       * @brief Get the sole bottleneck time.
       *
       * Roser C. et al. (2002) Shifting Bottleneck Detection.
       *
       * @returns The sole bottleneck time.
       */
      double sole_bottleneck_time() const;

      /**
       * @brief Get shifting bottleneck time.
       *
       * Roser C. et al. (2002) Shifting Bottleneck Detection.
       *
       * @returns The shifting bottleneck time.
       */
      double shifting_bottleneck_time() const;

      /**
       * @brief Get the total bottleneck time.
       *
       * Roser C. et al. (2002) Shifting Bottleneck Detection.
       *
       * @returns The total bottleneck time.
       */
      double total_bottleneck_time() const;

     /**
      * @brief Add time that this node have been the sole bottleneck.
      *
      * @param amount The amount to add in seconds.
      */
     void add_sole_bottleneck(simtime amount);

     /**
      * @brief Add time that this node have shared the bottleneck.
      *
      * @param amount The amount to add in seconds.
      */
     void add_shifting_bottleneck(simtime amount);

     /**
      * @brief Set a temporary sole bottleneck time.
      *
      * This is used when calculating shifting bottlnecks during the
      * simulation. Since we can't know if the current active period is the
      * longest active period, since it haven't ended yet, this function is
      * used to temporarily store the sole time.
      *
      * @param amount The amount to set in seconds.
      */
     void set_tmp_sole_bottleneck(simtime amount);

     /**
      * @brief Set a temporary shifting bottleneck time.
      *
      * This is used when calculating shifting bottlnecks during the
      * simulation. Since we can't know if the current active period is the
      * longest active period, since it haven't ended yet, this function is
      * used to temporarily store the shifting time.
      *
      * @param amount The amount to set in seconds.
      */
     void set_tmp_shifting_bottleneck(simtime amount);

     /**
      * @brief Set the batch this node should be associated with.
      *
      * This used to limit which batches that can be processed in this node.
      *
      * @param batch_id The batch id.
      */
     void set_batch_id(unsigned int batch_id);

     /**
      * @brief Get the current batch id
      *
      * @return The batch id.
      */
     unsigned int batch_id() const;

     /**
      * @brief End the active period and record its length.
      *
      * @param open_end True if the active period can change at a later time.
      */
     void end_active_period(bool open_end = false);

     /**
      * @brief Connect the failure of this node to another node.
      *
      * @param node The node that should fail when this node fails.
      */
     void add_failure_node(Node *node);

     /**
      * @brief Add a resource manager to this node.
      *
      * @param node The resource manager.
      */
     void add_resource_manager(ResourceManager *rm);

     /**
      * @brief Set what skills are required for processing on this node.
      *
      * @param resource The NodeResource containing the required skills.
      */
     void set_processing_resource(NodeResource *resource);

     /**
      * @brief Set what skills are required for repair on this node.
      *
      * @param resource The NodeResource containing the required skills.
      */
     void set_repair_resource(NodeResource *resource);

     /**
      * @brief Set what skills are required for setup on this node.
      *
      * @param resource The NodeResource containing the required skills.
      */
     void set_setup_resource(NodeResource *resource);

     /**
      * @brief Getter for the NodeResource containing the skills required for processing on this node.
      *
      * @param  check_enabled (Optional) True to also check that the NodeResource is enabled.
      *
      * @returns The processing NodeResource.
      */
     NodeResource* processing_resource(bool check_enabled = true) const;

     /**
      * @brief Getter for the NodeResource containing the skills required for repair on this node.
      *
      * @param  check_enabled (Optional) True to also check that the NodeResource is enabled.
      *
      * @returns The repair NodeResource.
      */
     NodeResource* repair_resource(bool check_enabled = true) const;

     /**
      * @brief Getter for the NodeResource containing the skills required for setup on this node.
      *
      * @param  check_enabled (Optional) True to also check that the NodeResource is enabled.
      *
      * @returns The setup NodeResource.
      */
     NodeResource* setup_resource(bool check_enabled = true) const;

     /**
      * @brief Function that tells whether or not processing resources are
      * needed.
      *
      * @return True if NodeResource for processing is set (i.e. not 0) and
      * all required skills aren't allocated, otherwise false.
      */
     bool need_processing_resources() const;

     /**
      * @brief Function that tells whether or not repair resources are
      * needed.
      *
      * @return True if NodeResource for repair is set (i.e. not 0) and
      * all required skills aren't allocated, otherwise false.
      */
     bool need_repair_resources() const;

     /**
      * @brief Function that tells whether or not setup resources are
      * needed.
      *
      * @param entity The entity that is about to enter.
      *
      * @return True if NodeResource for setup is set (i.e. not 0) and
      * all required skills aren't allocated and the entity is different
      * than the last entity to enter the node, otherwise false.
      */
     virtual bool need_setup_resources(Entity *entity) const;

     /**
      * @brief Based on the execution factors of the required skills of
      * the allocated resources calculates an average execution factor
      * for processing.
      *
      * @return An average execution factor for the required and allocated
      * skills.
      */
     double get_execution_factor() const;

     /**
      * @brief Based on the execution factors of the required skills of
      * the allocated resources calculates an average execution factor
      * for repair.
      *
      * @return An average execution factor for the required and allocated
      * skills.
      */
     double get_repair_execution_factor() const;

     /**
      * @brief Based on the execution factors of the required skills of
      * the allocated resources calculates an average execution factor
      * for setup.
      *
      * @return An average execution factor for the required and allocated
      * skills.
      */
     double get_setup_execution_factor() const;

     /**
      * @brief Methods that override this should try to allocate
      * resources with the skills required for processing on this node.
      *
      * @param entity The entity that entered or is about to enter
      * (SkillsFirst==true) this node.
      * @param schedule_event_out If true an out event will be scheduled
      * once all required resources are allocated and ready. Used when
      * SkillsFirst is true.
      *
      * @return True if the allocation was successful.
      */
     virtual bool try_allocate_resources(Entity *entity, bool schedule_event_out);

     /**
      * @brief Methods that override this should try to allocate
      * resources with the skills required for repair on this node.
      *
      * @param failure The active failure.
      *
      * @return True if the allocation was successful.
      */
     virtual bool try_allocate_repair_resources(Failure *failure);

     /**
      * @brief Methods that override this should try to allocate
      * resources with the skills required for setup on this node.
      *
      * @param entity The entity that entered or is about to enter
      * (SkillsFirst==true) this node.
      * @param schedule_event_out If true an out event will be scheduled
      * once all required resources are allocated and ready. Used when
      * SkillsFirst is true.
      *
      * @return True if the allocation was successful.
      */
     virtual bool try_allocate_setup_resources(Entity *entity, bool schedule_event_out);

     /**
      * @brief Override this method to implement how to interrupt an
      * ongoing processing that uses resources.
      */
     virtual void interrupt_processing_resource();

     /**
      * @brief Override this method to implement how to resume an
      * interrupted processing that require resources.
      */
     virtual void resume_processing_resource(bool add_elapsed_time = true);

     /**
      * @brief Override this method to implement how to interrupt an
      * ongoing repair that uses resources.
      */
     virtual void interrupt_repair_resource();

     /**
      * @brief Override this method to implement how to resume an
      * interrupted repair that require resources.
      */
     virtual void resume_repair_resource(bool add_elapsed_time = true);

     /**
      * @brief Override this method to implement how to interrupt an
      * ongoing setup that uses resources.
      */
     virtual void interrupt_setup_resource();

     /**
      * @brief Override this method to implement how to resume an
      * interrupted setup that require resources.
      */
     virtual void resume_setup_resource(Entity *entity, bool add_elapsed_time = true);

     /**
      * @brief Getter stating whether or not the processing on this
      * node is interrupted.
      *
      * @return True if processing that uses resources on this node
      * is interrupted, otherwise false.
      */
     bool processing_interrupted() const;

     /**
      * @brief Getter stating whether or not the repair on this
      * node is interrupted.
      *
      * @return True if repair that uses resources on this node
      * is interrupted, otherwise false.
      */
     bool repair_interrupted() const;

     /**
      * @brief Getter stating whether or not the setup on this
      * node is interrupted.
      *
      * @return True if setup that uses resources on this node
      * is interrupted, otherwise false.
      */
     bool setup_interrupted() const;

     /**
      * @brief Getter for the time when processing was interrupted.
      *
      * @return The time when the processing was interrupted.
      */
     simtime start_interrupted_processing() const;

     /**
      * @brief Getter for the time when repair was interrupted.
      *
      * @return The time when the repair was interrupted.
      */
     simtime start_interrupted_repair() const;

     /**
      * @brief Getter for the time when setup was interrupted.
      *
      * @return The time when the setup was interrupted.
      */
     simtime start_interrupted_setup() const;

     /**
      * @brief Adds a NodeResource to the block lists of all
      * connected ResourceManagers.
      *
      * @param resource The NodeResource to add to block lists.
      * @param node The node of associated with the NodeResource.
      * @param entity The entity associated with the NodeResource,
      * 0 for NodeResource used for repair.
      * @param failure The failure associated with the NodeResource,
      * 0 for NodeResource not used for repair.
      * @param The type of NodeResource, e.g. Processing, Repair, Setup.
      */
     void add_to_resource_block_lists(NodeResource *resource, Node *node, Entity *entity, Failure *failure, ResourceType type);

     /**
      * @brief Remove a NodeResource from the block lists of all
      * connected ResourceManagers.
      *
      * @param resource The NodeResource to remove from the block lists.
      * @param entity The entity associated with the NodeResource,
      * 0 for NodeResource used for repair.
      */
     void remove_from_resource_block_lists(NodeResource *resource, Entity *entity);

     /**
      * @brief Addition of processing resource request event to member
      * variable used for internal record keeping.
      *
      * @param evt The event to keep track of.
      */
     void add_request_resources_event(EventRequestProcessingResources *evt);
     
     /**
      * @brief Removes request reosurce event form internal list. Used when
      * the event is processed and Simulation handles deletion of
      * the event and also when events are freed by a entity
      * and its events reference list.
      *
      * @param evt  Event to remove.
      */
     void remove_request_resources_event(EventRequestProcessingResources *evt);

     /**
      * @brief Removes and deletes any lingering request resource events.
      * Used when request is satisfied, i.e. all needed skills
      * are allocated and ready, and addiontal requests aren't
      * needed.
      *
      * References to these events are also stored on entities
      * and any freed event is also removed from list on
      * the entity.
      *
      * @param entity Pointer to entity who was assigned
      * resources.
      * @param sim Pointer to simulation from which the events should be
      * removed.
      */
     void remove_request_resources_events(Entity *entity);
	 
     /**
      * @brief Check if this node got a request processing resource event for
      * the provided entity.
      *
      * @parameter entity The entity to match against a request resource
      * event.
      */
     bool has_request_processing_resource_event(Entity *entity);

     /**
      * @brief Addition of repair resource request event to member
      * variable used for internal record keeping.
      *
      * @param evt The event to keep track of.
      */
     void add_request_resources_event(EventRequestRepairResources *evt);

     /**
      * @brief Removes request reosurce event form internal list. Used when
      * the event is processed and Simulation handles deletion of
      * the event and also when events are freed by a entity
      * and its events reference list.
      *
      * @param evt  Event to remove.
      */
     void remove_request_resources_event(EventRequestRepairResources *evt);

     /**
      * @brief Removes and deletes any lingering request resource events.
      * Used when request is satisfied, i.e. all needed skills
      * are allocated and ready, and addiontal requests aren't
      * needed.
      *
      * References to these events are also stored on entities
      * and any freed event is also removed from list on
      * the entity.
      *
      * @param entity Pointer to entity who was assigned
      * resources.
      * @param sim Pointer to simulation from which the events should be
      * removed.
      */
     void remove_request_repair_resources_events();

     /**
      * @brief Addition of setup resource request event to member
      * variable used for internal record keeping.
      *
      * @param evt The event to keep track of.
      */
     void add_request_resources_event(EventRequestSetupResources *evt);

     /**
      * @brief Removes request reosurce event form internal list. Used when
      * the event is processed and Simulation handles deletion of
      * the event and also when events are freed by a entity
      * and its events reference list.
      *
      * @param evt  Event to remove.
      */
     void remove_request_resources_event(EventRequestSetupResources *evt);
     
     /**
      * @brief Removes and deletes any lingering request resource events.
      * Used when request is satisfied, i.e. all needed skills
      * are allocated and ready, and addiontal requests aren't
      * needed.
      *
      * References to these events are also stored on entities
      * and any freed event is also removed from list on
      * the entity.
      *
      * @param entity Pointer to entity who was assigned
      * resources.
      * @param sim Pointer to simulation from which the events should be
      * removed.
      */
     void remove_request_setup_resources_events(Entity *entity);

     /**
      * @brief Check if this node got a request setup resource event for
      * the provided entity.
      *
      * @parameter entity The entity to match against a request resource
      * event.
      */
     bool has_request_setup_resource_event(Entity *entity);

     /**
      * @brief Undo any setup resource request or resource ready event
      * connected to the provided entity. Also, if there aren't any
      * outstanding setup resource events on this node make sure any
      * allocated resource is released.
      *
      * @parameter entity The entity whos resource events should be
      * undone.
      */
     void undo_request_setup_resource(Entity *entity);

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
      * the event and also when events are freed by a entity
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
      * References to these events are also stored on entites
      * and any freed event is also removed from list on
      * the entity.
      *
      * @param entity Pointer to entity who was assigned
      * resources.
      * @param sim Pointer to simulation from which the events should be
      * removed.
      */
     void remove_processing_resource_ready_events(Entity *entity);
     
     /**
      * @brief Undo any processing resource request or resource ready event
      * connected to the provided entity. Also, if there aren't any
      * outstanding processing resource events on this node make sure
      * any allocate resource is released.
      *
      * @parameter entity The entity whos resource events should be
      * undone.
      */
     void undo_request_processing_resource(Entity *entity);

     /**
      * @brief Control triggered by rear exit of an entity from a node that has at least one
      *        successor that uses resources its setting skills first. Unless the entered node
      *        require resources, any made resource request for this entity will be undone.
      *        
      *        Required since resources are requested on call to is_open, while there is no guarantee
      *        that the entity will enter the node that requested resources. Especially if any of the
      *        required resources has a response time larger than 0 and a parallel node doesn't
      *        require resources.
      *
      * @param  departure   The node the entity exited.
      * @param  destination The node the entity entered.
      * @param  entity      Pointer to entity that just exited.
      */
     void resource_undo_control(Node* departure, Node* destination, Entity* entity);

     /**
      * @brief Addition of repair resource ready event to member
      * variable used for internal record keeping.
      *
      * @param evt The event to keep track of.
      */
     void add_repair_resource_ready_event(EventRepairResourceReady *evt);

     /**
      * @brief Removes request reosurce event form internal list. Used when
      * the event is processed and Simulation handles deletion of
      * the event and also when events are freed by a entity
      * and its events reference list.
      *
      * @param evt  Event to remove.
      */
     void remove_repair_resource_ready_event(EventRepairResourceReady *evt);

     /**
      * @brief Removes and deletes any lingering request resource events.
      * Used when request is satisfied, i.e. all needed skills
      * quests aren't
      * needed.
      *
      * References to these events are also stored on entities
      * and any freed event is also removed from list on
      * the entity.
      *
      * @param entity Pointer to entity who was assigned
      * resources.
      * @param sim Pointer to simulation from which the events should be
      * removed.
      */
     void remove_repair_resource_ready_events();

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
      * the event and also when events are freed by a entity
      * and its events reference list.
      *
      * @param evt  Event to remove.
      */
     void remove_setup_resource_ready_event(EventSetupResourceReady *evt);

     /**
      * @brief Removes and deletes any lingering request resource events.
      * Used when request is satisfied, i.e. all needed skills
      * quests aren't
      * needed.
      *
      * References to these events are also stored on entities
      * and any freed event is also removed from list on
      * the entity.
      *
      * @param entity Pointer to entity who was assigned
      * resources.
      * @param sim Pointer to simulation from which the events should be
      * removed.
      */
     void remove_setup_resource_ready_events(Entity *entity);

	 /**
	  * @brief Setter for helper state waiting for processing
	  *	resource.
	  *
	  * @param value Value to set.
	  */
	 void set_waiting_for_processing_resource(bool value);

	 /**
	  * @brief Setter for helper state waiting for repair
	  *	resource.
	  *
	  * @param value Value to set.
	  */
	 void set_waiting_for_repair_resource(bool value);

	 /**
	  * @brief Setter for helper state waiting for setup
	  *	resource.
	  *
	  * @param value Value to set.
	  */
	 void set_waiting_for_setup_resource(bool value);

     /**
      * @brief Tells whether or not skills first is active by
      * any NodeResource of this Node.
      *
      * @return True if any NodeResource on this Node has SkillsFirst set to
      * true, otherwise false.
      */
     bool skills_first_used();

     /**
      * @brief Add a failure to this node.
      *
      * @param failure The failure that this node should use.
      */
     virtual void add_failure(Failure *failure);

     /**
      * @brief Get the failure of this node.
      *
      * @return failure The failure that this node uses.
      */
     Failure* first_failure() const;

     /**
      * @brief Get all failures of this node.
      *
      * @return The all failures that this node uses.
      */
     std::list<Failure*> failures() const;

     /**
      * @brief Add time that is used to calculate the next failure should occur.
      *
      * @param time The amount to add.
      */
     void add_failure_processing(simtime time);

      /**
      * @brief Set all stochastic failures to inifinity.
      */
     void clear_stochastic_failures();

     /**
      * @brief Mark this node as reserved. Can be used to indicate that this
      * node is reserved.
      *
      * @param value True if this is node should be reserved.
      */
     void set_reserved(bool value);

     /**
      * @brief Get if this node is marked as reserved.
      *
      * @return value True if this is node is reserved.
      */
     bool reserved() const;

     /**
      * @brief Toggle the failed state.
      *
      * @param value True if the node is failed.
      */
     void set_failed(bool value);

     /**
      * @brief Toggle the unplanned state.
      *
      * @param node The node where the entity is located.
      * @param entity The entity that is updated.
      * @param value True if the node is unplanned.
      */
     void set_unplanned(bool value);

     /**
      * @brief Set the state to waiting.
      */
     void set_waiting();

     /**
      * @brief Set the state to working.
      */
     void set_working();

     /**
      * @brief Set the state to travelling.
      */
     void set_travelling();

     /**
      * @brief Set the state to blocked.
      */
     void set_blocked();

     /**
      * @brief Set the state to setup.
      */
     void set_setup();


     /**
      * @brief Toggle the paused state.
      *
      * @param value True if the node is paused.
      */
     void set_paused(bool value);

     /**
      * @brief Perform actions when a entity leaves the node.
      *
      * @param entity The entity.
      * @param node The node that should be the destination of the
      * entity. If it is zero all successors are all eligible.
      */
     virtual void leave(Entity *entity, Node *node) {}

     /**
      * @brief Perform actions when a setup is finished.
      */
     virtual void setup_end() {}

     /**
      * @brief Set the setup time generator that should be used by this node.
      *
      * @param number_generator The setup time generator.
      */
     virtual void set_setup_time_generator(NumberGenerator *number_generator);

     /**
      * @brief Get the setup time generator.
      *
      * @return The setup time generator.
      */
     NumberGenerator* setup_time_generator() const;

     /**
      * @brief Get the setup time from one entity to another.
      *
      * @param to The new entity.
      * @param from The old entity.
      *
      * @return The setup time.
      */
     double next_setup_time(Entity *to, Entity *from);

     /**
      * @brief Check if there is a setup number generator set for this node.
      *
      * @return True if this node can do setups.
      */
     bool have_setup() const;

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
      * @brief Try to allocate resources for processing from
      * connected ResourceManagers.
      *
      * @param entity The entity that is about to be processed.
      *
      * @return True if all required resources where allocated,
      * otherwise false.
      */
     bool allocate_processing_resource(Entity *entity);

     /**
      * @brief Try to allocate resources for repair from
      * connected ResourceManagers.
      *
      * @param failre The active failure.
      *
      * @return True if all required resources where allocated,
      * otherwise false.
      */
     bool allocate_repair_resource(Failure *failure);
     
     /**
      * @brief Try to allocate resources for setup from
      * connected ResourceManagers.
      *
      * @param entity The entity that the Node is about to be setup for.
      *
      * @return True if all required resources where allocated,
      * otherwise false.
      */
     bool allocate_setup_resource(Entity *entity);

     /**
      * @brief After processing is done, return allocated resources for
      * processing to their ResourceMangers. The return of a resource
      * will trigger the block list of the ResourceManager.
      *
      * @param entity The entity that was processed.
      */
     void release_processing_resources(Entity *entity);

     /**
      * @brief After repair is done, return allocated resources for
      * repair to their ResourceMangers. The return of a resource
      * will trigger the block list of the ResourceManager.
      */
     void release_repair_resources();

     /**
      * @brief After setup is done, return allocated resources for
      * setup to their ResourceMangers. The return of a resource
      * will trigger the block list of the ResourceManager.
      *
      * @param entity The entity that the Node was setup for.
      */
     void release_setup_resources(Entity *entity);

	 /**
	  * @brief Trigger blocklists of available resource
	  *	managers.
	  */
	 void trigger_blocklists_of_resource_managers() const;

     /**
      * @brief Check if the entity is allowed to leave this node.
      *
      * @param entity The entity that is about the leave.
      *
      * @return True if it is allowed to leave.
      */
     bool allow_leaving(Entity *entity);

     /**
      * @brief Set the departure of this node.
      *
      * @param  exit_port The exit port that should be set.
      */
     void set_exit_port(ExitPort* exit_port);

     /**
      * @brief Returns the node exit port.
      *
      * @returns The exit port.
      */
     ExitPort* exit_port() const;

     /**
      * @brief Set the enter port of this node.
      *
      * @param  enter_port The enter port that should be set.
      */
     void set_enter_port(EnterPort* enter_port);

     /**
      * @brief Returns the node enter port.
      *
      * @returns The enter port.
      */
     EnterPort* enter_port() const;

 protected:

     /**
      * @brief Define working stats.
      */
     void define_working_output();

     /**
      * @brief Define waiting stats.
      */
     void define_waiting_output();

     /**
      * @brief Define blocked stats.
      */
     void define_blocked_output();

     /**
      * @brief Define failed stats.
      */
     void define_failed_output();

     /**
      * @brief Define unplanned stats.
      */
     void define_unplanned_output();

     /**
      * @brief Define break stats.
      */
     void define_break_output();

     /**
      * @brief Define setup stats.
      */
     void define_setup_output();

     /**
      * @brief Define waiting for resource stats.
      */
     void define_waiting_for_resource_output();

     /**
      * @brief Define travelling stats.
      */
     void define_travelling_output();

     /**
      * @brief Define empty stats.
      */
     void define_empty_output();

     /**
      * @brief Define occupation stats.
      */
     void define_occupation_output();

     /**
      * @brief Define part entries stats.
      */
     void define_part_entries_output();

     /**
      * @brief Define part exits stats.
      */
     void define_part_exits_output();

     /**
      * @brief Define shifting bottleneck output.
      */
      void define_shifting_bottleneck_outputs();

     /**
      * @brief Set the entity that is currently located on this node.
      *
      * @param entity The entity.
      */
     void set_entity(Entity *entity);

     /**
      * @brief Update state waiting for resource based on
	  *	the helper states, waiting_for_processing_resource_,
	  *	waiting_for_repair_resource_ and
	  *	waiting_for_setup_resoruce_.
      */
     void update_waiting_for_resource();

     /**
      * @brief Add time that this node have been occupied.
      *
      * @param scale Which scale to apply to the time.
      * @param time The start time of the occupation.
      */
     void add_occupied_time(double scale, simtime time);

     /**
      * @brief Set when this node becomes empty.
      */
     void set_start_empty();

     /**
      * @brief Set when this node goes from empty to not empty.
      */
     void set_end_empty();

     /**
      * @brief Get the time this node was last operational.
      *
      * @return The time the node was last operational.
      */
     simtime end_operational() const;

 private:
     /**
      * @brief Add the time this node have been in the current state. Must be
      * called before changing the state.
      */
     void observe_state();

     /**
      * @brief When shifting bottleneck detection is on, either start or end an
      * active period depending on the state the node is in. Must be called
      * after the state is changed.
      */
     void observe_active_period();

     /**
      * @brief Interupt the empty state.
      *
      * In some states the node should not be counted as empty, even though it
      * is technically empty.
      */
     void interupt_empty();

     /**
      * @brief The time this node was last empty.
      */
     simtime last_empty_;

     /**
      * @brief How much time this node have been occupied.
      */
     simtime occupied_;

     /**
      * @brief The entity that is currently located on this node. Zero if
      * this node is empty.
      */
     Entity *entity_;

     /**
      * @brief Shifting bottleneck output.
      */
     ShiftingBottleneck bottleneck_;

     /**
      * @brief Temporary shifting bottleneck output, used for online shifting
      * bottleneck detection.
      */
     ShiftingBottleneck tmp_bottleneck_;

     /**
      * @brief Which, if any, batch that should be processed in this node.
      */
     unsigned int batch_id_;

     /**
      * @brief The time when state last changed.
      */
     simtime start_period_;

     /**
      * @brief A reference counter on how many failures that are active.
      */
     int disruption_refs_;

     /**
      * @brief A reference counter on how many unplanned periods that are
      * active.
      */
     int unplanned_refs_;

     /**
      * @brief State variables.
      */
     bool waiting_;
     bool waiting_for_resource_;
     bool working_;
     bool travelling_;
     bool blocked_;
     bool setup_;
     bool failed_;
     bool unplanned_;
     bool paused_;
     bool empty_;
	 /**
	  * @brief Helper states for waiting for resource.
	  */
	 bool waiting_for_processing_resource_;
	 bool waiting_for_repair_resource_;
	 bool waiting_for_setup_resource_;

     /**
      * @brief The amount of time spent in respective state.
      */
     simtime working_time_;
     simtime travelling_time_;
     simtime waiting_time_;
     simtime waiting_for_resource_time_;
     simtime blocked_time_;
     simtime setup_time_;
     simtime failed_time_;
     simtime unplanned_time_;
     simtime paused_time_;
     simtime empty_time_;

     /**
      * @brief True when the node is in an active state, only used when
      * shifting bottleneck detection is on.
      */
     bool active_;

     /**
      * @brief The start time of the current active period.
      */
     simtime start_active_period_;

     /**
      * @brief The simulation time when this node was last operational.
      */
     simtime end_operational_;

     /**
      * @brief Nodes that should fail when this node fails.
      */
     std::vector<Node*> failure_nodes_;

     /**
      * @brief The object that schedules disruption events.
      */
     std::list<Failure*> failures_;

     /**
      * @brief Available resource managers.
      */
     std::vector<ResourceManager*> resource_managers_;

     /**
      * @brief Map used to keep track of which of the NodeResource
      * on this node are added to block lists of connected ResourceManagers.
      */
     std::map<std::pair<NodeResource*,Entity*>, bool> on_resource_block_lists_;
    
     /**
      * @brief If set this specifies the skills needed for processing
      * on this node.
      */
     NodeResource *processing_resource_;

     /**
      * @brief If set this specifies the skills needed for repair
      * on this node.
      */
     NodeResource *repair_resource_;

     /**
      * @brief If set this specifies the skills needed for setup
      * on this node.
      */
     NodeResource *setup_resource_;

     /**
      * @brief Map use to keep track of all outstanding processing resource
      * request events associated with this Node.
      */
     std::map<EventRequestProcessingResources*, bool> request_resources_events_;

     /**
      * @brief Map use to keep track of all outstanding repair resource
      * request events associated with this Node.
      */
     std::map<EventRequestRepairResources*, bool> request_repair_resources_events_;

     /**
      * @brief Map use to keep track of all outstanding setup resource
      * request events associated with this Node.
      */
     std::map<EventRequestSetupResources*, bool> request_setup_resources_events_;

     /**
      * @brief Map use to keep track of all outstanding processing resource
      * ready events associated with this Node.
      */
     std::map<EventProcessingResourceReady*, bool> processing_resource_ready_events_;

     /**
      * @brief Map use to keep track of all outstanding repair resource
      * ready events associated with this Node.
      */
     std::map<EventRepairResourceReady*, bool> repair_resource_ready_events_;

     /**
      * @brief Map use to keep track of all outstanding setup resource
      * ready events associated with this Node.
      */
     std::map<EventSetupResourceReady*, bool> setup_resource_ready_events_;

     /**
      * @brief Time when processing was interrupted.
      */
     simtime start_interrupted_processing_;

     /**
      * @brief Time when repair was interrupted.
      */
     simtime start_interrupted_repair_;

     /**
      * @brief Time when setup was interrupted.
      */
     simtime start_interrupted_setup_;

     /**
      * @brief If true processing is interrupted.
      */
     bool processing_interrupted_;

     /**
      * @brief If true repair is interrupted.
      */
     bool repair_interrupted_;

     /**
      * @brief If true setup is interrupted.
      */
     bool setup_interrupted_;

     /**
      * @brief If true this node is reserved.
      */
     bool reserved_;
     //std::vector<Resource*> processing_resources_;

     /**
      * @brief The process time generator for out events.
      */
     NumberGenerator *process_time_generator_;

     /**
      * @brief The setup number generator.
      */
     NumberGenerator *setup_time_generator_;

     /**
      * @brief The exit port of this node.
      */
     ExitPort *exit_port_;

     /**
      * @brief The enter port of this node.
      */
     EnterPort *enter_port_;
};

} // namespace xsim

#endif // NODE_H
