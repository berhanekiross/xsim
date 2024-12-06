#ifndef EVENTTAKTCOMPLETE_H
#define EVENTTAKTCOMPLETE_H

#include <xsim_config>
#include <vector>

#include "event.h"

namespace xsim {

class Takt;

class XSIM_EXPORT EventTaktComplete : public Event {
 public:
     EventTaktComplete(Takt *takt,
             int priority = PRIORITY_TAKTCOMPLETE);

     /* Documented in event.h */
     void process() override;
     std::string sender() override;
     std::string receiver() override;
     std::string name() override;

 private:
     Takt *takt_;
};

}

#endif // EVENTTAKTCOMPLETE_H
