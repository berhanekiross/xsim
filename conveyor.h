#ifndef CONVEYOR_H
#define CONVEYOR_H

#include <xsim_config>
#include <list>
#include <string>

#include "conveyoritem.h"
#include "entitytime.h"
#include "node.h"
#include "int.h"
#include "double.h"
#include "signal.hpp"

namespace xsim {

class EventOpenConveyor;
class EventUpdateConveyor;
class EventAnimateConveyor;
class EventOut;
class Failure;
class Entity;
class Simulation;
class NumberGenerator;

/**
 * @brief A node that can store one or more entities. The entity
 * leave in first in first out order.
 */
class XSIM_EXPORT Conveyor : public Node {
 public:
    Signal<void (Conveyor*, std::vector<ConveyorAnimate*>)> conveyor_animate;

     /**
      * @brief Constructor.
      */
     Conveyor();

     /**
      * @brief Destructor.
      */
     virtual ~Conveyor();

     void simulation_init() override;
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
     double relative_occupation() const override;
     size_t max_occupation() const override;
     size_t min_occupation() const override;
     size_t content_size() const override;
     std::string text() const override;
     bool is_open(Entity *entity, bool ignore_full) override;
     bool enter(Entity* entity, Node* departure) override;
     void leave(Entity* entity, Node* destination) override;

     /**
      * @brief Sets the length of the conveyor (m).
      *
      * @param  length The length.
      */
     void set_length(Double length);

     /**
      * @brief Get the length of the conveyor (m).
      *
      * @return The length.
      */
     Double length() const;

     /**
      * @brief Sets the speed of the conveyor (m/s).
      *
      * @param  speed The speed.
      */
     void set_speed(Double speed);

     /**
      * @brief Get the speed of the conveyor (m/s).
      *
      * @return The speed.
      */
     Double speed() const;

     /**
      * @param  use_max_size If true max_size can be used to limit the content on the conveyor more
      *                      restrictively than the actual space restrictions.
      */
     void set_use_max_size(bool use_max_size);

     /**
     * @brief Check if max_size should be considered.
     *
     * @return True if max_size is considered.
     */
     bool use_max_size() const;

     /**
      * @param  max_size The maximum number of entities that can be stored on this conveyor, i.e. a
      *                  restriction on top of space restrictions. Only considered when use_max_size is
      *                  true.
      */
     void set_max_size(Int max_size);

     /**
      * @brief Get the maximum size of this conveyor.
      *
      * @return The maximum size.
      */
     Int max_size() const;

     /**
      * @param  value True if the conveyor should accumulate entities.
      */
     void set_accumulating(bool value);

     /**
      * @returns True if the conveyor should accumulate entities.
      */
     bool accumulating() const;

     /**
      * @param  value True enable animation events.
      */
     void set_animation_enabled(bool value);

     /**
      * @returns True if animation is enabled.
      */
     bool animation_enabled() const;

     /**
      * @param  value True if the variants are placed length-wise on the conveyor.
      */
     void set_length_oriented(bool value);

     /**
      * @returns True if the variants are placed length-wise on the conveyor.
      */
     bool length_oriented() const;

     /**
      * @brief Get a all entities that are currently located on this
      * conveyor, along with the simulation time on which they can leave.
      *
      * @return All entities on this conveyor.
      */
     std::list<EntityTime> buffer_contents() const;

     /**
      * @brief Check if the buffer is at full capacity.
      *
      * @return True if it is at full capacity.
      */
     bool is_full() const;

     /**
      * @brief Get the average number of entities on the conveyor.
      *
      * @return The wip.
      */
     double wip() const;

     /**
      * @brief Notfies that a entity has come far enough along the
      * conveyor so that new entities can enter. Should only be
      * called from EventOpenConveyor.
      *
      * @param entity The entity that was blocking the entrance.
      */
     void open_front(Entity *entity);

     /**
      * @brief Recalculate how far a entity can travel on the conveyor
      * until it is either blocked or can leave. Can also trigger updates on
      * other entities as well. Should only be called from
      * EventUpdateConveyor.
      *
      * @param entity The entity to update.
      */
     void update_conveyor(Entity *entity);

     /**
      * @brief Clear all animation callbacks.
      */
     void clear_animate_callbacks();

     /**
      * @brief Trigger an animation update.
      */
     void animation_update();

     /**
      * @brief Get the state (start position and length) of all entities
      * currently on this conveyor.
      *
      * @return The start position and length of all entities. It is the
      * caller responsibility to delete the ConveyorAnimate objects.
      */
     std::vector<ConveyorAnimate*> get_conveyor_state() const;

     /**
      * @brief Trigger animation of the conveyor.
      */
     void animate(bool reschedule = true);

     /**
      * @brief Set the interval how often the animation is updated.
      *
      * @param interval The update interval in seconds.
      */
     void set_animation_interval(double interval);

     /**
      * @returns The animation update interval in seconds.
      */
     double animation_interval() const;

 private:
     /**
      * @brief Start the conveyor after it has been stopped.
      *
      * @param state True if a state changed is the cause of the start.
      */
     void start_conveyor(bool state);

     /**
      * @brief Stop the conveyor. All entities are stopped and do not
      * move until the conveyor is started again.
      *
      * @param state True if a state changed is the cause of the stop.
      */
     void stop_conveyor(bool state);

     /**
      * @brief Try to schedule an update event for a entity on the
      * conveyor, if it fails it will become blocked.
      *
      * @param data The entity to schedule the update on.
      *
      * @return True if an update event was scheduled.
      */
     bool schedule_update_event(ConveyorItem &data);

     /**
      * @brief Schedules an animate event, if the conditions are right.
      */
     void schedule_animate_event();

     /**
      * @brief Stop the conveyor. All entities are stopped and do not
      * move until the conveyor is started again.
      *
      * @param state True if a state changed is the cause of the stop.
      */
     void update_position(ConveyorItem &data);

     /**
      * @brief Get the length of the variant, takes into account the
      * orientation of the conveyor.
      *
      * @param variant the variant to get the length of.
      *
      * @return The length in mm.
      */
     int variant_length(Variant *variant) const;

     /**
      * @brief Get the maximum distance a entity can travel.
      *
      * @param entity The entity to get the distance for.
      *
      * @return The distance the entity can travel.
      */
     int max_travel_distance(Entity *entity);

     /**
      * @brief Get the distance that have been covered since a given time.
      *
      * @param time The start time to calculate the distance from.
      *
      * @return The distance traveled.
      */
     double distance_traveled(simtime time);

     /**
      * @brief Checks that the contents of the conveyor do not overlap.
      */
     void validate();

     /**
      * @brief All entities that currently are located on this node, along
      * with the simulation time they are ready to leave.
      */
     std::list<ConveyorItem> buffer_;

     /**
      * @brief The length of the conveyor (m).
      */
     Double length_m_;

     /**
      * @brief The length of the conveyor (mm).
      */
     int length_;

     /**
     * @brief If true max_size_ is considered.
     */
     bool use_max_size_;

     /**
      * @brief The maximum capacity of entities on this node.
      * Default -1 space is the only limiting factor.
      */
     Int max_size_;

     /**
      * @brief The speed of the conveyor (m/s).
      */
     Double speed_m_per_second_;

     /**
      * @brief The speed of the conveyor (m/s).
      */
     double speed_;

     /**
      * @brief True if the conveyor accepts new entities.
      */
     bool is_open_;

     /**
      * @brief The maximum number of simultaneous entities on this buffer.
      */
     size_t max_occupied_;

     /**
      * @brief The minimum number of simultaneous entities on this buffer.
      */
     size_t min_occupied_;

     /**
      * @brief True if the conveyor is enabled and is moving.
      */
     bool enabled_;

     /**
      * @brief True if a blocked entity does not stop the entire
      * conveyor. This allows entity to accumulate.
      */
     bool is_accumulating_;

     /**
      * @brief True if a the conveyor place the variants along the length of
      * the variant. If it is false the variants are placeed along the width
      * instead.
      */
     bool length_oriented_;

     /**
      * @brief The time that should pass between animation updates.
      */
     double animation_interval_;

     /**
      * @brief The length of all variants that are currently on the
      * conveyor.
      */
     unsigned int occupation_length_;

     /**
      * @brief The length of all variants that are currently blocked.
      */
     int block_length_;

     /**
      * @brief True if a entity is blocked from leaving.
      */
     bool is_out_event_blocked_;

     /**
      * @brief The time the occupied statistics were last updated.
      */
     simtime last_update_;

     /**
      * @brief The accumulated length of the variants that have been on the
      * conveyor.
      */
     double occupation_;

     /**
      * @brief The current open event.
      */
     EventOpenConveyor *open_event_;

     /**
      * @brief How much further the first variant has to travel before the the
      * open event can happen.
      */
     double open_remaining_;

     /**
      * @brief The start time of the open event.
      */
     simtime open_start_;

     /**
      * @brief True if the open event is canceled.
      */
     bool is_open_event_canceled_;

     /**
      * @brief True if the open event is canceled by a state change
      * (failed, paused or unplanned).
      */
     bool is_open_event_state_canceled_;

     /**
      * @brief The update event.
      */
     EventUpdateConveyor *update_event_;

     /**
      * @brief True if the update event is canceled.
      */
     bool is_update_event_canceled_;

     /**
      * @brief The start time of the update event.
      */
     simtime update_start_;

     /**
      * @brief The time the update event was canceled.
      */
     simtime update_canceled_start_;

     /**
      * @brief The animate event.
      */
     EventAnimateConveyor *animate_event_;

     /**
      * @brief True if animation events should be sent out.
      */
     bool animation_enabled_;

     /**
      * @brief The minimum distance (mm) we will move a entity.
      */
     double precision_;
};

} // namespace xsim

#endif // CONVEYOR_H
