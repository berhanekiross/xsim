#ifndef FLOWSELECTION_H
#define FLOWSELECTION_H

#include <xsim_config>
#include <vector>

#include "object.h"

namespace xsim {

/** @brief A flow selection. */
class XSIM_EXPORT FlowSelection : public Object {
 public:
     /** @brief Default constructor */
     FlowSelection();

     /** @brief Destructor */
     ~FlowSelection();

     /** @brief Prepare for simulation by enabling the active flow */
     virtual void pre_simulation_init();

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

#endif // FLOWSELECTION_H
