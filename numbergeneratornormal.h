#ifndef NUMBERGENERATORNORMAL_H
#define NUMBERGENERATORNORMAL_H

#include <xsim_config>
#include <random>

#include "numbergeneratorbounded.h"

namespace xsim {

class Variant;

class XSIM_EXPORT NumberGeneratorNormal : public NumberGeneratorBounded {
 public:
     NumberGeneratorNormal(Double mean, Double sigma);
     NumberGeneratorNormal(const NumberGeneratorNormal& number_generator);

     NumberGeneratorNormal* clone() const override;
     double next() override;
     Double mean() const override { return mean_; }
     bool is_deterministic() const override { return false; }
     Double sigma() const;

 private:
     Double mean_;
     Double sigma_;
     std::normal_distribution<> distribution_;
};

} // namespace xsim

#endif // NUMBERGENERATORNORMAL_H
