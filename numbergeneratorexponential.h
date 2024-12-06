#ifndef NUMBERGENERATOREXPONENTIAL_H
#define NUMBERGENERATOREXPONENTIAL_H

#include <xsim_config>
#include <random>

#include "double.h"
#include "numbergeneratorbounded.h"

namespace xsim {

class Variant;

class XSIM_EXPORT NumberGeneratorExponential : public NumberGeneratorBounded {
 public:
     NumberGeneratorExponential(Double mean);
     NumberGeneratorExponential(const NumberGeneratorExponential& number_generator);

     NumberGeneratorExponential* clone() const override;
     double next() override;
     Double mean() const override;
     bool is_deterministic() const override { return false; }

 private:
     Double mean_;
     std::exponential_distribution<> distribution_;
};

} // namespace xsim

#endif // NUMBERGENERATOREXPONENTIAL_H
