#ifndef EVENTENTRYTIMEOUT_H
#define EVENTENTRYTIMEOUT_H

#include <xsim_config>

#include "event.h"

namespace xsim {

class ParallelOperation;

class XSIM_EXPORT EventEntryTimeout : public Event {
 public:
     EventEntryTimeout(ParallelOperation *parallel_operation,
             int priority = PRIORITY_ENTRYTIMEOUT);

     /* Documented in event.h */
     void process() override;
     std::string sender() override;
     std::string receiver() override;
     std::string name() override;

 private:
     ParallelOperation *parallel_operation_;
};

}

#endif // EVENTENTRYTIMEOUT_H
