#ifndef LOGIC_H
#define LOGIC_H

#include <xsim_config>
#include <string>
#include <vector>
#include <functional>

#include "object.h"
#include "simulation.h"

namespace xsim {

class EnterPort;
class Entity;
class Simulation;
class Variant;

/**
 * @brief A logic node can alter the behaviour of the simulation, for example by
 * preventing entities from entering nodes.
 */
class XSIM_EXPORT Logic : public virtual Object {
 public:
     /**
      * @brief Constructor.
      */
     Logic();
    
     /**
      * @brief Destructor
      */
     virtual ~Logic() {}

     /**
      * @brief Initialized the logic, called at the beginning of the simulation.
      */
     void init() override;

     /**
      * @brief Save outputs from this logic, called at the end of the
      * simulation.
      */
     void finalize() override;

     /**
      * @brief Reset any statistics that have been collected.
      *
      * @param time The current simulation time.
      */
     void reset_stats() override;
};

} // namespace xsim

#endif // LOGIC_H

