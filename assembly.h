#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include <list>
#include <map>
#include <string>

#include "node.h"
#include "signal.hpp"

namespace xsim {

class AssemblySpecification;
class EventAssemble;
class EventOut;
class EventSetupEnd;
class Failure;
class Entity;
class NumberGenerator;
class Variant;

/**
 * @brief A node that assembles entity(s) on another entity.
 */
class XSIM_EXPORT Assembly : public Node {
 public:
     Signal<void (Node*, Entity*)> entity_assembled;

     /**
      * @brief Constructor.
      */
     Assembly();

     /**
      * @brief Destructor.
      */
     virtual ~Assembly();

     void init() override;
     void finalize() override;
     void reset_stats() override;

     /* Documented in node.h */
     void define_outputs() override;
     void disruption_begin(Failure *failure,
            std::map<Node*, bool> &visited, int level, bool propagate_failure = true) override;
     void disruption_end(Failure *failure,
            std::map<Node*, bool> &visited, int level, bool propagate_failure = true) override;
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
     double relative_occupation() const override;
     size_t content_size() const override;
     void setup_end() override;
     bool is_open(Entity *entity, bool ignore_full) override;
     bool enter(Entity *entity, Node* departure) override;
     void leave(Entity *entity, Node *destination) override;

     /**
      * @brief Set if the the container must enter the assembly before
      * any parts are accepted.
      *
      * @param value True if the container must come before any parts.
      */
     void set_container_first(bool value);

     /**
      * @returns True if the container must come before any parts.
      */
     bool container_first() const;

     /**
      * @brief Get the average size of all assembly specifications that have
      * been assembled so for during the simulation.
      *
      * @return The average size.
      */
     double average_max_size() const;

     /**
      * @brief Add an assembly specification that should be used at this
      * assembly.
      *
      * @param assembly_specification The assembly specification to add.
      */
     void add_assembly_specification(
             AssemblySpecification *assembly_specification);

     /**
      * @returns All assembly specifications.
      */
     const std::vector<AssemblySpecification*>& assembly_specifications() const;

     /**
      * @brief Get all entities at this assembly.
      *
      * @return All entities.
      */
     std::vector<Entity*> entities() const;

     /**
      * @brief Assembles the parts onto the container.
      *
      * @param entity The container.
      */
     void assemble(Entity *entity);

     /**
      * @brief Set a maximum wait time for completing the assembly.
      *
      * A maximum time (chosen by a time distribution) after which the assembly
      * is assembled even though it might not be complete. A pre-condition
      * is that the container must have arrived. The time starts when the first
      * variant (that is not the container) enters the assembly. If it is not
      * set then there is no time limit.
      *
      * @param number_generator The distribution of the wait times.
      */
     void set_force_assembly_generator(NumberGenerator *number_generator);

     /**
      * @returns The force assembly generator if set, otherwise nullptr.
      */
     NumberGenerator* force_assembly_generator() const;

     /**
      * @brief Check if the assembly is ready to be assembled.
      *
      * @return True if it is complete.
      */
     bool is_assembly_complete() const;

     /**
      * @brief Check if the assembly have assembled the active assembly.
      *
      * @return True if it is assembled.
      */
     bool is_assembly_assembled() const;

     /**
      * @brief Check if a variant is needed by any active
      * assembly specification.
      *
      * @param variant The variant to check.
      *
      * @return True if this variant is needed.
      */
     bool is_variant_needed(Variant *variant) const;

     /**
      * @brief Check if a variant is the container for any assembly
      * specification.
      *
      * @param variant The variant unit to check.
      *
      * @return True if it is a container.
      */
     bool is_container(Variant *variant) const;

     /**
      * @brief Find the largest amount of units needed of a specific variant
      * in any active assemble specification.
      *
      * @param variant The variant to check.
      *
      * @return How many of the variants that are needed.
      */
     int variant_needed(Variant *variant) const;

     /**
     * @brief Start setting up for a new assembly.
      *
      * @param time The time when the assembly is ready to start the assembly process.
      */
     void do_setup(simtime time);

     /**
      * @brief Gets the container.
      *
      * @returns The current container if set, otherwise a nullptr.
      */
     Entity* container() const;

     /**
      * @brief Check if a specific entity use units.
      *
      * @return True if this entity uses units.
      */
     bool entity_use_units(Entity *entity) const;

     /**
      * @brief Perform actions when units enters the node.
      *
      * @param  variant The variant type of the units.
      * @param  units   How many units that entered.
      */
     void units_enter(Variant* variant, int units);

     /**
      * @brief Check how many units of a specific entity that is needed.
      *
      * @param  entity The entity that should be checked.
      *
      * @returns The amount of units that are needed of the specified entity.
      */
     int units_needed(Entity *entity) const;

 private:
     /**
      * @brief Notifies subscribers that an assemble occured.
      *
      * @param node The node the assembly occured on.
      * @param entity The container.
      * @param time The time of the assemble.
      */
     void assembled(Node *node, Entity *entity, simtime time);

     /**
      * @brief Schedule an assemble event for and entity.
      *
      * @param entity The entity to schedule an assembly event for.
      */
     void schedule_assemble(Entity *entity);

     /**
      * @brief Cancel the scheduled setup event.
      */
     void cancel_setup();

     /**
      * @brief Cancel the scheduled assemble event.
      */
     void cancel_assemble();

     /**
      * @brief Reschedule the canceled setup event.
      *
      * @param time The new time the setup event should be finished.
      */
     void schedule_canceled_setup(simtime time);

     /**
      * @brief Reschedule the canceled assemble event.
      *
      * @param time The new time the assemble event should be finished.
      */
     void schedule_canceled_assemble(simtime additional_time);

     /**
      * @brief Schedule an event out for a entity.
      *
      * @param time The entity that should be scheduled.
      */
     void schedule_event_out(Entity *entity);

     /**
      * @brief Get the maximum size of any active assembly specification.
      *
      * @return The maximum size.
      */
     unsigned int max_size_of_active_specifications() const;

     /**
      * @brief Tells the assembly that an assembly specification is complete.
      *
      * @param time The assembly specification that was completed.
      */
     void specification_complete(AssemblySpecification *spec);

     /**
      * @brief All assembly specifications used by this assembly.
      */
     std::vector<AssemblySpecification*> assembly_specifications_;

     /**
      * @brief What assembly specification that should be used for the assembly
      * is decided greedily by only keeping those that needs the incoming
      * entities. This list holds those specifications.
      */
     std::list<AssemblySpecification*> active_specifications_;

     /**
      * @brief The currently active container.
      */
     Entity *active_container_;

     /**
      * @brief All entities that are waiting to be assembled.
      */
     std::vector<Entity*> parts_;

     /**
      * @brief The simulation time when stats were last saved.
      */
     simtime enter_time_;

     /**
      * @brief True if the container must enter before any parts.
      */
     bool container_first_;

     /**
      * @brief The minimum occupation of the assembly.
      */
     size_t min_occupation_;

     /**
      * @brief The maximum occupation of the assembly.
      */
     size_t max_occupation_;

     /**
      * @brief The maximum occupied time this assembly could have had.
      */
     simtime occupied_reference_time_;

     /**
      * @brief True if the assemble event have been canceled.
      */
     bool assemble_event_canceled_;

     /**
      * @brief The simulation time when the assemble event is scheduled.
      */
     simtime assemble_event_time_;

     /**
      * @brief The currently scheduled assemble event.
      */
     EventAssemble *event_assemble_;

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
      * @brief The distribution that determines the maxium wait time until
      * the assembly is forced.
      */
     NumberGenerator *force_assembly_;

     /**
      * @brief An assembly can be blocked from completeing if it is forced
      * but the container have not arrived yet.
      */
     bool assemble_blocked_;
};

} // namespace xsim

#endif // ASSEMBLY_H
