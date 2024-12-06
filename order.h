#ifndef ORDER_H
#define ORDER_H

#include <xsim_config>
#include <map>
#include <vector>

#include "enterlogic.h"

namespace xsim {

class Node;
class Simulation;
class Variant;

/**
 * @brief entities are only allowed if there is an order for them and a
 * connected facade can order them (i.e they are registred). entities
 * that are not registred can always enter.
 */
class XSIM_EXPORT Order : public EnterLogic {
 public:
     /**
      * @brief Constructor.
      */
     Order();

     void init() override;
     void finalize() override;
     void reset_stats() override;

     /* Documented in enterlogic.h */
     bool allow_enter(Node *node, Entity *entity) override;

     /**
      * @brief Do the necessary actions when a entity enters the order.
      *
      * @param  departure   The node the entity is coming from.
      * @param  destination The node the entity going to.
      * @param  entity      The entity that entered a node.
      */
     void entering(Node* departure, Node* destination, Entity* entity);

     /**
      * @brief All variants that later are orded must first be registred.
      *
      * @param variant The variant to register.
      */
    void register_variant(Variant *variant);

    /**
     * @brief Adds a node as an entrance node.
     *
     * @param node The node to add.
     */
    void add_entrance(Node* node);

    /**
     * @returns All entrance nodes.
     */
    const std::vector<Node*>& entrances() const;

    /**
     * @brief Adds a node as an exit node.
     *
     * @param node The node to add.
     */
    void add_exit(Facade* facade);

    /**
     * @returns All exit nodes.
     */
    const std::vector<Facade*>& exits() const;

     /**
      * @brief Place an order for a specific number of variants.
      *
      * @param node The node that placed the order.
      * @param variant The variant to order.
      * @param order_quantity The amount of variant in the order.
      */
     void place_order(Node *node, Variant *variant, int order_quantity);

     /**
      * @brief Get the order priority for a entity.
      *
      * @param entity The entity to get the order priority for.
      *
      * @return The priority of the entity. Lower is better.
      */
     int order_priority(const Entity *entity) const;

 private:

     /**
      * @brief Helper class to keep track of orders.
      */
     class OrderItem {
     public:
         Node *node;
         Variant *variant;
         int order_quantity;
         int current_quantity;
     };

     /**
      * @brief All active orders.
      */
     std::list<OrderItem> orders;

     /**
      * @brief All registered variants.
      */
     std::map<Variant*, bool> registered_variants_;

     /** @brief The entrance nodes */
     std::vector<Node*> entrances_;

     /** @brief The exits nodes */
     std::vector<Facade*> exits_;
};

} // namespace xsim

#endif // ORDER_H
