#ifndef SOURCE_H
#define SOURCE_H

#include <xsim_config>
#include <deque>
#include <list>
#include <string>

#include "node.h"
#include "int.h"

namespace xsim {

class EventOut;
class Failure;
class Entity;
class Simulation;
class NumberGenerator;
class Variant;
class VariantCreator;

/**
 * @brief A node that creates entities.
 */
class XSIM_EXPORT Source : public Node {
 public:
     /**
      * @brief Constructor.
      */
     Source();

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
     void leave(Entity* entity, Node* destination) override;
     void set_process_time_generator(NumberGenerator* process_time) override;

     /**
      * @brief Set the number generator that determine which simulation time to
      * start producing entities.
      *
      * @param number_generator The number generator to use.
      */
     void set_start_number_generator(NumberGenerator *start_number_generator);

     /**
      * @returns The start number generator.
      */
     NumberGenerator* start_number_generator() const;

     /**
      * @brief Set the number generator that determine which simulation time to
      * stop producing entities.
      *
      * @param number_generator The number generator to use.
      */
     void set_stop_number_generator(NumberGenerator *stop_number_generator);

     /**
      * @returns The stop number generator.
      */
     NumberGenerator* stop_number_generator() const;

     /**
      * @brief Set the variant creator that should be used to create the
      * entities.
      *
      * @param variant_creator The variant creator to use.
      */
     void set_variant_creator(VariantCreator *variant_creator);

     /**
      * @returns The variant creator if set, otherwise a nullptr.
      */
     VariantCreator* variant_creator() const;

     /**
      * @brief Check if the variant creator has some randomeness when it
      * selects which variants to create.
      *
      * @return True if it is stochastic.
      */
     bool is_variant_creator_stochastic() const;

     /**
      * @brief Create a new entity.
      *
      * @param rescheduled True if event caused by previously failed creation.
      */
     void create_entity(bool rescheduled);

     /**
      * @brief Set a limit of how many entities to create.
      *
      * All entities are created at the start of the simulation from the
      * main number generator.
      *
      * @param limit How many entities to create.
      */
     void set_limit(Int limit);

     /**
      * @returns The limit on how many entities that can be created.
      */
     Int limit() const;

     /**
      * @brief Set whether entities should accumulate or not when
      * the source isn't operational.
      *
      * @param accumulating True if the source should accumulated
      * entity creations when it isn't operational, otherwise false.
      */
     void set_accumulating(bool accumulating);

     /**
      * @returns True if the source accumulates entities.
      */
     bool accumulating() const;

 protected:
     /**
      * @brief Handle things that need to be done once the source gets
      * operational again after being unplanned, paused and/or failed.
      *
      * Typically restart the creation of entities.
      */
     virtual void handle_operational();

     /**
      * @brief Handle creation of previously failed creations.
      */
     virtual void handle_rescheduled_creation();

     /**
      * @brief Handle creation of the next entity that should be created.
      *
      * If a creation fails the creation is added to failed_creations_ to be
      * handled in a FIFO-manner as soon as possible.
      */
     virtual void handle_creation();

     /**
      * @brief Used to check if it is possible to create an entity on the
      * source at this time.
      *
      * @return True if the source is operational and not occupied, otherwise
      * false.
      */
     virtual bool creation_possible();

     /**
      * @brief Used to check if failed creation should be considered failed
      * or be discarded.
      * 
      * Failed creations can be discarded when the source isn't accumulating_.
      */
     virtual bool consider_as_failed_creation();

     /**
      * @brief Used to create an entity on the source of the provided variant. 
      *
      * @param variant The variant from which an entity should be created.
      */
     virtual void create_entity(Variant* variant);

     /**
      * @brief The sequence of variants that the source failed to create
      * entities of.
      *
      * When source is accumulating this should be max one since total
      * scheduled time will handle all "missed" creations, but when the
      * source isn't accumulating this will keep track of the sequence of
      * valid attempts of creation that failed so that they aren't lost.
      */
     std::deque<Variant*> failed_creations_;

     /**
      * @brief True if a failed create entity event is scheduled.
      *
      * Added to distinguish between creation events caused be a
      * previously failed creation and ones caused by the standard
      * number generator.
      */
     bool scheduled_failed_create_entity_event_;

     /**
      * @brief True if a standard creation entity event is scheduled,
      * i.e. not a creation event that handles failed creations.
      */
     bool scheduled_create_entity_event_;

 private:
     /**
      * @brief Schedule an event that will create a entity.
      *
      * @param number_generator The number generator to use to get the
      * simulation time when the event is supposed to happen.
      * @param force_creation_time_update Used to force an update of the
      * creation time, typically for the first creation.
      */
     void schedule_create_entity(NumberGenerator *number_generator, bool force_creation_time_update = false);

     /**
      * @brief Try to schedule a creation that previously failed.
      *
      * @param number_generator The number generator to use to get the simulation time
      * when the event is supposed to happen.
      *
      * @return True if a failed event was scheduled, otherwise false.
      */
     bool try_schedule_failed_create_entity(NumberGenerator *number_generator);

     /**
      * @brief The variant creator determine which variant that should be
      * created.
      */
     VariantCreator *variant_creator_;

     /**
      * @brief How many entities that were created before the warmup
      * ended.
      */
     unsigned int warmup_entities_;

     /**
      * @brief The time to start producing entities.
      */
     NumberGenerator *start_number_generator_;

     /**
      * @brief The time to stop producing entities.
      */
     NumberGenerator *stop_number_generator_;

     /**
      * @brief The time to the next schedule create entity event.
      */
     simtime next_schedule_;

     /**
      * @brief The total amount of time that has been available to create
      * entities. This is like a pool that gets filled when the source
      * can't produce entities, for example if it is failed. This pool can
      * then be tapped to reduce the time until the next create entity
      * event. So that a even production rate is obtained.
      */
     simtime total_schedule_time_;

     /**
      * @brief The simulation time when stats were last saved.
      */
     simtime enter_time_;

     /**
      * @brief The time when the source should stop producing entities.
      */
     simtime stop_time_;

     /**
      * @brief The amount of entities to create, if it is set to the
      * maximum value it is not used.
      */
     Int limit_;

     /**
      * @brief Boolean stating whether failed creation of entities when
      * the source isn't operational should be accumulated or not.
      *
      * If entities are accumulated they will be created as soon as
      * possible after the source is operational again.
      */
     bool accumulating_;
};

} // namespace xsim

#endif // SOURCE_H
