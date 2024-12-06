#ifndef FLOW_H
#define FLOW_H

#include <xsim_config>
#include <vector>

#include "object.h"
#include "signal.hpp"

namespace xsim {

class Variant;
class Node;

/** @brief A helper struct for managing flow connections. */
struct FlowConnection {
    Object* from;
    Object* to;
    int weight;
    bool active;
};

/** @brief Flows. */
class XSIM_EXPORT Flow : public Object {
 public:
     Signal<void(Flow*, Node*, Node*, Variant*, int)> flow_link_added;

     /** @brief Default constructor */
     Flow();

     /** @brief Destructor */
     ~Flow();

     /** @brief Prepares for simulation by connecting nodes together. */
     virtual void pre_simulation_init();

     /**
      * @brief Adds a connection
      *
      * @param from   The object where the connection starts.
      * @param to     The object where the connection ends.
      * @param weight The weight of the connection.
      * @param active True if the connection is active.
      */
     void add_connection(Object* from, Object* to, int weight, bool active);

     /**
      * @brief Adds a variant to this flow.
      *
      * @param variant The variant to add.
      */
     void add_variant(Variant* variant);

     /**
      * @brief Set that this flow accepts any variant.
      *
      * @param  value True if this flow accepts any variant, otherwise false.
      */
     void set_any_variant(bool value);

     /**
      * @returns True it this flow accepts any variant.
      */
     bool any_variant() const;

     /**
      * @brief Adds an object to this flow.
      *
      * @param object The object that should be added.
      */
     void add_object(Object* object);

     /**
      * @returns All objects that follow this flow.
      */
     const std::vector<Object*> objects() const;

     /**
      * @brief Query if a variant follows this flow.
      *
      * @param variant The variant.
      *
      * @returns True if the variant follows this flow, otherwise false.
      */
     bool has_variant(Variant* variant) const;

     /**
      * @returns All flow connections in this flow.
      */
     const std::vector<FlowConnection>& connections() const;

     /**
      * @returns All variants that are part of this flow.
      */
     const std::vector<Variant*>& variants() const;

     /**
      * @brief Find all successors for a given object.
      *
      * @param object The object to find successors for.
      *
      * @returns All successors.
      */
     std::vector<FlowConnection> successors(Object* object) const;

     /**
      * @brief Find all predecessors for a given object.
      *
      * @param object The object to find predecessors for.
      *
      * @returns All predecessors.
      */
     std::vector<FlowConnection> predecessors(Object* object) const;

 private:
     /** @brief The connections of this flow */
     std::vector<FlowConnection> connections_;

     /** @brief The variants that follow this flow */
     std::vector<Variant*> variants_;

     /** @brief The objects that follow this flow */
     std::vector<Object*> objects_;

     /** @brief True if this flow accepts any variant */
     bool any_variant_;
};

} // namespace xsim

#endif // OBJECT_H
