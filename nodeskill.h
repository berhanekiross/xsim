#ifndef NODESKILL_H
#define NODESKILL_H

#include <xsim_config>
#include <string>

#include "simulation.h"

namespace xsim {

class LogicSkill;

/**
 * @brief A resource has a certain set of skill that can be used by nodes.
 */
class XSIM_EXPORT NodeSkill {
 public:
     /**
      * @brief Constructor.
      *
      * @param id The id of the skill.
      * @param name The name of the skill.
      * @param execution_factor A factor that is multiplied to the time it takes
      * to carry out the skill.
      */
     NodeSkill(int id, std::string name);

     void reset();

     /**
      * @brief Get the id of the skill.
      *
      * @return The id.
      */
     int id() const;
    
     /**
      * @brief Get the name of the skill.
      *
      * @return The name.
      */
     std::string name() const;

     void set_logic_skill(LogicSkill *skill);
     LogicSkill* logic_skill() const;
     //void release();

     bool is_allocated() const;
     bool is_allocated_and_ready() const;

 private:
     /**
      * @brief The name of the node.
      */
     std::string name_;

     /**
      * @brief The id of the node.
      */
     int id_;

     LogicSkill *skill_;
};

} // namespace xsim

#endif // NODESKILL_H
