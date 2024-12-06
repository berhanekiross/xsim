#ifndef NUMBERGENERATOREPT_H
#define NUMBERGENERATOREPT_H

#include <xsim_config>
#include <random>

#include "numbergenerator.h"

namespace xsim {

class Entity;
class Variant;

class XSIM_EXPORT NumberGeneratorEpt : public NumberGenerator {
public:

     NumberGeneratorEpt(NumberGenerator *number_generator,
                        double low_l,
                        double high_l);
     /**
     * @brief Copy constructor.
     */
     NumberGeneratorEpt(const NumberGeneratorEpt& number_generator);

     NumberGeneratorEpt* clone() const override;
     simtime next() override;
     simtime next(Entity *entity) override;
     simtime next(Entity *to, Entity *from) override;
     Double mean() const override;
     Double mean(Variant *variant) const override;
     Double mean(Variant *to, Variant *from) override;
     bool is_deterministic() const override { return false; }

     NumberGenerator* const number_generator() const { return number_generator_; }
     double low_l() const { return low_l_; }
     double high_l() const { return high_l_; }

 private:
     simtime next(double time);
     NumberGenerator *number_generator_;
     std::normal_distribution<> distribution_;

     const double shape1;
     const double shape2;
     double low_l_;
     double high_l_;
};

} // namespace xsim

#endif // NUMBERGENERATOREPT_H
