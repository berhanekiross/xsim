#ifndef NUMBERGENERATORUNIFORM_H
#define NUMBERGENERATORUNIFORM_H

#include <xsim_config>
#include <random>

#include "numbergenerator.h"
#include "double.h"

namespace xsim {

class Variant;

class XSIM_EXPORT NumberGeneratorUniform : public NumberGenerator {
 public:
     NumberGeneratorUniform(Double lower, Double upper);
     NumberGeneratorUniform(const NumberGeneratorUniform& number_generator);

     NumberGeneratorUniform* clone() const override;
     double next() override;
     Double mean() const override;
     bool is_deterministic() const override { return false; }

     Double lower() const { return lower_; }
     Double upper() const { return upper_; }

 private:
     Double lower_;
     Double upper_;
     std::uniform_real_distribution<> distribution_;
};

} // namespace xsim

#endif // NUMBERGENERATORUNIFORM_H
