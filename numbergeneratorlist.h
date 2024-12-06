#ifndef NUMBERGENERATORLIST_H
#define NUMBERGENERATORLIST_H

#include <xsim_config>
#include <string>
#include <list>

#include "numbergenerator.h"

namespace xsim {

class Variant;

class XSIM_EXPORT NumberGeneratorList : public NumberGenerator {
 public:
     NumberGeneratorList(bool setup);
     NumberGeneratorList(const NumberGeneratorList& number_generator);
     virtual ~NumberGeneratorList();

     NumberGeneratorList* clone() const override;
     simtime next() override;
     void init() override;
     bool is_deterministic() const override { return true; }
     Double mean() const override;
     Double mean(Variant *variant) const override;
     Double mean(Variant *to, Variant *from) override;

     void set_base_number_generator(NumberGenerator *base_number_generator);
     void add_interval_list(std::list<double> intervals, int replication);
     const std::list< std::list<double> >& replications() const { return replications_; }
     bool setup() const { return setup_; }
     NumberGenerator* const base_number_generator() const { return base_number_generator_; }

 private:
     std::list< std::list<double> > replications_;
     std::list< std::list<double> >::const_iterator replication_it_;
     std::list<double>::const_iterator interval_it_;
     bool setup_;
     NumberGenerator *base_number_generator_;
};

} // namespace xsim

#endif // NUMBERGENERATORLIST_H
