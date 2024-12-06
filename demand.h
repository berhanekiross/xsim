#ifndef DEMAND_H
#define DEMAND_H

#include <xsim_config>
#include <map>
#include <queue>

#include "enterlogic.h"
#include "node.h"
#include "int.h"

namespace xsim {

class NumberGenerator;
class Variant;
class VariantCreator;
class Simulation;

/**
 * @brief Can control the flow entities based on demand.
 */
class XSIM_EXPORT Demand : public EnterLogic, public Node {
 public:
     /** @brief Default constructor */
     Demand();

     void simulation_init() override;
     void init() override;
     void finalize() override;
     void reset_stats() override;
     void define_outputs() override;

     /* Documented in enterlogic.h */
     bool allow_enter(Node *node, Entity *entity) override;

     /* Documented in node.h */
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
     size_t min_occupation() const override;
     size_t max_occupation() const override;
     size_t content_size() const override;
     void set_process_time_generator(NumberGenerator* process_time) override;

     /**
      * @brief Do the necessary actions when a entity enters the demand.
      *
      * @param  departure   The node the entity is coming from.
      * @param  destination The node the entity going to.
      * @param  entity      The entity that entered a node.
      */
     void entering(Node* departure, Node* destination, Entity* entity);

     /**
      * @brief Set which variant creator to use.
      *
      * @param variant_creator The variant creator to use.
      */
     void set_variant_creator(VariantCreator *variant_creator);

     /**
      * @returns The variant creator if set, otherwise a nullptr.
      */
     VariantCreator* variant_creator() const;

     /**
      * @brief Set the number generator that determines when the first demand
      * should be created.
      *
      * @param number_generator The number generator to use.
      */
     void set_start_number_generator(NumberGenerator *number_generator);

     /**
      * @returns The start number generator if set, otherwise nullptr.
      */
     NumberGenerator* start_number_generator() const;

     /**
      * @brief Set the number generator that determines when demand should no
      * longer be created.
      *
      * @param number_generator The number generator to use.
      */
     void set_stop_number_generator(NumberGenerator *number_generator);

     /**
      * @returns The stop number generator if set, otherwise nullptr.
      */
     NumberGenerator* stop_number_generator() const;

     /**
      * @brief Create demand for a variant and schedule the next create
      * demand event.
      */
     void create_demand();

     /**
      * @brief set a limit of how much demand to create.
      *
      * all demands are created at the start of the simulation from the
      * main number generator.
      *
      * @param limit how much demand to create.
      */
     void set_limit(Int limit);

     /**
      * @returns The limit on how much demand to create.
      */
     Int limit() const;

     /**
      * @brief Set whether or not this Demand consumes units (true) or entities (false)
      *
      * @param  use_units True to use units.
      */
     void set_use_units(bool use_units);

     /**
      * @returns Whether or not this Demand consumes units (true) or entities (false)
      */
     bool use_units() const;

     /**
      * @brief States whether or not this Demand consumes units (true) or entities (false).
      *
      * @param  entity The entity.
      *
      * @returns True if this demand consumes units otherwise false.
      */
     bool entity_use_units(Entity *entity) const;

     /**
      * @brief Set whether or not this demand should delete entities with 0 units left.
      *
      * @param  delete_entity True to delete the entity.
      */
     void set_delete_entity(bool delete_entity);

     /**
      * @brief States whether or not this Demand deletes entities when all units
      * have been consumed.
      *
      * @return True if this demand deletes entities with 0 units.
      */
     bool delete_entity() const;

     /**
      * @brief Gets the demand for a given variant.
      *
      * @param variant The variant to get the demand for.
      *
      * @return The demand.
      */
     int get_demand(Variant *variant) const;

     /**
      * @brief Gets the backlog for a particular variant.
      *
      * @param  variant The variant to get the backlog for.
      *
      * @returns The backlog.
      */
     unsigned int get_backlog(Variant *variant) const;

     /**
      * @brief Gets the tardiness for a particular variant.
      *
      * @param  variant The variant to get the tardiness for.
      *
      * @returns The tardiness.
      */
     double get_tardiness(Variant *variant) const;

     /**
      * @brief Gets the average tardiness for a particular variant.
      *
      * The average tardiness is the tardiness divided by the backlog.
      *
      * @param  variant The variant to get the average tardiness for.
      *
      * @returns The average tardiness.
      */
     double get_average_tardiness(Variant *variant) const;

     /**
      * @brief The total backlog for all variants.
      *
      * @returns The total number of backlog for all variants.
      */
     int total_backlog() const;

     /**
      * @brief The total tardiness for all variants.
      *
      * @returns The total number of tardiness for all variants.
      */
     double total_tardiness() const;

     /**
      * @brief The total average tardiness for all variants.
      *
      * @returns The total number of average tardiness for all variants.
      */
     double total_average_tardiness() const;

 private:
     typedef std::map<xsim::Variant*, std::queue<simtime> > DemandTime;

     /**
      * @brief Schedule an event that will create a demand.
      *
      * @param next Schedule a create demand event at this time.
      */
     void schedule_create_demand(double next);

     /**
      * @brief The variant creator.
      */
     VariantCreator *variant_creator_;

     /**
      * @brief All demands.
      */
     std::map<Variant*, int> demands_;

     /**
      * @brief Keeps track of when demand was created so that backlog and
      * tardiness can be calculated.
      */
     DemandTime demand_times_;

     /**
      * @brief The backlog for each variant. How many variants that did not
      * meet its demand on time.
      */
     std::map<Variant*, unsigned int> backlog_;

     /**
      * @brief The tardiness for each variant. The total time that each variant
      * was late with a demand.
      */
     std::map<Variant*, simtime> tardiness_;

     /**
      * @brief The number generator that controls when demand creation should start.
      */
     NumberGenerator *start_number_generator_;

     /**
      * @brief The number generator that controls when demand creation should stop.
      */
     NumberGenerator *stop_number_generator_;

     /**
      * @brief The simulation time when the simulation should stop.
      */
     simtime stop_time_;

     /**
      * @brief The maximum amount of variants to create.
      */
     int demand_created_;

     /**
      * @brief The amount of demand to create, if it is set to the
      * maximum value it is not used.
      */
     Int limit_;

     /**
      * @brief Schedule an event that will create a demand.
      *
      * @param interval The number generator to use to get the simulation time
      *     when the event is supposed to happen. Observe that recursive call
      *     will replace start_number_generator_ with number_generator_.
      */
     void schedule_create_demand(NumberGenerator *interval);

     /**
      * @brief Handle the entry of an entity or units and
      * update the statistics accordingly.
      *
      * @param entity The entity that entered.
      * @param amount The amount that entered (1 for entities, use_units_=false).
      */
     void log_entry(Entity *entity, unsigned int amount);

     /**
      * @brief Control demand and consume units from the entity.
      *
      * In case the call orginiated from a triggered block list
      * units shouldn't be consumed, but rather wait for a call that
      * originates from an out-event. Note: That in order for such
      * an out-event to be scheduled this method needs to return
      * true for cases when there is a demand and the call originated
      * from a triggered block list.
      *
      * @param entity The entity whose units should be processed.
      *
      * @return True if all units were consumed -OR- the call
      * originated from a triggered block list and there is a demand.
      */
     bool process_units(Entity *entity);

     /**
      * @brief If true this demand will consume units instead of entities.
      *
      * Units are consumed directly in Demand::Allow and if all units
      * on the entity are consumed delete_entity_ will decide if the
      * will be deleted or not. Default value is false.
      */
     bool use_units_;

     /**
      * @brief If true this demand will delete entities when all units
      * have been consumed.
      *
      * Default value is false.
      */
     bool delete_entity_;

     /**
      * @brief Used to signal that demand creation was tried and failed,
      * for instance due to this logic being failed, paused or unplanned
      * (i.e. not operational).
      */
     bool demand_creation_failed_;
};

} //xsim

#endif // DEMAND_H
