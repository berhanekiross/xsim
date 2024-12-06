#ifndef STORE_H
#define STORE_H

#include <xsim_config>
#include <list>
#include <string>

#include "entitytime.h"
#include "node.h"
#include "int.h"

namespace xsim {

class CapacityLimit;
class EventOut;
class Failure;
class Entity;
class Simulation;
class NumberGenerator;

/**
 * @brief A node that can store one or more entities. The entities
 * can leave in any order.
 */
class XSIM_EXPORT Store : public Node {
 public:
     /**
      * @brief Constructor.
      */
     Store();

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
     size_t max_occupation() const override;
     size_t min_occupation() const override;
     size_t content_size() const override;
     std::string text() const override;
     bool enter(Entity *entity, Node* departure) override;
     void leave(Entity *entity, Node *destination) override;

     /**
      * @brief Sets maximum size
      *
      * @param  max_size The maximum number of entities that can be stored at any given time.
      */
     void set_max_size(Int max_size);

     /**
      * @brief Get the maximum size of this store.
      *
      * @return The maximum size.
      */
     const Int& max_size() const;

     /**
      * @brief Get a all entities that are currently located on this
      * store, along with the simulation time on which they can leave.
      *
      * @return All entities on this buffer.
      */
     std::list<EntityTime> store_contents() const;

     /**
      * @brief Check if the store is at full capacity.
      *
      * @return True if it is at full capacity.
      */
     virtual bool is_full() const;

     /**
      * @brief Check if the store is at full capacity regarding a particular
      * entity.
      *
      * @param entity The entity that is used for checking if the store is
      * at full capacity.
      *
      * @return True if the store is at full capacity with the entity.
      */
     virtual bool is_full(Entity *entity) const;

     /**
      * @brief Set the capacity limit.
      *
      * @param capacity_limit The new capacity limit.
      */
     void set_capacity_limit(CapacityLimit *capacity_limit);

     /**
      * @brief Get the demand for a variant.
      *
      * @param variant The variant to get the demand for.
      *
      * @return The demand for the variant.
      */
     unsigned int get_demand(Variant *variant);

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
      * @brief All entities that currently are located on this node, along
      * with the simulation time they are ready to leave.
      */
     std::list<EntityTime> buffer_;

     /**
      * @brief The maximum capacity of entities on this node.
      */
     Int max_size_;

     /**
      * @brief The maximum number of simultaneous entities on this buffer.
      */
     size_t max_occupied_;

     /**
      * @brief The minimum number of simultaneous entities on this buffer.
      */
     size_t min_occupied_;

     /**
      * @brief The simulation time when stats were last saved.
      */
     simtime enter_time_;

     /**
      * @brief Keeps track of how many entities of each variant that is
      * currently located on this store.
      */
     CapacityLimit *capacity_limit_;
};

} // namespace xsim

#endif // STORE_H
