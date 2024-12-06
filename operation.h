#ifndef OPERATION_H
#define OPERATION_H

#include <xsim_config>
#include <string>

#include "node.h"

namespace xsim {

class EventOut;
class EventSetupEnd;
class Failure;
class Entity;
class NumberGenerator;
class Variant;

/**
 * @brief A node that process one movble unit at a time.
 */
class XSIM_EXPORT Operation : public Node {
 public:
     /**
      * @brief Constructor.
      */
     Operation();

     /**
      * @brief Destructor.
      */
     virtual ~Operation();

     void finalize() override;
     void init() override;
     void reset_stats() override;

     /* Documented in node.h */
     void define_outputs() override;
     void disruption_begin(Failure *failure,
            std::map<Node*, bool> &visited, int level, 
            bool propagate_failure = true) override;
     void disruption_end(Failure *failure,
            std::map<Node*, bool> &visited, int level, 
            bool propagate_failure = true) override;
     void unplanned_begin() override;
     void unplanned_end() override;
     void paused_begin() override;
     void paused_end() override;
     void interrupt_processing_resource() override;
     void resume_processing_resource(bool add_elapsed_time = true) override;
     void interrupt_setup_resource() override;
     void resume_setup_resource(Entity *entity, bool add_elapsed_time = true) override;
     bool need_setup_resources(Entity *entity) const override;
     bool try_allocate_resources(Entity *entity, bool do_schedule_event_out) override;
     bool try_allocate_setup_resources(Entity *entity, bool do_schedule_event_out) override;
     size_t min_occupation() const override;
     size_t max_occupation() const override;
     size_t content_size() const override;
     void setup_end() override;
     bool enter(Entity *entity, Node* departure) override;
     void leave(Entity *entity, Node *node) override;

     void schedule_request_resources(Entity *entity);
     void schedule_request_setup_resources(Entity *entity);

     /**
      * @brief Start setting up the operation for another entity.
      *
      * @param time The time the setup is complete.
      */
     void do_setup(double time);

protected:
    /**
     * @brief Schedule an out event for an entity when it enters.
     * 
     * If the processing time cannot be determined at the time of entry, then this function can be
     * overloaded with an empty implementation that just returns zero. Just remember to schedule
     * the out event at some point, otherwise the entity will never exit.
     *
     * @param entity The entity that entered.
     *
     * @return The time when the entity will leave.
     */
    virtual double schedule_entity_out(Entity* entity);

 private:
     /**
      * @brief Schedule out event for an entity.
      *
      * @param entity The entity to schedule an out event for.
      */
     void schedule_out(Entity *entity);

	 /**
	  * @brief Perform necessary checks for lost events during
	  *			none operational time, i.e. lost out events due
	  *			to resource being ready during none operational
	  *			time or rescheduling of cancelled setup events.
	  */
	 void handle_operational();

     /**
      * @brief Checks if setup is used and required at this time.
      *
      * @param entity		The next entity to enter or 0 if
	  *							setup has been cancelled.
	  *
      * @return True if setup is required otherwise false.
      */
	 bool require_setup(Entity *entity) const;

     /**
      * @brief Cancel the scheduled setup event.
	  *
	  * @return True if an ongoing setup was cancelled.
      */
     bool try_cancel_setup();

     /**
      * @brief Reschedule the cancelled setup event.
	  *
	  * @return True if a cancelled setup was rescheduled.
      */
     bool try_schedule_cancelled_setup();

     /**
      * @brief True if the setup event is cancelled.
      */
     bool setup_event_cancelled_;

     /**
      * @brief The setup time remaining once it is cancelled.
      */
     simtime remaining_setup_time_;

     /**
      * @brief The currently scheduled setup end event.
      */
     EventSetupEnd *event_setup_end_;

     /**
      * @brief The simulation time when stats were last saved.
      */
     simtime enter_time_;
};

} // namespace xsim

#endif // OPERATION_H
