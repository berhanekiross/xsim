#ifndef NUMBERGENERATORGAMMA_H
#define NUMBERGENERATORGAMMA_H

#include <xsim_config>
#include <random>

#include "numbergeneratorbounded.h"

namespace xsim {

class Variant;

class XSIM_EXPORT NumberGeneratorGamma : public NumberGeneratorBounded {
 public:
     NumberGeneratorGamma(Double shape, Double scale);
     NumberGeneratorGamma(const NumberGeneratorGamma& number_generator);

     NumberGeneratorGamma* clone() const override;
     double next() override;
     Double mean() const override;
     bool is_deterministic() const override { return false; }
     Double shape() const { return shape_; }
     Double scale() const { return scale_; }

 private:
     Double shape_;
     Double scale_;
     std::gamma_distribution<> distribution_;
};

} // namespace xsim

#endif // NUMBERGENERATORGAMMA_H
