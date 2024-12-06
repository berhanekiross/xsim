#ifndef BREAKPOINT_H
#define BREAKPOINT_H

#include <xsim_config>
#include <string>

namespace xsim {

/**
* @brief Base class for events.
*/
class XSIM_EXPORT Breakpoint {
 public:
     /**
      * @brief Constructor.
      */
     Breakpoint() : has_type_(false), has_start_(false), has_stop_(false),
        has_receiver_(false), has_sender_(false) {}
     
     /**
      * @brief Set the type of event to break on.
      *
      * @param type The type of the breakpoint.
      */
     void set_type(std::string type) { type_ = type; has_type_ = true; }

     /**
      * @brief Check if the type is set.
      *
      * @return True if set.
      */
     bool has_type() const { return has_type_; }

     /**
      * @brief Get the type of event to break on.
      *
      * @return The type of the breakpoint.
      */
     std::string type() const { return type_; }

     /**
      * @brief Clear the type.
      */
     void clear_type() { has_type_ = false; }

     /**
      * @brief Set the start time to break on.
      *
      * @param time The start time of the breakpoint.
      */
     void set_start(simtime time) { start_ = time; has_start_ = true; }

     /**
      * @brief Check if the start time is set.
      *
      * @return True if set.
      */
     bool has_start() const { return has_start_; }

     /**
      * @brief Get the start time to break on.
      *
      * @return The start time.
      */
     simtime start() const { return start_; }

     /**
      * @brief Clear start.
      */
     void clear_start() { has_start_ = false; }

     /**
      * @brief Set the stop time to break on.
      *
      * @param time The stop time of the breakpoint.
      */
     void set_stop(simtime time) { stop_ = time; has_stop_ = true; }

     /**
      * @brief Check if the start time is set.
      *
      * @return True if set.
      */
     bool has_stop() const { return has_stop_; }

     /**
      * @brief Get the end time to break on.
      *
      * @return The end time.
      */
     simtime stop() const { return stop_; }

     /**
      * @brief Clear stop.
      */
     void clear_stop() { has_stop_ = false; }

     /**
      * @brief Set the receiver to break on.
      *
      * @param time The receiver of the breakpoint.
      */
     void set_receiver(std::string receiver) { receiver_ = receiver; has_receiver_ = true; }

     /**
      * @brief Check if the receiver is set.
      *
      * @return True if set.
      */
     bool has_receiver() const { return has_receiver_; }

     /**
      * @brief Get the receiver to break on.
      *
      * @return The receiver.
      */
     std::string receiver() const { return receiver_; }

     /**
      * @brief Clear receiver.
      */
     void clear_receiver() { has_receiver_ = false; }

     /**
      * @brief Set the sender to break on.
      *
      * @param time The sender of the breakpoint.
      */
     void set_sender(std::string sender) { sender_ = sender; has_sender_ = true; }

     /**
      * @brief Check if the sender is set.
      *
      * @return True if set.
      */
     bool has_sender() const { return has_sender_; }

     /**
      * @brief Get the sender to break on.
      *
      * @return The sender.
      */
     std::string sender() const { return sender_; }

     /**
      * @brief Clear sender.
      */
     void clear_sender() { has_sender_ = false; }

 private:
     /**
      * @brief The type to break on.
      */
     std::string type_;
     bool has_type_;

     /**
      * @brief The start time to break on.
      */
     simtime start_;
     bool has_start_;

     /**
      * @brief The stop time to break on.
      */
     simtime stop_;
     bool has_stop_;

     /**
      * @brief The receiver to break on.
      */
     std::string receiver_;
     bool has_receiver_;

     /**
      * @brief The sender to break on.
      */
     std::string sender_;
     bool has_sender_;
};

} // namespace xsim

#endif // BREAKPOINT_H
