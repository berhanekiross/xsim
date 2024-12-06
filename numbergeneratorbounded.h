#ifndef NUMBERGENERATORBOUNDED_H
#define NUMBERGENERATORBOUNDED_H

#include <xsim_config>

#include "numbergenerator.h"
#include "double.h"

namespace xsim {

class Variant;

class XSIM_EXPORT NumberGeneratorBounded : public NumberGenerator {
 public:

     /**
      * @brief Constructor
      *
      * @param  rng The random number generator.
      */
     NumberGeneratorBounded();

     /**
      * @brief Copy constructor
      *
      * @param  number_generator The number generator to copy.
      */
     NumberGeneratorBounded(const NumberGeneratorBounded& number_generator);

     /**
      * @param  value The value to set as the lower bound.
      */
     void set_lower(Double value);

     /**
      * @param  value The value to set as the upper bound.
      */
     void set_upper(Double value);

     /**
      * @returns The lower bound.
      */
     const Double& lower() const;

     /**
      * @returns The upper bound.
      */
     const Double& upper() const;

     /**
      * @returns True if it has a lower bound, false if not.
      */
     bool has_lower() const;

     /**
      * @returns True if it has a upper bound, false if not.
      */
     bool has_upper() const;

 private:
     /** @brief The lower bound */
     Double lower_;

     /** @brief The upper bound */
     Double upper_;

     /** @brief True if has lower bound, false if not */
     bool has_lower_;

     /** @brief True if has upper bound, false if not */
     bool has_upper_;
};

} // namespace xsim

#endif // NUMBERGENERATORBOUNDED_H
