#ifndef NUMBERGENERATORCONST_H
#define NUMBERGENERATORCONST_H

#include <xsim_config>

#include "double.h"
#include "numbergenerator.h"

namespace xsim {

class Variant;

class XSIM_EXPORT NumberGeneratorConst : public NumberGenerator {
 public:
     NumberGeneratorConst(Double time);
     /**
     * @brief Copy constructor.
     */
     NumberGeneratorConst(const NumberGeneratorConst& number_generator);

     NumberGeneratorConst* clone() const override;
     double next() override;
     Double mean() const override;
     bool is_deterministic() const override;

     const Double& time() const { return time_; };

 private:
     Double time_;
};

} // namespace xsim

#endif // NUMBERGENERATORCONST_H
