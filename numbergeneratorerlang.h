#ifndef NUMBERGENERATORLOGERLANG_H
#define NUMBERGENERATORLOGERLANG_H

#include <xsim_config>
#include <random>

#include "numbergeneratorbounded.h"
#include "double.h"
#include "int.h"

namespace xsim {

class Variant;

class XSIM_EXPORT NumberGeneratorErlang : public NumberGeneratorBounded {
 public:
     NumberGeneratorErlang(Int shape, Double scale);
     NumberGeneratorErlang(const NumberGeneratorErlang& number_generator);

     NumberGeneratorErlang* clone() const override;
     double next() override;
     Double mean() const override;
     bool is_deterministic() const override { return false; }
     Int shape() const { return shape_; }
     Double scale() const { return scale_ ; }

 private:
     Int shape_;
     Double scale_;
     std::gamma_distribution<> distribution_;
};

} // namespace xsim

#endif // NUMBERGENERATORLOGERLANG_H
