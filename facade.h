#ifndef FACADE_H
#define FACADE_H

#include <xsim_config>
#include <string>

#include "node.h"
#include "int.h"

namespace xsim {

class Entity;
class Failure;
class Order;
class Simulation;
class NumberGenerator;

/**
 * @brief Helper class to keep track of orders.
 */
class FacadeOrderItem {
public:
    Variant* variant;
    Int order_point;
    Int order_quantity;
    int level;
};

/**
 * @brief A node that can store one or more entities. The entities
 * can leave in any order. entities are orded when needed.
 */
class XSIM_EXPORT Facade : public Node {
 public:
     /**
      * @brief Constructor.
      */
     Facade();

     /**
      * @brief Destructor.
      */
     virtual ~Facade();

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
     bool enter(Entity *entity, Node* departure) override;
     void leave(Entity* entity, Node* destination) override;

     /**
      * @brief Get the maximum size of this store.
      *
      * @return The maximum size.
      */
     unsigned int max_size() const;

     /**
      * @brief Get a all entities that are currently located on this
      * store, along with the simulation time on which they can leave.
      *
      * @return All entities on this buffer.
      */
     std::list<Entity*> facade_contents();

     /**
      * @brief Add variant that are to be kept at a certain quantity.
      *
      * @param variant The variant that should be monitored.
      * @param order_point When the number of variants fall below this value
      * an order is placed.
      * @param order_quantity When an order is placed this many variants are
      * orded.
      */
     void add_order(Variant *variant, Int order_point, Int order_quantity);

     /**
      * @brief Set connected order object for this facade.
      *
      * @param order The order that is conneced to this facade.
      */
     void set_order(Order *order);

     /**
      * @brief Find the order item from a variant
      *
      * @param variant The variant to get the order item for.
      *
      * @returns The order item if found, otherwise nullptr.
      */
     FacadeOrderItem* get_order_item(Variant* variant) const;

     /**
      * @returns All order items.
      */
     const std::vector<FacadeOrderItem*> orders() const;

 private:

     /**
      * @brief All entities that currently are located on this node.
      */
     std::list<Entity*> buffer_;

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
      * @brief The order connected to this facade.
      */
     Order *order_;

     /**
      * @brief All variants and when to order them.
      */
     std::vector<FacadeOrderItem*> orders_;
};

} // namespace xsim

#endif // FACADE_H
