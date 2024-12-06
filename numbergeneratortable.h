#ifndef NUMBERGENERATORTABLE_H
#define NUMBERGENERATORTABLE_H

#include <xsim_config>
#include <map>

#include "numbergenerator.h"

namespace xsim {

class Entity;
class Variant;

struct NumberGeneratorTableItem {
    Variant* variant;
    NumberGenerator* number_generator;
};

class XSIM_EXPORT NumberGeneratorTable : public NumberGenerator {
 public:
     NumberGeneratorTable();
     NumberGeneratorTable(const NumberGeneratorTable& number_generator);

     NumberGeneratorTable* clone() const override;
     double next() override;
     double next(Entity *entity) override;
     void init() override;
     Double mean() const override;
     Double mean(Variant *variant) const override;
     bool is_deterministic() const override { return true; }

     /**
      * @brief Adds number generator for a variant.
      *
      * @param variant          The variant.
      * @param number_generator The number generator.
      */
     void add(Variant* variant, NumberGenerator *number_generator);

     /**
      * @returns All number generator table items.
      */
     const std::vector<NumberGeneratorTableItem>& variants() const;

 private:
     std::vector<NumberGeneratorTableItem> variants_order_;
     typedef std::map<Variant*, NumberGenerator*> VariantTimeType;
     VariantTimeType variants_;
};

} // namespace xsim

#endif // NUMBERGENERATORTABLE_H
