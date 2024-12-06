#ifndef EVENTCREATEDEMAND_H
#define EVENTCREATEDEMAND_H

#include <xsim_config>
#include <vector>

#include "event.h"

namespace xsim {

class Demand;
class Variant;

class XSIM_EXPORT EventCreateDemand : public Event {
 public:
     EventCreateDemand(Demand *demand, int priority = PRIORITY_CREATEDEMAND);

     /* Documented in event.h */
     void process() override;
     std::string receiver() override;
     std::string name() override;

 private:
     Demand *demand_;
};

} // namespace xsim

#endif // EVENTCREATEDEMAND_H
