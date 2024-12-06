#ifndef NUMBERGENERATORTRIANGLE_H
#define NUMBERGENERATORTRIANGLE_H

#include <xsim_config>
#include <random>

#include "numbergenerator.h"
#include "double.h"

namespace xsim {

class Variant;

class XSIM_EXPORT NumberGeneratorTriangle : public NumberGenerator {
 public:
     NumberGeneratorTriangle(Double lower, Double mode, Double upper);
     NumberGeneratorTriangle(const NumberGeneratorTriangle& number_generator);

     NumberGeneratorTriangle* clone() const override;
     double next() override;
     Double mean() const override;
     bool is_deterministic() const override { return false; }

     Double lower() const { return lower_; }
     Double mode() const { return mode_; }
     Double upper() const { return upper_; }

 private:
     Double lower_;
     Double mode_;
     Double upper_;
     std::uniform_real_distribution<> distribution_;
};

} // namespace xsim

#endif // NUMBERGENERATORTRIANGLE_H
