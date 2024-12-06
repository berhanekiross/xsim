#ifndef EVENTSHIFTCALENDARSTART_H
#define EVENTSHIFTCALENDARSTART_H

#include <xsim_config>
#include <vector>

#include "event.h"

namespace xsim {

class ShiftCalendar;

class XSIM_EXPORT EventShiftCalendarStart : public Event {
 public:
     EventShiftCalendarStart(ShiftCalendar *shift,
             int priority = PRIORITY_SHIFT_CALENDAR_START);

     /* Documented in event.h */
     void process() override;
     std::string sender() override;
     std::string receiver() override;
     std::string name() override;

 private:
     ShiftCalendar *shift_;
};

} // namespace xsim

#endif // EVENTSHIFTCALENDARSTART_H
