#ifndef EVENTCREATEENTITY_H
#define EVENTCREATEENTITY_H

#include <xsim_config>
#include <string>
#include <vector>

#include "event.h"

namespace xsim {

class Source;

class XSIM_EXPORT EventCreateEntity : public Event {
 public:
     EventCreateEntity(Source *source, bool rescheduled,
             int priority = PRIORITY_CREATEMU);

     /* Documented in event.h */
     void process() override;
     std::string receiver() override;
     std::string name() override;

 private:
     Source *source_;

     /**
      * @brief True when this creation event is created to deal with
      * a previously failed creation atempt.
      */
     bool rescheduled_;
};

} // namespace xsim

#endif // EVENTCREATEENTITY_H
