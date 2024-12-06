#ifndef VARIANT_H
#define VARIANT_H

#include <xsim_config>
#include <string>
#include <vector>

#include "object.h"
#include "signal.hpp"

namespace xsim {

class Node;
class Entity;
class Simulation;

/**
 * @brief Base class for entities.
 */
class XSIM_EXPORT Variant : public Object {
 public:
     /**
      * @brief Constructor.
      *
      * @param sim The simulation manager.
      * @param name The name of the variant.
      * @param id A unique identifier.
      * @param length The physical length, in mm, of the variant.
      * @param width The physical width, in mm, of the variant.
      * @param units Specifies the number of units this part represents.
      */
     Variant(int length, int width, unsigned int units);

     /**
      * @brief Destructor
      */
     virtual ~Variant();

     /**
      * @brief Create a entity.
      *
      * @param node The node at which the entity is created.
      *
      * @return A new entity.
      */
     Entity* create(Node *node);

     /**
      * @brief Reset the variant so a new simulation can begin.
      */
     void init() override;

     /**
      * @brief Save outputs from this variant, called at the end of the
      * simulation.
      */
     virtual void post_finalize();

     /**
      * @brief Reset any statistics that have been collected.
      */
     virtual void post_reset_stats();

     /*
      * @brief Get the area of the variant.
      *
      * @return The area of the variant.
      */
     double area() const;

     /*
      * @brief Get all exit replication values.
      *
      * @return Exit replications.
      */
     std::vector<double> exit_replications() const;

     /**
      * @brief Get all cycle time replication values.
      *
      * @return Cycle time replications.
      */
     std::vector<double> cycle_time_replications() const;

     /**
      * @brief Get all throughput replication values.
      *
      * @return Throughput replications.
      */
     std::vector<double> throughput_replications() const;

     /**
      * @brief Get all wip replication values.
      *
      * @return Work in process replications.
      */
     std::vector<double> wip_replications() const;

     /**
      * @brief Add time this variant have contributed to the work in process.
      *
      * @param time The amount to add.
      */
     void add_wip_time(double time);

     /**
      * @brief Record that a entity of this variant left the model.
      *
      * @param time The simulation time the entity left.
      */
     void add_exit(double time);

     /**
      * @brief Get how many entities of this variant that have left the
      * model.
      *
      * @return The number of exits of this variant.
      */
     unsigned int exits() const;

     /**
      * @brief Get the cycle time of this variant. The cycle time is the total
      * amount of time any entity of this variant have been in the model.
      *
      * @return The cycle time.
      */
     double cycle_time() const;

     /**
      * @brief Get the throughput of this variant.
      *
      * @return The throughput.
      */
     double throughput() const;

     /**
      * @brief Get the work in process of this variant.
      *
      * @return The work in process.
      */
     double wip() const;

     /**
      * @brief Get the width of the variant.
      *
      * @return The width.
      */
     int width() const;

     /**
      * @brief Get the length of the variant.
      *
      * @return The length.
      */
     int length() const;

     /**
      * @brief Getter for the amount of units.
      *
      * @return The amount of units.
      */
     unsigned int units() const;

 private:
     /**
      * @brief The length of the variant.
      */
     unsigned int length_;

     /**
      * @brief The width of the variant.
      */
     unsigned int width_;

     /* Values from all replications. */
     std::vector<double> exit_replications_;
     std::vector<double> cycle_time_replications_;
     std::vector<double> throughput_replications_;
     std::vector<double> wip_replications_;

     /**
      * @brief The next entity id.
      */
     unsigned int entity_id_;

     /**
      * @brief How many exits there have been.
      */
     unsigned int exits_;

     /**
      * @brief The total amount time spent in the model by all entities of
      * this variant.
      */
     double cycle_time_;

     /**
      * @brief The total amount of time any entity of this variant have
      * contributed to the work in process in any node.
      */
     double wip_time_;

     /**
      * @brief The number of units each entity of this variant starts
      * with.
      */
     unsigned int units_;
};

} // namespace xsim

#endif // VARIANT_H
