#ifndef NUMBERGENERATORSEQUENCE_H
#define NUMBERGENERATORSEQUENCE_H

#include <xsim_config>
#include <vector>

#include "numbergenerator.h"

namespace xsim {

class Simulation;
class Variant;

class XSIM_EXPORT NumberGeneratorSequence : public NumberGenerator {
 public:
     NumberGeneratorSequence();
     NumberGeneratorSequence(const NumberGeneratorSequence& number_generator);

     NumberGeneratorSequence* clone() const override;
     simtime next() override;
     void init() override;
     Double mean() const override;
     bool is_deterministic() const override;

     void add_to_sequence(simtime time);
     const std::vector<double>& sequence() const { return sequence_; }

 private:
     std::vector<double> sequence_;
     std::vector<double>::const_iterator sequence_it_;
};

} // namespace xsim

#endif // NUMBERGENERATORSEQUENCE_H
