#ifndef SAVEXML_H
#define SAVEXML_H

#include <xsim_config>
#include <string>
#include <vector>

#include "flow.h"
#include "component.h"
#include "pugixml.hpp"
#include "double.h"
#include "int.h"

namespace xsim {

class Assembly;
class Batch;
class MaxWip;
class Node;
class NodeResource;
class Facade;
class Kanban;
class Object;
class Order;
class Simulation;
class ShiftCalendar;
class Store;
class ResourceManager;
class NumberGenerator;
class NumberGeneratorBounded;
class Variant;
class VariantCreatorDelivery;

class SaveXml {
 public:
     SaveXml(Simulation *sim);

     std::string save();
     void save_file(const std::string& file);
private:
    void write_double_value(pugi::xml_node xml_node, const std::string& name, const Double& value);
    void write_int_value(pugi::xml_node xml_node, const std::string& name, int value);
    void write_int_value(pugi::xml_node xml_node, const std::string& name, const Int& value);
    void write_int_value(pugi::xml_node xml_node, const std::string& name, const std::string& value);
    void write_bool_value(pugi::xml_node xml_node, const std::string& name, bool value);
    void write_time_value(pugi::xml_node xml_node, const std::string& name, Double value);
    void write_time_value(pugi::xml_node xml_node, const std::string& name, double value);
    void write_time_value(pugi::xml_node xml_node, const std::string& name, const std::string& value);
    void write_bool_child(pugi::xml_node xml_node, const std::string& name, bool value);
    void write_double_child(pugi::xml_node xml_node, const std::string& name, double value);
    void write_double_child(pugi::xml_node xml_node, const std::string& name, Double value);
    void write_int_child(pugi::xml_node xml_node, const std::string& name, int value);
    void write_int_child(pugi::xml_node xml_node, const std::string& name, Int value);
    void write_value_child(pugi::xml_node xml_node, const std::string& name, const std::string& value);
    void write_string_attribute(pugi::xml_node xml_node, const std::string& name, const std::string& value);
    void write_bool_attribute(pugi::xml_node xml_node, const std::string& name, bool value);
    void write_int_attribute(pugi::xml_node xml_node, const std::string& name, int value);
    void write_double_attribute(pugi::xml_node xml_node, const std::string& name, double value);
    void write_user_attributes(pugi::xml_node xml_node, Object* object);
    void write_move_strategy(pugi::xml_node xml_node, Object* object);
    void write_distribution_value(pugi::xml_node xml_node,
        const std::string& name,
        const std::string& type,
        double value);
    void write_distribution_value(pugi::xml_node xml_node,
        const std::string& name,
        const std::string& type,
        const Double& value);
    void write_distribution_value(pugi::xml_node xml_node,
        const std::string& name,
        const std::string& type,
        const Int& value);

    void write_lower_upper_bound(pugi::xml_node xml_node, NumberGeneratorBounded* number_generator);
    void write_bool_cell(pugi::xml_node xml_node, int row, int col, bool value);
    void write_int_cell(pugi::xml_node xml_node, int row, int col, int value);
    void write_int_cell(pugi::xml_node xml_node, int row, int col, Int value);
    void write_double_cell(pugi::xml_node xml_node, int row, int col, double value);
    void write_double_cell(pugi::xml_node xml_node, int row, int col, Double value);
    void write_time_cell(pugi::xml_node xml_node, int row, int col, Double value);
    void write_empty_cell(pugi::xml_node xml_node, int row, int col);
    void write_shift_nodes(pugi::xml_node xml_node, ShiftCalendar* shift_calendar);
    void write_shifts(pugi::xml_node xml_node, ShiftCalendar* shift_calendar);
    void write_batch_demands(pugi::xml_node xml_node, Batch* batch);
    void write_dispatch(pugi::xml_node xml_node, Object* object);
    void write_empty_resource(pugi::xml_node xml_node, const std::string& type);
    void write_resource(pugi::xml_node xml_node, NodeResource* node_resource, const std::string& type);
    void write_resources(pugi::xml_node xml_node, Node* node);
    void write_node_attributes(pugi::xml_node xml_node, Object* object);
    void write_media(pugi::xml_node xml_node, const std::string& ns);
    void write_order_entrances(pugi::xml_node xml_node, Order* order);
    void write_order_exits(pugi::xml_node xml_node, Order* order);
    void write_resources(pugi::xml_node xml_node, ResourceManager* resource_manager);
    void write_node_successor_references(pugi::xml_node xml_node, const std::string& name, Object* object);
    void write_node_predecessor_references(pugi::xml_node xml_node, const std::string& name, Object* object);
    void write_flow(pugi::xml_node flows_node, Flow* flow);
    void write_flows(pugi::xml_node xml_node, Object* object);
    void write_variants(pugi::xml_node xml_node, Simulation* sim);
    void write_variables(pugi::xml_node xml_node, Simulation* sim);
    void write_skills(pugi::xml_node xml_node, Simulation* sim);
    void write_notes(pugi::xml_node xml_node, Object* object);
    void write_input_maps(pugi::xml_node xml_node);

    void write_model(pugi::xml_document& doc);
    void write_templates(pugi::xml_node model_node, Simulation* sim);
    void write_component(pugi::xml_node xml_node, Object* object);
    void write_nodes(pugi::xml_node xml_node, Object* root_object);
    void write_logics(pugi::xml_node xml_node, Object* root_object);
    void write_distribution(pugi::xml_node xml_node,
        NumberGenerator* number_generator,
        const std::string& type);
    void write_cell(pugi::xml_node xml_node,
        int row,
        int col,
        NumberGenerator* number_generator,
        const std::string& type);
    void write_cell(pugi::xml_node xml_node, int row, int col, const std::string& value);
    void write_variant_id_cell(pugi::xml_node node, int row, int col, Variant* variant);
    void write_force_assembly(pugi::xml_node xml_node, Assembly* assembly);
    void write_creation_variants(pugi::xml_node xml_node, Object* object);
    void write_number_generator(pugi::xml_node xml_node,
        NumberGenerator* number_generator,
        const std::string& name);
    void write_failures(pugi::xml_node xml_node, Object* object);
    void write_creation(pugi::xml_node xml_node, Object* object);
    void write_limit(pugi::xml_node xml_node,
        Int limit,
        NumberGenerator* start_generator,
        NumberGenerator* stop_generator);
    void write_response_time(pugi::xml_node xml_node, NumberGenerator* number_generator);
    void write_entry_timeout(pugi::xml_node xml_node, NumberGenerator* number_generator);
    void write_setup_time(pugi::xml_node xml_node, NumberGenerator* number_generator);
    void write_process_time(pugi::xml_node xml_node, NumberGenerator* number_generator);
    void write_delivery_table(pugi::xml_node xml_node, VariantCreatorDelivery* variant_creator);
    void write_kanban_limits_table(pugi::xml_node xml_node, Kanban* kanban);
    void write_max_wip_limits(pugi::xml_node xml_node, MaxWip* max_wip);
    void write_incomplete_timeout(pugi::xml_node xml_node, NumberGenerator* number_generator);
    void write_batch_sizes(pugi::xml_node xml_node, Batch* batch);
    void write_capacity_table(pugi::xml_node xml_node, Store* store);
    void write_order_table(pugi::xml_node xml_node, Facade* facade);
    void write_assembly_table(pugi::xml_node xml_node, Assembly* assembly);
    void write_timetable_references(pugi::xml_node xml_node, Node* node);

    template<typename T>
    void write_object_references(pugi::xml_node xml_node, Object* object, const std::string& type)
    {
        pugi::xml_node node;
        bool first = true;

        Component* component = object->get_parent_component();
        for (Flow* flow : component->polymorphic_objects<Flow>(true)) {
            for (const FlowConnection& flow_connection : flow->successors(object)) {
                if (flow_connection.from != object)
                    continue;
                if (typeid(*flow_connection.to).hash_code() == typeid(T).hash_code()) {
                    if (first) {
                        node = xml_node.append_child(type.c_str());
                        first = false;
                    }
                    pugi::xml_node reference_node = node.append_child("oc:reference");
                    write_string_attribute(reference_node, "idref", flow_connection.to->id());
                }
            }
            for (const FlowConnection& flow_connection : flow->predecessors(object)) {
                if (flow_connection.to != object)
                    continue;
                if (typeid(*flow_connection.from).hash_code() == typeid(T).hash_code()) {
                    if (first) {
                        node = xml_node.append_child(type.c_str());
                        first = false;
                    }
                    pugi::xml_node reference_node = node.append_child("oc:reference");
                    write_string_attribute(reference_node, "idref", flow_connection.from->id());
                }
            }
        }
    }

    pugi::xml_document doc_;
    Simulation* sim_;
};

} // namespace xsim

#endif // SAVEXML_H
