#ifndef NUMBERGENERATORBINOMIAL_H
#define NUMBERGENERATORBINOMIAL_H

#include <xsim_config>
#include <random>

#include "numbergenerator.h"
#include "double.h"
#include "int.h"

namespace xsim {

class Variant;

class XSIM_EXPORT NumberGeneratorBinomial : public NumberGenerator {
 public:
     NumberGeneratorBinomial(Int n, Double p);
     NumberGeneratorBinomial(const NumberGeneratorBinomial& number_generator);

     NumberGeneratorBinomial* clone() const override;
     double next() override;
     Double mean() const override;
     bool is_deterministic() const override { return false; }
     Int n() const { return n_; }
     Double p() const { return p_; }

 private:
     Int n_;
     Double p_;
     std::binomial_distribution<> distribution_;
};

} // namespace xsim

#endif // NUMBERGENERATORBINOMIAL_H
