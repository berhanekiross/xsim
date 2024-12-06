#ifndef EVENTBREAKBEGIN_H
#define EVENTBREAKBEGIN_H

#include <xsim_config>
#include <vector>

#include "event.h"

namespace xsim {

class ShiftCalendar;

class XSIM_EXPORT EventBreakBegin : public Event {
 public:
     EventBreakBegin(ShiftCalendar *shift, int priority = PRIORITY_BREAK);

     /* Documented in event.h */
     void process() override;
     std::string sender() override;
     std::string receiver() override;
     std::string name() override;

 private:
     ShiftCalendar *shift_;
};

} // namespace xsim

#endif // EVENTBREAKBEGIN_H
