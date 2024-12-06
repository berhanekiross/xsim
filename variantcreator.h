#ifndef VARIANTCREATOR_H
#define VARIANTCREATOR_H

#include <xsim_config>
#include <map>
#include <vector>

#include "object.h"
#include "int.h"

namespace xsim {

class NumberGenerator;
class Variant;

/**
 * @brief Base class for controlling in which order variants are created.
 */
class XSIM_EXPORT VariantCreator : public Object {
 public:
     /**
      * @brief Constructor.
      */
     VariantCreator();

     /**
      * @brief Destructor.
      *
      */
     virtual ~VariantCreator();

     /**
      * @brief Create a new variant.
      *
      * @param variant The amount of the variant that should be created
      *     and if -1 variant creation should be terminated. OBS!
      *     0 amount is also allowed.
      *
      * @return The type of variant that should created.
      */
     virtual Variant* create(bool complete_batch, bool &terminate) = 0;

     /**
      * @brief Get how many variants that remain in the current batch.
      *
      * @return The remaining variants.
      */
     virtual int remaining_in_batch() const;

     /**
      * @brief Get size of current creation/batch.
      *
      * @return The size of the current batch.
      */
     virtual int current_batch_size() const = 0;

     /**
      * @brief Way of checking whether the creation time/interval should be
      * updated or not.
      *
      * @return True if creation time/interval should be updated otherwise
      * False.
      */
     virtual bool update_creation_time() const;

     /**
      * @brief Check if randomness is used when creating variants.
      *
      * @return True if it is stocastic.
      */
     virtual bool is_stochastic() const = 0;

     /**
      * @brief  Check if more variants can be created or not.
      *
      * Considers constant_zero_amount_, i.e. if only variants with constant zero amount are
      * added to the variant creator it will be considered depleted.
      * 
      * Override this function to signal other cases when there aren't more variants to create, i.e.
      * when this variant creator is depleted.
      * 
      * @returns    True if no more variants can be created.
      */
     virtual bool depleted() const;

     /**
      * @brief Add a variant that the variant creator should create..
      *
      * @param variant The variant.
      */
     void add_variant_handled(Variant *variant);

     /**
      * @brief Check if the  variant is created by this variant creator.
      *
      * @param variant The variant to check for.
      *
      * @return True if it created by this variant creator.
      */
     bool is_handled(Variant *variant) const;

     /**
      * @brief Get the start iterator of the available variants.
      *
      * @return The begin iterator.
      */
     std::vector<Variant*>::const_iterator variants_begin() const;

     /**
      * @brief Get the end iterator of the available variants.
      *
      * @return The end iterator.
      */
     std::vector<Variant*>::const_iterator variants_end() const;

     /**
      * @param  interval_time The interval time to set.
      */
     virtual void set_interval_time(NumberGenerator* interval_time);

     /**
      * @returns The interval time if set, otherwise a nullptr.
      */
     NumberGenerator* interval_time() const;

     /**
      * @brief Used to set a limit of how many entities this should be created.
      *
      * @param limit The number of entities to create.
      */
     void set_limit(Int limit);

     /**
      * @brief Returns the number of created entities.
      */
     int created_count() const;

     /* Documented in object.h */
     virtual void init();

protected:
    /**
    * @brief Bool stating if all added variants have constant zero amounts or not, i.e. if this
    * variant creator will ever produce any variants.
    */
    bool constant_zero_amount_;

     /**
      * @brief The maximum amount of entities that can be created.
      *
      * Default value int max.
      */
     Int limit_;

     /**
      * @brief The number of created entities.
      */
     int created_count_;

private:
     /**
      * @brief All variants that can be created.
      */
     std::map<Variant*, bool> variants_;

     /**
      * @brief Since the map does not preserve the order the variants were
      * added, we need to save it seperatly.
      */
     std::vector<Variant*> variant_order_;

     /** @brief The interval time generator */
     NumberGenerator* interval_time_;
};

} // namespace xsim

#endif // VARIANTCREATOR_H
