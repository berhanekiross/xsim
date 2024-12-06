#ifndef CAPACITYLIMIT_H
#define CAPACITYLIMIT_H

#include <xsim_config>
#include <map>
#include <utility>

#include "object.h"

namespace xsim {

class Entity;
class Variant;

/**
 * @brief Specify capacity limits on entities.
 */
class XSIM_EXPORT CapacityLimit : public Object {
 public:
     /**
      * @brief Destructor.
      *
      */
     virtual ~CapacityLimit() {}

     /**
      * @brief Add an entity.
      *
      * @param entity The entity to add.
      */
     virtual void add_entity(Entity *entity) = 0;

     /**
      * @brief Remove a entity.
      *
      * @param entity The entity to remove.
      */
     virtual void remove_entity(Entity *entity) = 0;

     /**
      * @brief Check if the maximum number of entities have been reached.
      *
      * @param entity The entity to check against.
      *
      * @return True if the capacity limit is at full capacity.
      */
     virtual bool is_full(Entity *entity) const = 0;

     /**
      * @brief Get the demand for a particular variant.
      *
      * @param variant The variant to get the demand for.
      *
      * @return The demand.
      */
     virtual unsigned int get_demand(Variant *variant) const { return 0; }
};

} // namespace xsim

#endif // CAPACITYLIMIT_H
