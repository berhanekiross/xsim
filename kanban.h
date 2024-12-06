#ifndef KANBAN_H
#define KANBAN_H

#include <xsim_config>
#include <map>
#include <string>
#include <vector>

#include "enterlogic.h"
#include "double.h"

namespace xsim {

class Buffer;
class Entity;
class Node;
class Simulation;
class Variant;

struct KanbanItem {
    Variant* variant;
    Double limit;
};

/**
 * @brief A node that can be used to limit the flow of entities.
 */
class XSIM_EXPORT Kanban : public EnterLogic {
 public:
     /**
      * @brief Constructor.
      */
     Kanban();

     /* Documented in object.h */
     void simulation_init() override;
     void init() override;
     void finalize() override;
     void reset_stats() override;
     void define_outputs() override;

     /* Documented in enterlogic.h */
     bool allow_enter(Node *node, Entity *entity) override;

     /**
      * @brief Do the necessary actions when a entity enters the kanban.
      *
      * @param  departure   The node the entity is coming from.
      * @param  destination The node the entity going to.
      * @param  entity      The entity that entered a node.
      */
     void entering(Node* departure, Node* destination, Entity* entity);

     /**
      * @brief Do the necessary actions when a entity exits the kanban.
      *
      * @param departure The node the entity exited.
      * @param destination The node the entity entered.
      * @param entity The entity that exited.
      */
     void exited(Node* departure, Node* destination, Entity* entity);

     /**
      * @brief Add a limit on a specific variant.
      *
      * @param variant The variant to limit.
      * @param limit The maximum number of entities of the variant.
      */
     void add_variant_limit(Variant *variant, Double limit);

     /**
      * @returns All kanban items.
      */
     const std::vector<KanbanItem>& variants() const;

    /**
     * @brief The total WIP for all variants.
     *
     * @returns The total WIP.
     */
    double wip() const;

    /**
     * @brief The WIP for a particular variant.
     *
     * @param  variant The variant to get the WIP for.
     *
     * @returns The WIP.
     */
    double wip(Variant *variant) const;

private:
     /**
      * @brief Helper class to keep track of kanban information.
      */
     struct KanbanPrivateItem {
         int limit;
         int count;
         simtime wip_time_sum;
     };
     typedef std::map<Variant*, KanbanPrivateItem> Variants;

     /**
      * @brief Add time to the work in process.
      *
      * @param time The amount of time to add.
      */
     void add_wip_time(simtime time);

     /**
      * @brief Increase the entity count.
      *
      * @param entity The root entity.
      * @param variants The results.
      */
     void increase_wip(Entity *entity, Variants &variants);

     /**
      * @brief Decrease the entity count.
      *
      * @param entity The root entity.
      * @param variants The results.
      */
     void decrease_wip(Entity *entity, Variants &variants);

     /**
      * @brief How many of each variant that is currently on this kanban.
      */
     std::map<Variant*, KanbanPrivateItem> variants_;

     /**
      * @brief The last log time.
      */
     simtime last_log_time_;

     /**
      * @brief used to preserve the order the variants were added.
      */
     std::vector<KanbanItem> order_variants_;
};

} // namespace xsim

#endif // KANBAN_H
