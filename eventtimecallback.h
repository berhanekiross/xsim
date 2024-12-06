#ifndef EVENTTIMECALLBACK_H
#define EVENTTIMECALLBACK_H

#include <xsim_config>
#include <vector>
#include <functional>

#include "event.h"

namespace xsim {

class XSIM_EXPORT EventTimeCallback : public Event {
 public:
     EventTimeCallback(std::function<void (void)> callback,
             int priority = PRIORITY_TIMECALLBACK);

     /* Documented in event.h */
     void process() override;
     std::string sender() override;
     std::string receiver() override;
     std::string name() override;

 private:
     std::function<void (void)> callback_;
};

} // namespace xsim

#endif // EVENTTIMECALLBACK_H
