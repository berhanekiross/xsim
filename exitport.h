#ifndef EXITPORT_H
#define EXITPORT_H

#include <xsim_config>
#include <string>
#include <functional>
#include <list>
#include <unordered_map>
#include <deque>

#include "common.h"
#include "object.h"
#include "signal.hpp"

namespace xsim {

class Node;
class EventOut;
class ExitLogic;
class Entity;
class Variant;
class MoveController;

/**
 * @brief A departure handles how entities leave nodes.
 */
class XSIM_EXPORT ExitPort : public Object {
 public:
     Signal<void(Variant*, Node*, int weight)> successor_node_added;
     Signal<void (ExitLogic*)> exit_logic_added;

     /**
      * @brief Constructor.
      *
      * @param node The node this departure belongs to.
      */
     ExitPort(Node *node);

     void init() override;
     void finalize() override;
     void reset_stats() override;

     /**
      * @brief Gets the parent node.
      *
      * @returns The parent node.
      */
     Node* node() const;

     /**
      * @brief Override this method to implement how to interrupt an
      * ongoing processing that uses resources.
      */
     virtual void interrupt_processing_resource();

     /**
      * @brief Override this method to implement how to resume an
      * interrupted processing that require resources.
      */
     virtual void resume_processing_resource();

     /**
      * @brief ExitPort specific disruption begin.
      */
     virtual void disruption_begin();

     /**
      * @brief ExitPort specific disruption end.
      */
     virtual void disruption_end();

     /**
      * @brief ExitPort specific unplanned begin.
      */
     virtual void unplanned_begin();

     /**
      * @brief ExitPort specific unplanned end.
      */
     virtual void unplanned_end();

     /**
      * @brief ExitPort specific paused begin.
      */
     virtual void paused_begin();

     /**
      * @brief ExitPort specific paused end.
      */
     virtual void paused_end();

     /**
      * @brief Check if the entity is allowed to leave this node.
      *
      * @param entity The entity that is about the leave.
      *
      * @return True if it is allowed to leave.
      */
     virtual bool allow_leaving(Entity *entity);

     /**
      * @brief Set the move strategy.
      *
      * @param  move_controller The move controller to set.
      */
     virtual void set_move_controller(MoveController* move_controller);

     /**
      * @returns The move controller for this departure.
      */
     virtual MoveController* move_controller() const;

     /**
      * @brief Add entity to exit blocking list.
      *
      * @param entity The entity to add.
      */
     void add_exit_blocking(Entity *entity);

     /**
      * @brief Remove entity from exit blocking list.
      *
      * @param entity The entity to remove.
      */
     void remove_exit_blocking(Entity *entity);

     /**
      * @brief Try to move any entities that currently are on the exit
      * blocking list.
      */
     void check_exit_blocking();

     /**
      * @brief Remove an scheduled out event.
      *
      * @param event_out The event to remove.
      */
     void remove_event_out(EventOut *event_out);

     /**
      * @brief Add an exit logic that can block an exiting entity.
      *
      * @param logic The logic.
      */
     virtual void add_exit_logic(ExitLogic *logic);

     /**
      * @brief Get the number of exits from this node.
      *
      * @return The number of exits.
      */
     unsigned int exits() const;

     /**
      * @brief Get the througput.
      *
      * @return The throughput.
      */
     double throughput() const;

     /**
      * @brief Get the cycle time.
      *
      * @return The cycle time.
      */
     double cycle_time() const;

     /**
      * @brief Creates and schedules an out event for a entity.
      *
      * @param entity The entity that is about to leave.
      * @param time The simualtion time it should leave.
      * @param priority PRIORITY_OUT is the default priority for a regular
      * move, if the entity have been blocked PRIORITY_OUT_UNBLOCK can be
      * used which have a higher priority which means that it will preced any
      * PRIORITY_OUT events that have the same time.
      * @param node The node the entity is leaving, can be a nullptr.
      */
     void schedule_event_out(Entity *entity, simtime time,
             int priority = PRIORITY_OUT, Node *node = nullptr);

     //bool interrupted() const;

     /**
      * @brief Cancel all scheduled out events.
      */
     void cancel_out_events();

     /**
      * @brief Increment number of exits and cycle time.
      *
      * @param entity The entity that left.
      */
     void exit(Entity *entity);

     /**
      * @brief Check if there are scheduled out events.
      *
      * @return True if there are scheduled out events.
      */
     bool have_scheduled_out_events() const;

     /**
      * @return All out events that are currently scheduled.
      */
     const std::deque<EventOut*>& out_events() const;

     /**
      * @brief Set if a canceled event that is resumed should be delayed by
      * the elapsed time.
      *
      * @param value True if the event should be delayed by the elapsed time.
      */
     void set_add_elapsed_time(bool value);

 private:
     /**
      * @brief Schedule new out events for all canceled out events.
      */
     void schedule_canceled_out_events();

     /**
      * @brief Calculate remaining time for an out event.
      *
      * @param  event_out The event to get the remaining time for.
      *
      * @returns The remaining time or 0.0 if remaining time is less than 0.0.
      */
	 simtime get_remaining_out_event_time(EventOut *event_out) const;


     /**
      * @brief The move controller for this departure.
      */
     MoveController* move_controller_;

     /**
      * @brief The number of entities that have left this node.
      */
     unsigned int exits_;

     /**
      * @brief The total amount of time that each entity have spent on
      * this node.
      */
     simtime cycle_time_;

     /**
      * @brief All scheduled out events.
      */
     std::deque<EventOut*> out_events_;

     /**
      * @brief If true out events have been canceled.
      */
     bool out_event_cancelled_;

     /**
      * @brief The simulation time the out events were canceled.
      */
     simtime start_cancel_;

     /**
      * @brief All entity that are exit blocked.
      */
     std::list<Entity*> exit_blocking_;

     /**
      * @brief All logics that can prevent an entity from leaving.
      */
     std::vector<ExitLogic*> logics_;

     /**
      * @brief True if a canceled event that is resumed should be delayed by
      * the elapsed time.
      */
     bool add_elapsed_time_;

     /**
      * @brief The node that owns this departure.
      */
     Node *node_;
};

} // namespace xsim

#endif // EXITPORT_H
