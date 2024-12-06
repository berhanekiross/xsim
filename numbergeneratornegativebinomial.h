#ifndef NUMBERGENERATORNEGATIVEBINOMIAL_H
#define NUMBERGENERATORNEGATIVEBINOMIAL_H

#include <xsim_config>
#include <random>

#include "numbergenerator.h"
#include "int.h"
#include "double.h"

namespace xsim {

class Variant;

class XSIM_EXPORT NumberGeneratorNegativeBinomial : public NumberGenerator {
 public:

     /**
     * OBS! Boost definition differs from the default Wikipedia definition
     *
     * Boost: X ~ NB(r,p)
     *
     * r – Fixed number of successes
     * p – Probability of success
     * X ~ Random variable of number failures for reaching the
     *      fixed number of successes
     *
     * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     *
     * Wikipedia (Default definition): X ~NB(r,p)
     *
     * r – Fixed number of failures
     * p – Probability of success
     * X ~ Random variable of number of success for reaching the
     *      fixed number of failures
     *
     **/
     NumberGeneratorNegativeBinomial(Int r, Double p);
     NumberGeneratorNegativeBinomial(const NumberGeneratorNegativeBinomial& number_generator);

     NumberGeneratorNegativeBinomial* clone() const override;
     double next() override;
     Double mean() const override;
     bool is_deterministic() const override { return false; }

     Int r() const { return r_; }
     Double p() const { return p_; }

 private:
     Int r_;
     // Probability of failure
     Double p_;
     std::negative_binomial_distribution<> distribution_;
};

} // namespace xsim

#endif // NUMBERGENERATORNEGATIVEBINOMIAL_H
