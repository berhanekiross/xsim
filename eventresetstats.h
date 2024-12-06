#ifndef EVENTRESETSTATS_H
#define EVENTRESETSTATS_H

#include <xsim_config>
#include <vector>

#include "event.h"

namespace xsim {

class Simulation;

class XSIM_EXPORT EventResetStats : public Event {
 public:
     EventResetStats(Simulation *sim,
             int priority = PRIORITY_RESET_STATS);

     /* Documented in event.h */
     void process() override;
     std::string sender() override;
     std::string receiver() override;
     std::string name() override;

 private:
     Simulation *sim_;
};

} // namespace xsim

#endif // EVENTRESETSTATS_H
