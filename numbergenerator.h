#ifndef NUMBERGENERATOR_H
#define NUMBERGENERATOR_H

#include <xsim_config>

#include "object.h"
#include "double.h"

namespace xsim {

class Entity;
class Variant;

/**
 * @brief The base class for number generators.
 */
class XSIM_EXPORT NumberGenerator : public Object {
 public:

     /**
     * @brief Constructor.
     */
     NumberGenerator();

     /**
      * @brief Desctructor.
      */
     virtual ~NumberGenerator() = default;

     /**
     * @brief Clone the number generator.
     *
     * @return A clone of the number generator.
     */
     virtual NumberGenerator* clone() const = 0;

     /**
      * @brief Get the next time in the number generator.
      *
      * @return The next time in the number generator.
      */
     virtual double next();

     /**
      * @brief Get the next number from the number generator, with one entity as context.
      *
      * Override to provide entity specific numbers, defaults to next().
      *
      * @param entity The entity for which to get a number.
      *
      * @return The next time in the number generator.
      */
     virtual double next(Entity* entity);

     /**
      * @brief Get the next number from the number generator, with two entities as context.
      *
      * Override to provide entity specific numbers. Defaults to next() if the entities are
      * different, otherwise zero.
      *
      * @param to The entity to switch to.
      * @param from The entity to switch from.
      *
      * @return The next number.
      */
     virtual double next(Entity* to, Entity* from);

     /**
      * @brief Get the expected mean of the number generator.
      *
      * @return The expected mean of the number generator.
      */
     virtual Double mean() const = 0;

     /**
      * @brief Get the expected mean of the number generator.
      * Base class implementation returns general mean,
      * override to provide variant specific times.
      *
      * @param variant The variant
      *
      * @return The expected mean of the number generator.
      */
     virtual Double mean(Variant* variant) const;

     /**
      * @brief Get the expected mean of the number generator,
      * always 0 if there is no switch in variant.
      *
      * @param to The variant too switch to.
      * @param from The variant too switch from.
      *
      * @return The expected mean of the number generator.
      */
     virtual Double mean(Variant *to, Variant *from);

     /**
      * @brief Check if the number generator is deterministic.
      *
      * @return True if deterministic.
      */
     virtual bool is_deterministic() const = 0;

     /**
      * @brief Query if this number generator is always zero.
      *
      * @returns True if always zero, false if not.
      */
     bool is_always_zero() const;
};

} // namespace xsim

#endif // NUMBERGENERATOR_H
