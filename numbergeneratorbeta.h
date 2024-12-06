#ifndef NUMBERGENERATORBETA_H
#define NUMBERGENERATORBETA_H

#include <xsim_config>
#include <random>

#include "numbergenerator.h"

namespace xsim {

class Variant;

/**
 * @brief The beta distribution.
 *
 * Boost does not have a beta distribution in boost/random. Here it is
 * implemented with the gamma distribution.
 */
class XSIM_EXPORT NumberGeneratorBeta : public NumberGenerator {
 public:
     NumberGeneratorBeta(Double alpha,
                         Double beta,
                         Double min,
                         Double max);
     NumberGeneratorBeta(const NumberGeneratorBeta& number_generator);
     NumberGeneratorBeta* clone() const override;
     double next() override;
     Double mean() const override;
     bool is_deterministic() const override;
     Double alpha() const { return alpha_; };
     Double beta() const { return beta_; };
     Double min() const { return min_; };
     Double max() const { return max_; };

 private:
     /**
     * Return the next mean value, not considering lower and
     * upper bounds.
     */
     double beta_value();
     Double alpha_;
     Double beta_;
     Double min_;
     Double max_;
     std::gamma_distribution<> alpha_dist_;
     std::gamma_distribution<> beta_dist_;
};

} // namespace xsim

#endif // NUMBERGENERATORBETA_H
