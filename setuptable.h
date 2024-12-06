#ifndef SETUPTABLE_H
#define SETUPTABLE_H

#include <xsim_config>
#include <map>

#include "numbergenerator.h"
#include "double.h"

namespace xsim {

class Entity;
class Variant;

class XSIM_EXPORT SetupTable : public NumberGenerator {
 public:
     typedef std::map< std::pair<Variant*, Variant*>, NumberGenerator*> SetupTableType;

     /**
      * @brief Constructor.
      *
      * @param  table    that maps a pair of variants (i.e. to -> from)
      *                  to a number generator. OBS! Not allowed to be empty.
      * @param  variants The variants.
      */
     SetupTable(
         const std::map< std::pair<Variant*, Variant*>, NumberGenerator*>& table,
         const std::vector<Variant*>& variants);
     SetupTable(const SetupTable& setup_table);

     virtual ~SetupTable();

     SetupTable* clone() const override;
     double next() override;
     double next(Entity *to, Entity *from) override;
     Double mean() const override;
     Double mean(Variant *to, Variant *from) override;
     bool is_deterministic() const override { return true; }
     SetupTableType clone_table() const;
     NumberGenerator* get_number_generator(Variant* to, Variant* from) const;
     const std::vector<Variant*>& variants() const;

 private:
     SetupTableType table_;
     std::vector<Variant*> variants_;
};

} // namespace xsim

#endif // SETUPTABLE_H
