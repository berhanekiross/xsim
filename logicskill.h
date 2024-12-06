#ifndef LOGICSKILL_H
#define LOGICSKILL_H

#include <xsim_config>
#include <string>

#include "simulation.h"

namespace xsim {

class LogicResource;

/**
 * @brief A resource has a certain set of skill that can be used by nodes.
 */
class XSIM_EXPORT LogicSkill {
 public:
     /**
      * @brief Constructor.
      *
      * @param id The id of the skill.
      * @param name The name of the skill.
      * @param execution_factor A factor that is multiplied to the time it takes
      * to carry out the skill.
      */
     LogicSkill(
            int id,
            std::string name,
            LogicResource *resource,
            double execution_factor);

     void reset();
     void release();

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

     double execution_factor() const;
     bool is_ready() const;

	 LogicResource* resource() const;

     //void set_resource(LogicResource *resource);
     //bool is_allocated() const;

 private:
     /**
      * @brief The name of the node.
      */
     std::string name_;

     /**
      * @brief The id of the node.
      */
     int id_;

     LogicResource *resource_;
     double execution_factor_;
};

} // namespace xsim

#endif // SKILL_H
