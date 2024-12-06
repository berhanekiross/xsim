#ifndef MAXWIP_H
#define MAXWIP_H

#include <xsim_config>
#include <map>
#include <string>

#include "enterlogic.h"
#include "double.h"
#include "int.h"

namespace xsim {

class Entity;
class Node;
class Simulation;
class Variant;

class MaxWipVariantLimitItem {
public:
    Variant* variant;
    Double limit;
};

/**
 * @brief A logic that can limit the amount of entities in an area.
 */
class XSIM_EXPORT MaxWip : public EnterLogic {
public:
    Signal<void(Logic*, Entity*)> entity_entered;
    Signal<void(Logic*, Entity*)> entity_exiting;
    Signal<void(Logic*, Entity*)> entity_exited;

    /**
     * @brief Constructor.
     *
     * @param max The maximum number of entities in this max wip.
     */
    MaxWip(Int max);

    void simulation_init() override;
    void init() override;
    void finalize() override;
    void reset_stats() override;
    void define_outputs() override;

    /* Documented in enterlogic.h */
    bool allow_enter(Node *node, Entity *entity) override;

     /**
      * @brief Add a limit on a specific variant.
      *
      * @param variant The variant to add the limit for.
      * @param limit The maximum number of entities of a particular variant
      *  allowed in this max wip.
      */
    virtual void add_variant_limit(Variant *variant, Double limit);

    /**
     * @returns All variant limit items.
     */
    const std::vector<MaxWipVariantLimitItem>& variant_limits() const;

    /**
     * @param  value The maximum number of entities to set.
     */
    void set_max(Int value);

    /**
     * @brief Get the maximum number of entities allowed in this max wip.
     *
     * @return The maximum number of entities allowed in this max wip.
     */
    virtual Int max() const;

    /**
     * @brief Get the current number of entities in this max wip.
     *
     * @return The current number of entities in this max wip.
     */
    virtual int current() const;

    /**
     * @brief Do the necessary actions when a entity enters the max wip.
     *
     * @param  departure   The node the entity is coming from.
     * @param  destination The node the entity is going to.
     * @param  entity      The entity that entered a node.
     */
    virtual void entered(Node *departure, Node* destination, Entity *entity);

    /**
     * @brief Do the necessary actions when an entity is about to leave the max wip.
     *
     * @param  departure   The node the entity is about to leave.
     * @param  destination The node the entity is about to enter.
     * @param  entity      The entity that is moving.
     */
    virtual void exiting(Node *departure, Node* destination, Entity *entity);

    /**
     * @brief Do the necessary actions when a entity exits the max wip.
     *
     * @param  departure   The node the entity exited.
     * @param  destination The node the entity entered.
     * @param  entity      The entity that moved.
     */
    virtual void exited(Node* departure, Node* destination, Entity* entity);

    /**
     * @brief Used as a callback for when entities are deleted.
     *
     * @param entity The entity that was deleted.
     */
    virtual void entity_deleted(Entity *entity);

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
     * @brief Helper class to keep track of max wip information.
     */
    struct Item {
        int limit;
        int count;
        simtime wip_time_sum;
    };
    typedef std::map<Variant*, Item> Variants;
    //typedef std::map<Variant*, double> VariantTimes;
    typedef std::map<Entity*, double> EntityTimes;

    /**
     * @brief Remove a tracked entity.
     *
     * @param entity The entity to remove.
     * @param disconnect_delete_slot True if the deleted slot should be disconnected from the entity.
     */
    void remove_entity(Entity* entity, bool disconnect_deleted_slot);

    /**
     * @brief The maximum number of entities in this max wip.
     */
    Int max_;

    /**
     * @brief The current number of entities in this max wip.
     */
    int current_;

    /**
     * @brief All entities in this max wip and when they arrived.
     */
    EntityTimes entities_;

    /**
     * @brief True if the limit table is empty.
     */
    bool empty_limit_table_;

    /**
     * @brief The variants tracked by this max wip.
     */
    Variants variants_;

    /** @brief The variant limits */
    std::vector<MaxWipVariantLimitItem> variant_limits_;
};

} // namespace xsim

#endif // MAXWIP_H
