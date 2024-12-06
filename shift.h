#ifndef SHIFT_H
#define SHIFT_H

#include <xsim_config>
#include <string>

#include "object.h"

namespace xsim {

/**
 * @brief Describes a shift object.
 */
class XSIM_EXPORT Shift : public Object {
 public:
     /**
      * @brief Constructor.
      *
      * @param name The name of the shift.
      * @param start The start time of the shift.
      * @param length The length of the shift.
      */
     Shift(std::string name, simtime start);

     void init() override;

     /**
      * @brief Add a break to the shift. The node will be in the paused state
      * during the break.
      *
      * @param start The start simulation time of the break.
      * @param length The length of the break.
      */
     void add_break(simtime start, simtime length);

     /**
      * @brief Check if this shift have any breaks.
      * 
      * @return True if the shift have one ore more breaks.
      */
     bool have_breaks() const;

     /**
      * @brief Get the simulation start time of the current break.
      *
      * @return The simulation start time of the current break.
      */
     simtime break_start() const;

     /**
      * @brief Get the length of the current break.
      *
      * @return The length of the current break.
      */
     simtime break_length() const;

     /**
      * @brief Advance to the next break, resets to the first break if there
      * are no more breaks.
      *
      * @return True if there were another break.
      */
     bool next_break();

 private:
     class Break {
      public:
          Break(simtime start, simtime length) : start_(start), length_(length) {}

          simtime start_;
          simtime length_;
     };

     /**
      * @brief The name of the shift.
      */
     std::string name_;

     /**
      * @brief The start of the shift.
      */
     simtime start_;

     /**
      * @brief All breaks in this shift calendar.
      */
     std::vector<Break> breaks_;

     /**
      * @brief An iterator pointing to the currently active break.
      */
     std::vector<Break>::iterator current_break_;
};

} // namespace xsim

#endif // SHIFT_H