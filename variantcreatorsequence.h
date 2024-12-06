#ifndef VARIANTCREATORSEQUENCE_H
#define VARIANTCREATORSEQUENCE_H

#include <xsim_config>
#include <list>

#include "variantcreator.h"

namespace xsim {

class Variant;
class NumberGenerator;

struct VariantCreatorSequenceItem {
    Variant *variant;
    NumberGenerator *number_generator;
};

/**
 * @brief Create variant in a given sequence.
 */
class XSIM_EXPORT VariantCreatorSequence : public VariantCreator {
 public:
     /**
      * @brief Constructor.
      */
     VariantCreatorSequence();
     virtual ~VariantCreatorSequence();

     void init() override;

     /* Documented in variantcreator.h */
     Variant* create(bool complete_batch, bool &terminate) override;
     int remaining_in_batch() const override;
     int current_batch_size() const override;
     bool update_creation_time() const override;
     bool is_stochastic() const override;
     bool depleted() const override;

     /**
      * @brief Add a variant to the sequence.
      *
      * @param variant The variant to add.
      * @param amount Distribution used to set the size of the batch.
      */
     virtual void add_to_creation_sequence(Variant *variant, NumberGenerator* amount);

     /**
      * @returns The variant sequence items.
      */
     std::vector<VariantCreatorSequenceItem> creation_sequence() const;

     /**
      * @brief Set if variants should be created cyclically. That is, when all
      * variants in a sequence have been created it starts from the beginning.
      *
      * @param value True if variants should be created cyclically.
      */
     void set_cyclic(bool value);

     /**
      * @returns True if cyclic is enabled.
      */
     bool cyclic() const;

     /**
      * @brief Set if batching should be used. It is used to count how many
      * variants that remain in a batch. Where the batch is simply the size
      * specified when the variant was added.
      *
      * @param value True if batching should be used.
      */
     void set_batch(bool value);

     /**
      * @returns True if batch is enabled.
      */
     bool batch() const;

     /**
      * @brief Set if sequence batching should be used. Sequence batching
      * means that the entire sequence will be created at a single time
      * instance.
      *
      * @param value True if sequence batching should be used.
      */
     void set_sequence_batch(bool value);

     /**
      * @returns True if sequence batch is enabled.
      */
     bool sequence_batch() const;

 private:
     /**
      * @brief Helper struct.
      */
     struct VariantCreatorSequencePrivateItem {
         Variant *variant;
         int size;
         NumberGenerator *amount;
         void update_size();
     };

     /**
      * @brief All variants and the sequence they should be created.
      */
     std::list<VariantCreatorSequencePrivateItem> creation_sequence_;

     /**
      * @brief A iterator that points to the next variant that should be
      * created.
      */
     std::list<VariantCreatorSequencePrivateItem>::iterator sequence_it_;

     /**
      * @brief True if cyclic is used.
      */
     bool cyclic_;

     /**
      * @brief True if batching is used.
      */
     bool batch_;

     /**
      * @brief This keep track of how many variants that have been created of
      * each variant in a batch.
      */
     int count_;

     /**
      * @brief True if batching of entire sequence is used.
      */
     bool sequence_batch_;

};

} // namespace xsim

#endif // VARIANTCREATORSEQUENCE_H
