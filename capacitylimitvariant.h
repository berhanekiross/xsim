#ifndef CAPACITYLIMITVARIANT_H
#define CAPACITYLIMITVARIANT_H

#include <xsim_config>
#include <map>
#include <utility>

#include "capacitylimit.h"
#include "double.h"

namespace xsim {

class Variant;

/** @brief Helper class to keep track of the different variants and their
 * associated information.
 */
struct CapacityLimitVariantItem {
    Variant* variant;
    Double capacity;
    Double safety_limit;
};

/**
 * @brief CapacityLimitVariant has two purposes, the first is to specify how
 * many entities of a particular variant a node can hold. Together with this it
 * is possible to specify a safety limit. When the number of entities, of
 * particular variant, falls below the safety limit it creates a demand for
 * this variant. The second purpose is to actually keep track of how many
 * entities that are currenty on a node, so a node can know when it is full and
 * how much demand there is for each variant.
 */
class XSIM_EXPORT CapacityLimitVariant : public CapacityLimit {
public:
    /** @brief Destructor */
    ~CapacityLimitVariant();

    void simulation_init() override;

     /**
      * @brief Specifies the safety limit and how many entities of a
      * particular variant this capacity can hold.
      *
      * @param variant The variant.
      * @param capacity The maximum number of entities of the variant.
      * @param safety_limit When the number of entities falls belows this
      * a demand equal to the difference is created.
      */
     void insert_variant(Variant *variant, Double capacity, Double safety_limit);

     /**
      * @brief Get the demand for a particular variant with respect to the
      * safety limit.
      *
      * @param variant The variant to get the demand for.
      *
      * @return The demand.
      */
     unsigned int get_demand(Variant *variant) const override;

     /* Documented in capacitylimit.h */
     void add_entity(Entity *entity) override;
     void remove_entity(Entity *entity) override;
     bool is_full(Entity *entity) const override;
     void init() override;


     /**
      * @returns All capacity limit variant items.
      */
     const std::vector<CapacityLimitVariantItem*> variants() const;

 private:
     struct CapacityLimitVariantItemPrivate {
         unsigned int size;
         unsigned int capacity;
         unsigned int safety_limit;
};
     /**
      * @brief Gets a capacity limit item for a variant.
      *
      * @param variant The variant.
      *
      * @returns The capacity limit variant item if found, otherwise nullptr.
      */
     CapacityLimitVariantItemPrivate* get_item(Variant* variant) const;

     /**
      * @brief All variants.
      */
     std::vector<CapacityLimitVariantItem*> variants_;

     /** @brief Internal data structure to keep track of variants and their limits. */
     std::map<Variant*, CapacityLimitVariantItemPrivate*> capacities_;
};

} // namespace xsim

#endif // CAPACITYLIMITVARIANT_H
