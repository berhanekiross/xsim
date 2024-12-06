#ifndef FAILURE_H
#define FAILURE_H

#include <xsim_config>
#include <list>
#include <string>

#include "node.h"
#include "object.h"
#include "double.h"

namespace xsim {

class EventDisruptionBegin;
class EventDisruptionEnd;
class NumberGenerator;

/**
 * @brief A failure handles the scheduling of disruption events.
 */
class XSIM_EXPORT Failure : public Object {
 public:

     /**
      * @brief Constructor.
      *
      * @param sim The simulation manager.
      * @param name The name of the failure.
      * @param node The node that the failure belong to.
      * @param type The type of the failure.
      * @param reference The time reference that should be used to count the
      * time between two failures.
      */
     Failure(std::string name,
             Node *node,
             FailureTypes type,
             FailureTimeReference reference);

     /**
     * @brief Copy constructor.
     */
     Failure(const Failure& failure, Node* node = 0);

     /**
      * @brief Destructor.
      */
     virtual ~Failure();

     /**
     * @brief Copies the failure to the input node.
     * 
     * @param node The node that should be the owner of the copied 
     *      failure.
     */
     virtual Failure* clone(Node* node = 0) const;

     /* Documented in object.h */
     void simulation_init() override;
     void init() override;
     void finalize() override;
     void reset_stats() override;

     /**
      * @param node Sets the owner node of this failure.
      */
     void set_node(Node* node);

     /**
     * @brief Fail state of the failure.
     */
     bool is_failed() const { return is_failed_; }

     /**
     * @brief Get the type of the failure.
     */
     FailureTypes failure_type() const;

     /**
     * @brief Get the time reference of the failure.
     */
     FailureTimeReference failure_reference() const;

     /**
     * @brief Get the availability of the failure.
     */
     Double availability() const;

     /**
     * @brief Get the mean time to repair of the failure.
     */
     Double mttr() const;

     /**
      * @brief Set the failure number generator of this node.
      *
      * @param number_generator The failure interval.
      */
     void set_interval(NumberGenerator *number_generator);

     /**
      * @brief Get the failure interval of this node.
      *
      * @return The failure interval.
      */
     NumberGenerator* interval() const;

     /**
      * @brief Set the failure duration of this node.
      *
      * @param duration The failure duration.
      */
     void set_duration(NumberGenerator *duration);

     /**
      * @brief Get the failure duration of this node.
      *
      * @return The failure duration.
      */
     NumberGenerator* duration() const;

     /**
      * @brief Set failure interval and failure duration based on availability
      * and mean time to repair.
      *
      * @param availability How much, in percent, this node should be available.
      * @param mttr The mean time to repair.
      */
     void set_availability_mttr(Double availability, Double mttr);

     /**
      * @brief Check if conditions are such that the start of the next failure
      * is cancelable.
      */
     bool is_begin_cancelable() const;

     /**
      * @brief Check if conditions are such that the current failure can be
      * canceled.
      */
     bool is_end_cancelable() const;

     /**
      * @brief Add time that is used to calculate the next failure should occur.
      *
      * @param time The amount to add.
      */
     void add_processing_time(simtime time);

      /**
      * @brief Set stochastic number generators to inifinity.
      */
     void clear_stochastic();

      /**
      * @brief Cancels any pending disruption begin event.
      */
     void cancel_begin();

      /**
      * @brief Cancels any pending disruption end event.
      */
     void cancel_end();

      /**
      * @brief Check if the disruption begin event is canceled.
      *
      * @return True if the disruption begin event is canceled.
      */
     bool is_begin_canceled() const;

      /**
      * @brief Check if the disruption end event is canceled.
      *
      * @return True if the disruption end event is canceled.
      */
     bool is_end_canceled() const;

      /**
      * @brief Notify the failure that an disruption started.
      */
     void disruption_started();

      /**
      * @brief Notify the failure that an disruption ended.
      */
     void disruption_ended();

      /**
      * @brief Add time when the node was down (not operational).
      *
      * @param time The time the node was down.
      */
     void add_down_time(simtime time);

      /**
      * @brief Reschedules any canceled disruption event.
      *
      * @param time The time since the disruption event was canceled.
      */
     void reschedule(simtime time);

     /**
      * @brief Get the number of currently active FailureItems.
      *
      * Same as the number of entities currently located on the node that owns
      * this failure. Used to keep track of total processing time of all
      * entities when failure time reference is PROCESSING.
      *
      * @return Number of FailureItems.
      */
     size_t failure_processing_size() { return failure_processing_.size(); }

     /**
      * @brief Callback method triggered when a node with failure reference
      * PROCESSING changes state. Used to keep track all the time a node
      * has been processing something in order to be able to schedule the
      * when the next failure should occur.
      *
      * @param node The node whose state changes are being monitored.
      * @param state The state that got changed.
      * @param previous_state The state of the node before this state change.
      */
     void node_state_changed(
            Node *node, Node::State state, Node::State previous_state);

     /**
      * @brief Callback method used to keep track of the assembly identity of the last entity to
      *        enter the node that owns this failure.
      *
      * @param  departure   The node the entity is coming from.
      * @param  destination The node the entity going to.
      * @param  entity      The entity that entered a node.
      */
     void entity_enter(Node* departure, Node* destination, Entity* entity);

     /**
      * @brief Observer method used to keep track of the number entities that exited the node that
      *        owns this failure.
      *        
      *        Disassemblies represent a special case where the only exit considered is that of the
      *        entity matching the assembly identity of the entity that entered the disassembly.
      *
      * @param  departure   The node the entity exited.
      * @param  destination The node the entity entered.
      * @param  entity      The entity that enter the node.
      */
     void entity_exited(Node* departure, Node* destination, Entity* entity);

     /**
      * @brief Sets the cycle_count_, that is the number of entity exits
      * that are required before the next failure should be scheduled.
      *
      * Used for failures of type CYCLES. For example this could be used to
      * override the default initialization of the cycle count. Calling this
      * with count = 0 will result in a runtime error being throw.
      *
      * @param count An integer value larger than or equal to 1.
      */
     void set_cycle_count(unsigned int count);

     /**
      * @brief The total time that this failure has been failed until current
      * time (simulation->now()).
      *
      * Note! This doesn't include time when a failure has been canceled for
      * some reason, e.g. missing resource.
      *
      * @return Total time this failure has been failed.
      */
     simtime stats_failed_time();

 private:
     /**
      * @brief Schedules a new failure event.
      */
     void schedule_failure();

     /**
      * @brief Schedule a disruption begin event.
      *
      * @param time The relative time, from now, when the event should happen.
      * @param priority The priority of the event.
      */
     void schedule_disruption_begin(simtime time,
            int priority = PRIORITY_DISRUPTION_BEGIN);

     /**
      * @brief Saves any processing time that are in the past and modifies the
      * failure processing list accordingly.
      *
      * This is only meaningful when the failure reference is set to PROCESSING.
      */
     simtime get_past_processing_time();

     /**
      * @brief Used to update all processing times that all ready occured (i.e.
      * prior to simulation->now()) as used.
      */
     void update_used_processing_times();

     /**
      * @brief Updates the failure processing list after a failure is over.
      *
      * This is only meaningful when the failure reference is set to PROCESSING.
      */
     void update_failure_processing_list();

     /**
      * @brief Checks if a failure should be scheduled based on the current
      * accumlated processing time.
      *
      * This is only meaningful when the failure reference is set to PROCESSING.
      * Note this function assumes that the failure items are sorted and start
      * at the current simulation time.
      */
     void schedule_failure_processing();

     /**
      * @brief The node that owns this failure.
      */
     Node *node_;

     /**
     * @brief The availability of the failure in %, i.e (0,100].
     */
     Double availability_;

     /**
     * @brief The mean time to repair of the failure in %, i.e (0,100].
     */
     Double mttr_;

     /**
      * @brief The number generator for disruption begin events.
      */
     NumberGenerator *failure_interval_;

     /**
      * @brief The number generator for disruption end events.
      */
     NumberGenerator *failure_duration_;

     /**
      * @brief The current, if any, disruption begin event.
      */
     EventDisruptionBegin *event_disruption_begin_;

     /**
      * @brief The current, if any, disruption end event.
      */
     EventDisruptionEnd *event_disruption_end_;

     /**
      * @brief True if the disruption begin event has been canceled.
      */
     bool disruption_begin_event_canceled_;

     /**
      * @brief True if the disruption end event has been canceled.
      */
     bool disruption_end_event_canceled_;

     /**
     * @brief Fail state of the failure.
     */
     bool is_failed_;

     /**
     * @brief Type of the failure.
     */
     FailureTypes failure_type_;

     /**
      * @brief The failure reference that is currently used.
      *
      * The different references affect how the time too next failure is
      * calculated.
      */
     FailureTimeReference failure_reference_;

     /**
      * @brief The relative start time of the next failure.
      */
     simtime next_failure_;

     /**
      * @brief The absolute start time of the next failure.
      */
     simtime next_failure_abs_;

     /**
      * @brief The duration of the current failure.
      */
     simtime next_failure_end_;

     /**
      * @brief Used for keeping track when the next failure should occur,
      * only used when the failure reference is set to PROCESSING.
      */
     class FailureItem {
     public:
         FailureItem(simtime start, simtime stop) :
             start_(start),
             stop_(stop),
             used_(0),
             interrupted_(0) {}

         simtime start()
         {
             return start_;
         }

         simtime stop()
         {
             return stop_ + interrupted_;
         }

         simtime used()
         {
             return used_;
         }

         simtime interrupted()
         {
             return interrupted_;
         }

         void add_interrupted(simtime value)
         {
             interrupted_ += value;
         }

         simtime unused(simtime now)
         {
             simtime ref = stop() < now ? stop() : now;
             simtime unused = ref - (start_ + used_ + interrupted_);
             return unused > 0 ? unused : 0;
         }

         simtime remaining()
         {
             return stop() - (start_ + used_ + interrupted_);
         }

         void update_used(simtime now)
         {
             simtime new_used = unused(now);
             if (new_used > 0)
                 used_ += new_used;
         }
         
     private:
         simtime start_;
         simtime stop_;
         simtime used_;
         simtime interrupted_;
     };

     /**
      * @brief A list of all active process times. Used to calculate when
      * when the next disruption should occur, only used when the failure
      * reference is PROCESSING.
      */
     std::list<FailureItem> failure_processing_;

     /**
      * @brief All processing time that have occured in the past and is no
      * longer in the active list, only used when the failure reference is
      * PROCESSING.
      */
     simtime saved_processing_time_;

     /**
      * @brief Used by the state monitoring callback to keep track of when the
      * previous state change took place (in this case when a change of the
      * processing status of the node was last changed).
      */
     simtime last_processing_observation_;

     /**
      * @brief Adds a callback to monitor state changes of the node that owns
      * this failure.
      */
     void add_state_change_callback();

     /**
      * @brief Adds callbacks to monitor entity exits (and entries for
      * disassemblies) from flow nodes that owns this failure.
      */
     void add_cycles_callbacks();

     /**
      * @brief Counter to keep track of entity exits from the node that owns
      * this failure.
      *
      * During initalization of the failure this will get a random integer
      * value between 1 and mean value of the distribution used for the
      * interval between failures. Using the function set_cycle_count it is
      * possible to override this initialization.
      */
     unsigned int cycle_count_;

     /**
      * @brief Reference to the last entity to enter a disassembly that owns
      * this failure.
      *
      * Used to tell for which exits from a disassembly the cycle_count_ should
      * be decreased.
      */
     Entity *entered_entity_;

     /**
      * @brief The total time this failure has been failed not counting the
      * time when a failure is canceled for some reason, e.g. missing resource.
      */
     simtime stats_failed_time_;

     /**
      * @brief The last time statistics was updated for this failure.
      */
     simtime stats_log_time_;

     /**
      * @brief Updates all statistics up until the current time
      * (simulation->now()).
      */
     void log_stats();
};

} // namespace xsim

#endif // FAILURE_H
