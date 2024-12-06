#ifndef PARALLELOPERATION_H
#define PARALLELOPERATION_H

#include <xsim_config>
#include <list>
#include <string>
#include <map>

#include "node.h"
#include "movestrategy.h"
#include "exitport.h"
#include "int.h"

namespace xsim {

class ExitLogic;
class EventOut;
class EventEntryTimeout;
class Failure;
class Entity;
class NumberGenerator;
class Operation;
class ParallelOperationExitLogic;
class ParallelOperationOperation;

/**
 * @brief A node that can process one or more entities. The entities
 * can leave in any order.
 */
class XSIM_EXPORT ParallelOperation : public Node {
 public:
     typedef std::map<Entity*, Node*> ContentTracker;

     /**
      * @brief Constructor.
      */
     ParallelOperation();

     /**
      * @brief Destructor.
      */
     virtual ~ParallelOperation();

     void simulation_init() override;
     void init() override;
     void finalize() override;
     void reset_stats() override;

     /* Documented in node.h */
     void define_outputs() override;
     void set_outputs() override;
     void disruption_begin(Failure *failure,
            std::map<Node*, bool> &visited,
            int level,
            bool propagate_failure = true) override;
     void disruption_end(Failure *failure,
            std::map<Node*, bool> &visited,
            int level,
            bool propagate_failure = true) override;
     void unplanned_begin() override;
     void unplanned_end() override;
     void paused_begin() override;
     void paused_end() override;
     void interrupt_processing_resource() override;
     void resume_processing_resource(bool add_elapsed_time = true) override;
     void interrupt_repair_resource() override;
     void resume_repair_resource(bool add_elapsed_time = true) override;
     void interrupt_setup_resource() override;
     void resume_setup_resource(Entity *entity, bool add_elapsed_time = true) override;
     bool is_open(Entity *entity, bool ignore_full) override;
     double relative_occupation() const override;
     size_t max_occupation() const override;
     size_t min_occupation() const override;
     size_t content_size() const override;
     std::string text() const override;
     void set_process_time_generator(NumberGenerator* number_generator) override;
     void set_setup_time_generator(NumberGenerator* number_generator) override;

     /** @brief Prepares for simulation by creating the operations. */
     virtual void pre_simulation_init();

     virtual bool allow(Node *node, Entity *entity);

     /**
      * @param  value If false parallel processing is only allowed for parts of
      * the same variant that enter at the same time.
      */
     void set_mixed_processing(bool value);

     /**
      * @returns True if mixed processing is used.
      */
     bool mixed_processing() const;

     /**
      * @param  value If true parts are only allowed to enter during
      * entry_timeout after the first part entered.
      */
     void set_synchronize_entries(bool value);

     /**
      * @returns True if synchronize entries is used.
      */
     bool synchronize_entries() const;

     /**
      * @param value If true exits from the parallel operation are synchronized.
      */
     void set_synchronize_exits(bool value);

     /**
      * @returns True if synchronize exits is used.
      */
     bool synchronize_exits() const;

     /**
      * @param  value True to share failures between internal operations.
      */
     void set_failure_zone(bool value);

     /**
      * @returns True if failure zone is used.
      */
     bool failure_zone() const;

     /**
     * @brief Add a failure to this node and duplicate it per operation.
     *
     * @param failure The failure that this node should use.
     */
      void add_failure(Failure *failure) override;

     /**
     * @brief Add an exit logic that can block an exiting entity
     * to all internal operations.
     *
     * @param logic The logic.
     */
     virtual void add_exit_logic(ExitLogic *logic);

     /**
      * @brief Send the entity to its designated internal operation.
      *
      * @param  entity    The entity.
      * @param  departure The node that the entity is leaving from.
      */
     bool enter(Entity *entity, Node* departure) override;

     /**
     * @brief Does nothing for this object since it is
     * handled by the leave of the internal operations.
     * However, being a abstract function it still need
     * to be inclued.
     */
     void leave(Entity *entity, Node *node) override;

     /**
      * @brief Called when an entity is leaving the parallel operation.
      *
      * @param entity The entity that is leaving.
      * @param operation The operation the entity is leaving from.
      * @param destination The node the entity is entering into.
      */
     virtual void leave(Entity* entity, ParallelOperationOperation* operation, Node* destination);

     /**
      * @brief Sets number parallel operations
      *
      * @param  num_operations Number of parallel operations.
      */
     void set_num_operations(Int num_operations);

     /**
      * @brief Get the number of parallel operations on this parallel
      * operation.
      *
      * @return The number of parallel operations.
      */
     Int num_operations() const;

     /**
      * @brief Get all internal operations of this parallel operation.
      *
      * @return All internal operations of this parallel operation.
      */
     std::list<ParallelOperationOperation*> operations() const;

     /**
      * @brief Get a all entities that are currently located on this
      * parallel operation, along with the simulation time on which they can
      * leave.
      *
      * @return All entities on this parallel operation.
      */
     std::list<Entity*> contents() const;

     /**
      * @brief Check if the parallel operation is full.
      *
      * @return True if it is full.
      */
     bool is_full() const;

     /**
     * @brief Try moving queued parts to their reserved operations.
     *
     * @param start_cycle If true it's allowed to start a new cycle.
     *
     * @return True if something was dequeued.
     */
     bool try_move_queued(bool start_cycle);

     /**
      * @brief Getter for entry timeout number generator.
      *
      * @return Pointer to the number generator.
      */
     NumberGenerator* entry_timeout() const;

     /**
      * @brief Setter for entry timeout number generator.
      *
      * @param number_generator The number generator to set.
      */
     void set_entry_timeout(NumberGenerator *number_generator);

     /**
     * @brief Check forward blocking considering the specific settings
     * of the parallel operation.
     *
     * @return True if check was performed.
     */
     bool try_check_forward_blocking();

     /**
     * @brief Try to schedule out events for parts possibly
     * prohibited from leaving due to synchronized exits.
     *
     * @return True if out events are/were scheduled.
     */
     bool try_schedule_out_events();

     /**
      * @brief Gets the average of a specific output from all operations.
      *
      * @param name The name of the output to get the average of.
      *
      * @returns The average value.
      */
     double average_output(const std::string &name) const;

     /**
      * @brief Gets the average entry timeout.
      *
      * @returns The average entry timeout.
      */
     double average_entry_timeout() const;

 private:
     /**
      * @brief All entities that currently are located on this node.
      */
     std::list<Entity*> content_;

     /**
      * @brief The maximum capacity of entities on this node.
      */
     Int num_ops_;

     /**
     * @brief If false only entities of the same variant are
     * allowed to be processed simultaneously. In addition, they
     * need to enter at the same time and new entries aren't allowed
     * until all currently processed parts have exited.
     */
     bool mixed_processing_;

     /**
     * @brief When true entries to the Parallel operation are
     * only accepted during a certain time after the entry of
     * the first part. The processing of parts will commence
     * as soon as the entry_waiting_time_ is reached or as
     * soon as the Parallel operation is full.
     */
     bool synchronize_entries_;

     /**
     * @brief Used when entries to the Parallel operation are
     * synchronized, the processing of the entered parts will
     * commence once this time is reached or as soon as
     * the Parallel operation is full.
     */
     NumberGenerator* entry_timeout_;

     /**
     * @brief Pointer to entry timeout event, used to possibly cancel
     * entry timeout event early.
     */
     EventEntryTimeout* event_entry_timeout_;

     /**
     * @brief List of queued parts that hasn't entered their
     * reserved operations due to the synchronization of entries.
     */
     std::vector<Entity*> queued_;

     /**
     * @brief True if synchronize_entries_ is active and a cycle is
     * active.
     */
     bool cycle_active_;

     /**
     * @brief True if synchronize_exits is active and all entities
     * scheduled to leave haven't left yet.
     * 
     */
     bool synchronized_exits_in_progress_;

     /**
     * @brief The number of full cycles, i.e. cycles without
     * empty operations, when entries are synchronized.
     */
     unsigned int num_full_cycles_;

     /**
     * @brief The number of cycles when entries are synchronized.
     */
     unsigned int num_cycles_;

     /**
     * @brief Total entry timeout time.
     */
     simtime total_entry_timeout_;

     /**
     * @brief Time that the latest event entry timeout was created.
     */
     simtime entry_timeout_creation_time_;

     /**
     * @brief Event entry time out counter, i.e. the number of
     * entry timeouts performed.
     */
     unsigned int entry_timeout_counter_;

     /**
     * @brief Log entry timeout time.
     */
     void log_entry_timeout();

     /**
     * @brief If true failures on the internal operations are
     * propagated to the other internal operations, i.e. if one
     * goes down all goes down.
     */
     bool failure_zone_;

     /**
     * @brief When true exits from the Parallel operation are
     * scheduled once all parallely processed parts are finished
     * and all operations are operational.
     */
     bool synchronize_exits_;

     /**
     * @brief Pointer to synchronized exit logic used for ensuring
     * parts exit at the same time when mixed_processing_ is deactivated.
     */
     ParallelOperationExitLogic* synchronized_exit_logic_;

     /**
     * @breif Time of the last entry made to parallel operation.
     */
     simtime time_of_last_entry_;

     /**
      * @brief The maximum number of simultaneous entities on this
      * parallel operation.
      */
     size_t max_occupied_;

     /**
      * @brief The minimum number of simultaneous entities on this parallel operation.
      */
     size_t min_occupied_;

     /**
      * @brief The simulation time when stats were last saved.
      */
     simtime enter_time_;

     /**
     * @brief Dictionary mapping entities to internal operations, i.e. used to
     * assign entities to internal operations in is_open and they are removed
     * in the exited observer.
     */
     ContentTracker mus_;

     /**
     * @brief List of internal operations.
     */
     std::list<ParallelOperationOperation*> operations_;

     /**
     * @brief A reference counter on how many internal operations that are
     * failed.
     */
     int failed_refs_;

     /**
     * @brief A reference counter on how many internal operations that are
     * waiting.
     */
     unsigned int waiting_refs_;

     /**
     * @brief A reference counter on how many internal operations that are
     * working.
     */
     unsigned int working_refs_;

     /**
     * @brief A reference counter on how many internal operations that are
     * blocked.
     */
     unsigned int blocked_refs_;

     /**
     * @brief A reference counter on how many internal operations that are
     * setup.
     */
     unsigned int setup_refs_;

     /**
     * @brief Observer of state changes for internal operations.
     *
     * @param node The node who's state changed.
     * @param state The state that changed.
     * @param previous_state The active state before this change.
     */
     void state_observer(Node* node, Node::State state, Node::State previous_state);

     /**
     * @brief Used to count down the state reference counter of the
     * previous state.
     *
     * @param previous_state The previously active state.
     */
     void observe_previous_state(State previous_state);

     /**
     * @brief Used to determine what state the parallel operation
     * should be in when the state of the internal operations
     * fall outside the following definition:
     *
     * Unplanned - All internal need to be unplanned (allways true)
     * Paused - All internal need to be unplanned (allways true)
     * Failed - All internal need to be failed (NOT allways true)'
     * Waiting - At least one is waiting'
     * Working - All internal need to be working'
     * Blocked - All internal need to be blocked'
     * Setup - All internal need to be setup'
     *
     * ' = If a state change of the internal operations will not
     * fulfill these criterias, the state will be determined by
     * the priority made in this function.
     */
     void set_state();

     /**
      * @brief Observer of disruption ended on internal operations. Used to trigger blocked entities.
      *
      * @param  node    The node who's disruption ended.
      * @param  failure The failure that ended.
      */
     void disruption_end_observer(Node* node, Failure* failure);

     /**
      * @brief Observer of exiting signals on internal operations. Used to forward exiting signals of
      *        interval operations to the subscribers of the parallel operation.
      *
      * @param  departure   The node the entity exited.
      * @param  destination The node the entity entered.
      * @param  entity      The entity that want to exit.
      */
     void exiting_observer(Node *departure, Node* destination, Entity* entity);

     /**
      * @brief Observer of exited signals on internal operations. Used to forward exited signals of
      *        interval operations to the subscribers of the parallel operation.
      *
      * @param  departure   The node the entity exited.
      * @param  destination The node the entity entered.
      * @param  entity      The entity that exited.
      */
     void exited_observer(Node* departure, Node* destination, Entity* entity);

     /**
     * @brief Observer for entity updates. Used to
     * forward entity updates of interal operations to the
     * subscribers of the parallel operation.
     *
     * @param node The node on which the entity is located.
     * @param entity The entity that was updated.
     */
     void entity_update_observer(Node* node, Entity* entity);

     /**
     * @brief Return if variant is allowed to enter considering
     * with regards to whether mixed_processing_ is allowed or not.
     *
     * @param entity The variant.
     *
     * @return If the variant is allowed to enter or not.
     */
     bool entry_allowed(Entity* entity);

     /**
     * @brief Get the operation reserved for a specific variant.
     *
     * @param entity The variant.
     *
     * @return The reserved operation or null pointer if no node
     * has been reserved.
     */
     Node* get_reserved_operation(Entity* entity);

     /**
     * @brief Check if all content is finished and that all
     * occupied nodes are operational.
     */
     bool all_finished_and_operational();
};

} // namespace xsim

#endif // PARALLELOPERATION_H
