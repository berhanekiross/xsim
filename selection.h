#ifndef SELECTION_H
#define SELECTION_H

#include <xsim_config>
#include <vector>

#include "object.h"

namespace xsim {

/** @brief Selection. */
class XSIM_EXPORT Selection : public Object {
 public:
     /** @brief Default constructor */
     Selection();

     /** @brief Destructor */
     ~Selection();

     /** @brief Prepare for simulation by enabling the active selection */
     void pre_simulation_init();

     /**
      * @brief Sets the active selection.
      *
      * @param  object The object that should be active.
      */
     void set_active(Object* object);

     /**
      * @brief Gets the active selection.
      *
      * @returns The active object, nullptr if no object is selected.
      */
     Object* active() const;

private:
    /** @brief The currently selected object. */
    Object* active_;
};

} // namespace xsim

#endif // SELECTION_H
