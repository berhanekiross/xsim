#ifndef NUMBERGENERATORWEIBULL_H
#define NUMBERGENERATORWEIBULL_H

#include <xsim_config>
#include <random>

#include "numbergeneratorbounded.h"

namespace xsim {

class Variant;

class XSIM_EXPORT NumberGeneratorWeibull : public NumberGeneratorBounded {
 public:
     NumberGeneratorWeibull(Double shape, Double scale);
     NumberGeneratorWeibull(const NumberGeneratorWeibull& number_generator);

     NumberGeneratorWeibull* clone() const override;
     double next() override;
     Double mean() const override { return mean_; }
     bool is_deterministic() const override { return false; }

     Double shape() const { return shape_; }
     Double scale() const { return scale_; }

 private:
     Double shape_;
     Double scale_;
     Double mean_;
     std::weibull_distribution<> distribution_;
};

} // namespace xsim

#endif // NUMBERGENERATORWEIBULL_H
