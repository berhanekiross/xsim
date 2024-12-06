#ifndef DISASSEMBLY_H
#define DISASSEMBLY_H

#include <xsim_config>
#include <list>
#include <string>
#include <utility>

#include "entitytime.h"
#include "node.h"
#include "signal.hpp"

namespace xsim {

class Failure;
class Entity;
class NumberGenerator;
class EventOut;
class EventSetupEnd;
class Variant;
class EventDisassemble;

/**
 * @brief A node that disassembles entity(s) from a entity.
 */
class XSIM_EXPORT Disassembly : public Node {
 public:
     Signal<void (Node*, Entity*)> entity_disassembled;

     /**
      * @brief Constructor.
      */
     Disassembly();

     /**
      * @brief Destructor.
      */
     virtual ~Disassembly();

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
     bool is_open(Entity *entity, bool ignore_full) override;
     double relative_occupation() const override;
     size_t min_occupation() const override;
     size_t max_occupation() const override;
     size_t content_size() const override;
     bool enter(Entity *entity, Node* departure) override;
     void leave(Entity *entity, Node *destination) override;

     /**
      * @brief Disassemble all parts from the container.
      *
      * @param entity The container.
      */
     void disassemble(Entity *entity);

     /**
      * @brief Get the current work in process.
      *
      * @return The work in process.
      */
     double wip() const;

     /**
      * @brief Get all entities that are located on this node.
      *
      * @return All entities on this node.
      */
     std::vector<Entity*> entities() const;

     /**
      * @brief There can be several layers of assembled entities, i.e.
      * entities that have assembled parts that themselves are assembled
      * to another entity. If recursive disassemble is used every
      * assembled entity is disassembled, otherwise only the top layer is
      * disassembled.
      *
      * @param value True if recursive disassemble should be used.
      */
     void set_recursive_disassemble(bool value);

     /**
      * @returns True if recursive disassemble should be used.
      */
     bool recursive_disassemble() const;

     /**
      * @param  value True if entities are allowed to overtake.
      */
     void set_overtake(bool value);

     /**
      * @returns True if entities are allowed to overtake.
      */
     bool overtake() const;

     /**
      * @param  value True if empty entities are processed.
      */
     void set_process_empty(bool value);

     /**
      * @returns True if empty entities are processed.
      */
     bool process_empty() const;

     /**
      * @brief Get whether or not the container is forced to leave after
	  * other disassembled entities have exited.
      */
     bool get_container_last();

     /**
      * @brief If True the container should exit after all disassembled
	  * entities have exited, otherwise entities are allowed to leave
	  * independently of each other. Default False.
      *
      * @param value True if the container should be the last to exit.
      */
     void set_container_last(bool value);

 private:
     /**
      * @brief Cancel the disassemble event.
      */
     void cancel_disassemble();

     /**
      * @brief Reschedule the canceled disassemble event.
      *
      * @param time The new time the disassemble should be done.
      */
     void schedule_canceled_disassemble(simtime time);

     /**
      * @brief The disassemble event.
      */
     EventDisassemble *event_disassemble_;

     /**
      * @brief All entities that have been disassembled and are waiting to
      * leave.
      */
     std::list<EntityTime> entities_;

     /**
      * @brief The simulation time when stats were last saved.
      */
     simtime enter_time_;

     /**
      * @brief True if disassemble event have been canceled.
      */
     bool disassemble_event_canceled_;

     /**
      * @brief The maximum number of entities that have been on this node.
      */
     size_t max_occupation_;

     /**
      * @brief The total work in process time.
      */
     simtime wip_;

     /**
      * @brief The size of the total number of entities that got
      * disassembled onto this node.
      */
     size_t last_max_size_;

     /**
      * @brief The maximum occupied time this disassembly could have had.
      */
     simtime occupied_ref_time_;

     /**
      * @brief The simulation time of the disassemble event.
      */
     simtime disassemble_time_;

     /**
      * @brief True if recursive disassemble should be used.
      */
     bool recursive_disassemble_;

     /**
      * @brief True if any of the assembled parts have a destination.
      */
     bool any_part_have_destination_;

     /**
      * @brief True if disassemble events should be scheduled even for empty
      * containers.
      */
     bool process_empty_;

     /**
      * @brief True if a container that is not disassembling anything can
      * overtake a container that already undergoing disassembling.
      */
     bool overtake_;

     /**
      * @brief True if the container entity should be the last to exit.
      */
     bool container_last_;
};

} // namespace xsim

#endif // DISASSEMBLY_H
