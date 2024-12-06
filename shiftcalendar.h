#ifndef SHIFTCALENDAR_H
#define SHIFTCALENDAR_H

#include <xsim_config>
#include <list>
#include <string>
#include <vector>

#include "object.h"

namespace xsim {

class BreakItem {
public:
    std::string start;
    std::string end;
};

class ShiftCalendarItem {
public:
    std::string name;
    double start;
    double end;
    bool monday;
    bool tuesday;
    bool wednesday;
    bool thursday;
    bool friday;
    bool saturday;
    bool sunday;

    /**
     * @brief Adds a break
     *
     * @param  start The start of the break.
     * @param  end   The end of the break.
     */
    void add_break(const std::string& start, const std::string& end)
    {
        breaks.push_back({ start, end });
    }

    std::vector<BreakItem> breaks;
};

class Event;
class Node;
class Shift;
class Simulation;

/**
 * @brief Controls the time a node is available.
 */
class XSIM_EXPORT ShiftCalendar : public Object {
 public:
     /**
      * @brief Constructor.
      */
     ShiftCalendar();

     /** @brief Destructor */
     ~ShiftCalendar();

     /** @brief Prepare for simulation by creating shift objects */
     virtual void pre_simulation_init();

     void init() override;

     /**
      * @brief Add a node that should follow this shift.
      *
      * @param node The node to add.
      */
     void add_node(Node *node);

     /**
      * @brief Removes a node from the shift calendar.
      *
      * @param  node The node to remove.
      */
     void remove_node(Node *node);

     /** @brief Removes all nodes from the shift calendar. */
     void clear_nodes();

     /**
      * @returns The nodes that follow this shift calendar.
      */
     const std::vector<Node*>& nodes() const;

     /**
      * @brief Add a shift.
      *
      * @param name The name of the shift.
      * @param start The simulation start time of the shift.
      * @param end The simulation end time of the shift.
      * @param monday True if the shift should be active on monday.
      * @param tuesday True if the shift should be active on tuesday.
      * @param wednesday True if the shift should be active on wednesday.
      * @param thursday True if the shift should be active on thursday.
      * @param friday True if the shift should be active on friday.
      * @param saturday True if the shift should be active on saturday.
      * @param sunday True if the shift should be active on sunday.
      *
      * @return The new shift.
      */
     ShiftCalendarItem* add_shift(std::string name, simtime start, simtime end, bool monday,
             bool tuesday, bool wednesday, bool thursday, bool friday,
             bool saturday, bool sunday);

     /**
      * @returns All shift items.
      */
     const std::vector<ShiftCalendarItem*>& shifts() const;

     /**
      * @brief Schedule a new shift.
      */
     void schedule_shift();    

     /**
      * @brief Perform actions at the start of the shift.
      */
     void shift_begin();

     /**
      * @brief Perform actions at the end of the shift.
      */
     void shift_end();

     /**
      * @brief Perform actions at the start of the break.
      */
     void break_begin();

     /**
      * @brief Perform actions at the end of the break.
      */
     void break_end();

 private:
     class ShiftItem {
      public:
          Shift *shift;
          simtime start;
          simtime length;
     };
     /**
      * @brief Insert a new shifts.
      *
      * @param start The start simulation time the shift should start.
      * @param end The end simulation time of the shift.
      */
     void insert_shift(Shift *shift, simtime start, simtime length);
     
     /**
      * @brief Calculates how many seconds there to the next shift start.
      * Normalized to one week.
      *
      * @param next_start The start of the next shift, it is normalized to one
      * week.
      *
      * @return How many seconds to the next shift start.
      */
     simtime seconds_to_next_start(simtime next_start);

     /**
      * @brief All nodes that follow this shift.
      */
     std::vector<Node*> nodes_;

     /**
      * @brief All shift items in this shift calendar.
      */
     std::vector<ShiftItem> shift_items_;

     /**
      * @brief All shifts in this shift calendar.
      */
     std::vector<ShiftCalendarItem*> shifts_;

     /**
      * @brief An iterator pointing to the currently active shift item.
      */
     std::vector<ShiftItem>::iterator current_shift_;

     Event *shift_end_event_;

     /**
      * @brief The time in seconds from the start date to the preceding
      * Monday midnight.
      */
     double offset_from_monday_midnight_;

     /** @brief True if the current shift is unplanned */
     bool unplanned_;

     /** @brief True if the current shift is paused */
     bool paused_;
};

} // namespace xsim

#endif // SHIFTCALENDAR_H
