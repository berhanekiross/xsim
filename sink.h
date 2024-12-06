#ifndef SINK_H
#define SINK_H

#include <xsim_config>
#include <string>

#include "node.h"

namespace xsim {

class EventOut;
class EventSetupEnd;
class Failure;
class Entity;
class NumberGenerator;

/**
 * @brief A node that destroys entities.
 */
class XSIM_EXPORT Sink : public Node {
 public:
     /**
      * @brief Constructor.
      */
     Sink();

     /**
      * @brief Desctructor.
      */
     virtual ~Sink();

     void init() override;
     void finalize() override;
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
     void interrupt_repair_resource() override;
     void resume_repair_resource(bool add_elapsed_time = true) override;
     void interrupt_setup_resource() override;
     void resume_setup_resource(Entity *entity, bool add_elapsed_time = true) override;
     size_t min_occupation() const override;
     size_t max_occupation() const override;
     size_t content_size() const override;
     bool enter(Entity *entity, Node* departure) override;
     void setup_end() override;

     /* Documented in exitport.h */
     void leave(Entity *entity, Node *node) override;

 private:
     /**
      * @brief Schedule an out event for an entity.
      *
      * @param entity The entity to schedule an out event for.
      */
     bool schedule_out(Entity *entity);

     /**
      * @brief Start setting up the operation for another entity.
      *
      * @param time The time the setup is complete.
      */
     void setup(double time);

     /**
      * @brief Cancel the scheduled setup event.
      */
     void cancel_setup();

     /**
      * @brief Reschedule the canceled setup event.
      *
      * @param time The new time the setup event should be finished.
      */
     void schedule_canceled_setup(simtime time);

     /**
      * @brief True if the setup event is canceled.
      */
     bool setup_event_canceled_;

     /**
      * @brief The simulation time when the assemble event is scheduled.
      */
     simtime setup_event_time_;

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

#endif // SINK_H
