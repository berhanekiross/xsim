#ifndef EVENTSHIFTEND_H
#define EVENTSHIFTEND_H

#include <xsim_config>
#include <vector>

#include "event.h"

namespace xsim {

class ShiftCalendar;

class XSIM_EXPORT EventShiftEnd : public Event {
 public:
     EventShiftEnd(ShiftCalendar *shift,
             int priority = PRIORITY_SHIFT);

     /* Documented in event.h */
     void process() override;
     std::string sender() override;
     std::string receiver() override;
     std::string name() override;

 private:
     ShiftCalendar *shift_;
};

} // namespace xsim

#endif // EVENTSHIFTEND_H
