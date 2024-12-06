#ifndef NUMBERGENERATORDISCRETEUNIFORM_H
#define NUMBERGENERATORDISCRETEUNIFORM_H

#include <xsim_config>
#include <random>

#include "numbergenerator.h"
#include "int.h"

namespace xsim {

class Variant;

class XSIM_EXPORT NumberGeneratorDiscreteUniform : public NumberGenerator {
 public:
     NumberGeneratorDiscreteUniform(Int lower, Int upper);
     NumberGeneratorDiscreteUniform(const NumberGeneratorDiscreteUniform& number_generator);

     NumberGeneratorDiscreteUniform* clone() const override;
     double next() override;
     Double mean() const override;
     bool is_deterministic() const override { return false; }
     Int lower() const { return lower_; }
     Int upper() const { return upper_; }

 private:
     Int lower_;
     Int upper_;
     std::uniform_int_distribution<> distribution_;
};

} // namespace xsim

#endif // NUMBERGENERATORDISCRETEUNIFORM_H
