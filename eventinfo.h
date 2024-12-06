#ifndef EVENTINFO_H
#define EVENTINFO_H

#include <xsim_config>
#include <string>

namespace xsim  {

/**
 * @brief Stores basic information about an event.
 *
 * It is used instead of the regular events when returning the event list.
 */
class XSIM_EXPORT EventInfo {
 public:
     /**
      * @brief Constructor.
      *
      * @param type The event type.
      * @param time The time the event occured.
      * @param receiver The receiver of the event.
      * @param sender The sender of the event.
      * @param breakpoint True if a breakpoint is set on this event.
      * @param breakpoint_stopped True if the simulation was stopped on this event.
      */
     EventInfo(std::string type, 
               simtime time,
               std::string receiver,
               std::string sender,
               bool breakpoint,
               bool breakpoint_stopped)
         : type_(type),
           time_(time),
           receiver_(receiver),
           sender_(sender),
           breakpoint_(breakpoint),
           breakpoint_stopped_(breakpoint_stopped) {}

     std::string type() const { return type_; }
     simtime time() const { return time_; }
     std::string receiver() const { return receiver_; }
     std::string sender() const { return sender_; }
     bool breakpoint() const { return breakpoint_; }
     bool breakpoint_stopped() const { return breakpoint_stopped_; }

 private:
     std::string type_;
     simtime time_;
     std::string receiver_;
     std::string sender_;
     bool breakpoint_;
     bool breakpoint_stopped_;
};

} // namespace xsim

#endif // EVENTINFO_H
