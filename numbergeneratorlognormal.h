#ifndef NUMBERGENERATORLOGNORMAL_H
#define NUMBERGENERATORLOGNORMAL_H

#include <xsim_config>
#include <random>

#include "numbergeneratorbounded.h"

namespace xsim {

class Variant;

class XSIM_EXPORT NumberGeneratorLognormal : public NumberGeneratorBounded {
 public:
     NumberGeneratorLognormal(Double mean, Double sigma);
     NumberGeneratorLognormal(const NumberGeneratorLognormal& number_generator);

     NumberGeneratorLognormal* clone() const override;
     double next() override;
     Double mean() const override { return mean_; }
     bool is_deterministic() const override { return false; }
     Double sigma() const { return sigma_; }

 private:
     Double mean_;
     Double sigma_;
     std::lognormal_distribution<> distribution_;
};

} // namespace xsim

#endif // NUMBERGENERATORLOGNORMAL_H
