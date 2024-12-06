#ifndef VARIANTCREATORRANDOM_H
#define VARIANTCREATORRANDOM_H

#include <xsim_config>
#include <vector>
#include <random>

#include "variantcreator.h"
#include "double.h"

namespace xsim {

struct VariantCreatorRandomItem {
    Variant* variant;
    Double probability;
};

class Variant;

/**
 * @brief Create variants at random.
 */
class XSIM_EXPORT VariantCreatorRandom : public VariantCreator{
 public:
     /** @brief Default constructor */
     VariantCreatorRandom();

     /* Documented in variantcreator.h */
     Variant* create(bool complete_batch, bool &terminate) override;
     void init() override;
     int current_batch_size() const override;
     bool is_stochastic() const override;
     bool depleted() const override;

     /**
      * @brief Adds a variant to the list of possible variants that can be created.
      *
      * The probability of creating a particular variant is calculated as the
      * variant probability divided by the sum of all variant probabilities.
      *
      * @param          variant     The variant to add.
      * @param          probability The relative probability to create the variant.
      */
     void add_variant(Variant *variant, Double probability);

     /**
      * @returns All variant items.
      */
     const std::vector<VariantCreatorRandomItem>& variants() const;

 private:
     /**
      * @brief The random distribution.
      */
     std::discrete_distribution<> distribution_;

     /**
      * @brief All variants.
      */
     std::vector<VariantCreatorRandomItem> variants_;
};

} // namespace xsim

#endif // VARIANTCREATORRANDOM_H
