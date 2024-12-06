#ifndef VARIANTCREATORDELIVERY_H
#define VARIANTCREATORDELIVERY_H

#include <xsim_config>
#include <vector>

#include "variantcreatorsequence.h"
#include "double.h"

namespace xsim {

class Variant;
class NumberGenerator;

struct VariantCreatorDeliveryItem {
    Variant *variant;
    NumberGenerator *amount;
    Double time;
};

/**
 * @brief Create variants using a delivery table.
 */
class XSIM_EXPORT VariantCreatorDelivery : public VariantCreatorSequence {
 public:
     VariantCreatorDelivery();
     ~VariantCreatorDelivery();

     /** @brief Prepare for simulation by sorting and creating the delivery sequence */
     virtual void pre_simulation_init();

     void add_to_creation_sequence(Variant *variant, NumberGenerator* amount) override;
     void set_interval_time(NumberGenerator* number_generator) override;

     /**
      * @brief Add a variant to the sequence.
      *        
      *        The creation of variants will be sorted according to their time, so it is possible to
      *        add the variants in any order.
      *
      * @param  variant The variant to add.
      * @param  amount  Distribution used to set the size of the batch.
      * @param  time    The absolute time when the variant should be created. It is specified as the
      *                 number of seconds from the start (time zero).
      */
     void add_to_creation_sequence(Variant *variant, NumberGenerator* amount, Double time);

     /**
      * @returns The delivery sequence.
      */
     const std::vector<VariantCreatorDeliveryItem*>& delivery_sequence() const;

private:
    /** @brief The sequence of variants */
    std::vector<VariantCreatorDeliveryItem*> sequence_;
};

} // namespace xsim

#endif // VARIANTCREATORDELIVERY_H
