#ifndef XML_H
#define XML_H

#include <limits>
#include <list>
#include <map>
#include <string>
#include <regex>
#include <boost/variant.hpp>

#include "pugixml.hpp"
#include "muParser.h"
#include "common.h"
#include "component.h"
#include "flow.h"
#include "double.h"
#include "int.h"
#include "propertycontainer.h"
#include "prioritysignal.h"
#include "simulation.h"
#include "failure.h"
#include "xsim-llvm.h"

namespace xsim {

class Assembly;
class AssemblySpecification;
class Batch;
class Buffer;
class MaxWip;
class CapacityLimit;
class CriticalWip;
class Conveyor;
class Demand;
class Disassembly;
class Logic;
class Node;
class Facade;
class FailureZone;
class Kanban;
class Object;
class Operation;
class Order;
class ParallelOperation;
class ResourceManager;
class Sink;
class Source;
class Selection;
class ShiftCalendar;
class Store;
class Takt;
class ResourceManager;
class NumberGenerator;
class NumberGeneratorBounded;
class NumberGeneratorSequence;
class NumberGeneratorTable;
class Variant;
class VariantCreator;
class VariantCreatorDelivery;
class VariantCreatorRandom;
class VariantCreatorSequence;
class SetupTable;

typedef boost::variant<Operation*, Source*, Sink*, Buffer*, Store*, Assembly*, Disassembly*, ParallelOperation*, Facade*, Conveyor*, FailureZone*, Demand*, ResourceManager*, Batch*> all_types;


class Xml {
 public:
     Xml();
     ~Xml();

     void load(std::string xml);
     void load_from_file(std::string file);
     void init(std::string xml);
     std::string write_output();
     void write_output(std::string file);
     void write_output(pugi::xml_document &doc);

     std::string start() const;
     double warmup();
     double horizion();
     int replications();
     std::chrono::system_clock::time_point start_date() const;
     std::string bottleneck_detection() const;

private:
     void create_model();

     void create_nodes(
         pugi::xml_node root,
         Component* root_component,
         Object* object,
         std::string path);

     void create_notes(pugi::xml_node component_node, Component* component);

     void create_logics(
         pugi::xml_node root,
         Component* component,
         std::string path);

     void create_sources(
             pugi::xml_node root,
             Component* component,
             std::string path);

     void create_operations(
             pugi::xml_node root,
             Component* component,
             std::string path);

     void create_parallel_operations(
             pugi::xml_node root,
             Component* component,
            std::string path);

     void create_assemblies(
             pugi::xml_node root,
             Component* component,
             std::string path);

     void create_disassemblies(
             pugi::xml_node root,
             Component* component,
             std::string path);

     void create_sinks(
             pugi::xml_node root,
             Component* component,
             std::string path);

     void create_buffers(
             pugi::xml_node root,
             Component* component,
             std::string path);

     void create_conveyors(
             pugi::xml_node root,
             Component* component,
             std::string path);

     void create_stores(
             pugi::xml_node root,
             Component* component,
             std::string path);

     void create_facades(
             pugi::xml_node root,
             Component* component,
             std::string path);

     void create_links(
            pugi::xml_node flowNode,
            Component* component,
            Object* object);

     void create_shift_links(pugi::xml_node component_node, Component* component);
     void create_demand_links(pugi::xml_node component_node, Component* component);
     void create_maxwip_links(pugi::xml_node component_node, Component* component);
     void create_criticalwip_links(pugi::xml_node component_node, Component* component);
     void create_batch_links(pugi::xml_node component_node, Component* component);
     void create_order_links(pugi::xml_node component_node, Component* component);
     void create_takt_links(pugi::xml_node component_node, Component* component);
     void create_resource_links(pugi::xml_node component_node, Component* component);
     void create_kanban_links(pugi::xml_node component_node, Component* component);
     void create_failure_zone_links(pugi::xml_node root, Component* component);

     void create_selection(
             pugi::xml_node selectionNode,
             Component* root_component,
             Selection* selection,
             std::string path);

     ShiftCalendar* create_shifts(pugi::xml_node timetable_node);

     MaxWip* create_max_wip(pugi::xml_node root, std::string path);
     Order* create_order(pugi::xml_node root, std::string path);
     FailureZone* create_failure_zone(pugi::xml_node root, std::string path);
     ResourceManager* create_resource( pugi::xml_node root, std::string path);

     void create_component(
             pugi::xml_node component_node,
             Component* root_component,
             Component* component,
             std::string path);

     void create_entrances(
             pugi::xml_node component_node,
             Component* component);

     void create_exits(
             pugi::xml_node component_node,
             Component* component);

     Operation* create_operation(
             pugi::xml_node operation_node,
             std::string path);

     ParallelOperation* create_parallel_operation(
            pugi::xml_node operation_node,
            std::string path);

     Buffer* create_buffer(
             pugi::xml_node buffer_node,
             std::string path);

     Conveyor* create_conveyor(
             pugi::xml_node conveyor_node,
             std::string path);

     Source* create_source(
             pugi::xml_node source_node,
             std::string path);

     Sink* create_sink(
             pugi::xml_node sink_node,
             std::string path);

     Store* create_store(
             pugi::xml_node store_node,
             std::string path);

     Facade* create_facade(
             pugi::xml_node facade_node,
             std::string path);

     Assembly* create_assembly(
             pugi::xml_node assembly_node,
             std::string path);

     Disassembly* create_disassembly(
             pugi::xml_node disassembly_node,
             std::string path);

     CriticalWip* create_critical_wip(
             pugi::xml_node critical_wip_node,
             std::string path);

     Demand* create_demand(
             pugi::xml_node demand_node,
             std::string path);

     Batch* create_batch(
             pugi::xml_node batch_node,
             std::string path);

     Takt* create_takt(
             pugi::xml_node takt_node,
             std::string path);

     Kanban* create_kanban(
             pugi::xml_node takt_node,
             std::string path);

     void create_variables(pugi::xml_node node);

     VariantCreatorDelivery* create_variant_creator_delivery(pugi::xml_node node, all_types object);

     VariantCreatorSequence* create_variant_creator_sequence(pugi::xml_node node, all_types object);

     VariantCreatorRandom* create_variant_creator_random(pugi::xml_node node);

     NumberGeneratorTable* create_number_generator_table(pugi::xml_node node, all_types object);

     CapacityLimit* create_capacity_limit(pugi::xml_node node);

     void add_kanban_limits(pugi::xml_node node, Kanban* kanban);

     void add_max_wip_limits(pugi::xml_node node, MaxWip *max_wip, int capacity);

     AssemblySpecification* create_assembly_specification(pugi::xml_node node);

     void create_batch_table(pugi::xml_node node, Batch *batch);

     void set_user_attributes(Object* object, pugi::xml_node xml_node);

     NumberGenerator* create_start_generator(pugi::xml_node node);

     VariantCreator* create_variant_creator(pugi::xml_node node, all_types object);

     NumberGenerator* create_number_generator(pugi::xml_node node, all_types object);

     NumberGenerator* create_setup_number_generator(pugi::xml_node node, all_types object);

     void set_front_exit_callback(pugi::xml_node& xml_node, Node* node);

     void set_rear_exit_callback(pugi::xml_node& xml_node, Node* node);

     void set_front_enter_callback(pugi::xml_node& xml_node, Node* node);

     template <typename T>
     void connect_priority_signal(PrioritySignal<T>& signal, pugi::xpath_node_set nodes)
     {
         for (auto xpath_node : nodes) {
             auto root_node = xpath_node.node();
             auto priority_attr = root_node.attribute("priority");
             int priority = 0;
             if (priority_attr)
                 priority = priority_attr.as_int();

             if (strcmp(root_node.first_child().name(), "c:function") == 0) {
                 auto node = root_node.first_child();
                 std::string function_name = std::regex_replace(node.attribute("name").value(), std::regex(".*:"), "");
                 std::string function_id = node.attribute("id").value();
                 std::string name = node.parent().parent().attribute("name").value();

                 std::stringstream function_path;
                 function_path << "function_" << function_id << "::" << "create_" << function_name;
                 auto f = sim()->jit()->get_function_ptr<T>(function_path.str());
                 if (!f) {
                     std::stringstream ss;
                     ss << name;
                     ss << ": Cannot create function: ";
                     ss << function_name << std::endl;
                     llvm::handleAllErrors(f.takeError(), [&](const llvm::ErrorInfoBase& EI) {
                         std::string str;
                         llvm::raw_string_ostream os(str);
                         EI.log(os);
                         os.flush();
                         ss << str;
                         });
                     throw std::runtime_error(ss.str());
                 }
                 signal.connect(*(f.get()), priority);
             }
         }
    }


    // template <typename T>
    // void connect_signal1(Signal<T>& signal, pugi::xpath_node_set nodes)
    // {
    //     for (auto xpath_node : nodes) {
    //         if (strcmp(xpath_node.node().first_child().name(), "c:function") == 0) {
    //             auto node = xpath_node.node().first_child();
    //             std::string function_name = std::regex_replace(node.attribute("name").value(), std::regex(".*:"), "");
    //             std::string function_id = node.attribute("id").value();
    //             std::string name = node.parent().parent().attribute("name").value();

    //             std::stringstream function_path;
    //             function_path << "function_" << function_id << "::" << "connect_" << function_name;
    //             auto f = sim()->jit()->get_function<T>(function_path.str());
    //             if (!f) {
    //                 std::stringstream ss;
    //                 ss << name;
    //                 ss << ": Cannot create function: ";
    //                 ss << function_name << std::endl;
    //                 llvm::handleAllErrors(f.takeError(), [&](const llvm::ErrorInfoBase& EI) {
    //                     std::string str;
    //                     llvm::raw_string_ostream os(str);
    //                     EI.log(os);
    //                     os.flush();
    //                     ss << str;
    //                     });
    //                 throw std::runtime_error(ss.str());
    //             }
    //             f.get()
    //             //signal.connect(*(f.get()));
    //         }
    //     }
    //}

     template <typename T>
     void load_user_attributes(T* node, pugi::xml_node root_node)
     {
         pugi::xpath_node_set attr_nodes = root_node.select_nodes("oc:user_attributes/oc:user_attribute");
         for (pugi::xpath_node xpath_node : attr_nodes) {
             pugi::xml_node xml_node = xpath_node.node();
             std::string name = xml_node.attribute("name").value();
             std::string data_type = xml_node.attribute("data_type").value();
             if (data_type == "real") {
                 Double value = get_double(xml_node.select_node("oc:value").node());
                 node->properties()->set(name, static_cast<double>(value));
             } else if (data_type == "integer") {
                 Int value = get_int(xml_node.select_node("oc:value").node());
                 node->properties()->set(name, static_cast<int>(value));
             }
         }
     }

	 void create_modules(pugi::xml_node root);

	 void compile_functions(pugi::xml_node root);
     void connect_simulation_signals(pugi::xml_node root);

     void create_function(
            std::stringstream &code,
            const std::string &name,
            const std::string &namespace_name,
            const std::string &return_type,
            const std::string &arguments,
            bool is_class,
            bool is_signal,
            const pugi::xpath_node_set &params,
            bool decl_only);

     std::vector<std::string> create_functions(pugi::xml_node root, std::stringstream &code, bool decl_only);

     NumberGenerator* create_ept(pugi::xml_node node, NumberGenerator *number_generator);

     void set_dispatch_rule(Node *node, pugi::xml_node xml_node);

     FailureTypes get_failure_type(pugi::xml_node node);

     FailureTimeReference get_failure_time_reference(pugi::xml_node node);

     std::string get_value_string(pugi::xml_node node);

     Double get_value_time(pugi::xml_node node);

     Int get_int(pugi::xml_node node);

     Double get_double(pugi::xml_node node);

     bool get_value_bool(pugi::xml_node node);

     void set_bounds(pugi::xml_node node, NumberGeneratorBounded* number_generator);

     NumberGenerator* get_distribution_time(pugi::xml_node node, all_types object);

     SetupTable* create_setup_table(pugi::xml_node node, all_types object);

     void set_move_strategy(pugi::xml_node xml_node, Node *node);

     void write_bottleneck(pugi::xml_node node);

     pugi::xml_node write_object_output(Object *object, const std::string &path_prefix, pugi::xml_node root_node);
     pugi::xml_node write_logic_output(Logic *logic, const std::string &path_prefix, pugi::xml_node root_node);

     pugi::xml_node write_output(pugi::xml_node node,
             std::string name, std::vector<double> replications);

     bool is_link_active(std::string from_id, std::string to_id) const;

     void read_table(
            pugi::xml_node node,
            std::vector< std::vector<pugi::xml_node> > &table,
            size_t &num_rows,
            size_t &num_columns);

     template<typename T>
     void read_failures(pugi::xml_node xml_node, T* node)
     {
         pugi::xpath_node_set disturbance_nodes = xml_node.select_nodes("oc:disturbances/oc:disturbance");
         for (pugi::xpath_node xpath_node : disturbance_nodes) {
             pugi::xml_node disturbance_node = xpath_node.node();
             std::string failure_name = disturbance_node.first_child().attribute("name").value();
             FailureTypes type = get_failure_type(disturbance_node.first_child());
             FailureTimeReference reference = get_failure_time_reference(disturbance_node.first_child());
             std::unique_ptr<Failure> failure = std::make_unique<Failure>(failure_name, node, type, reference);

             if (type == PERCENT) {
                 pugi::xml_node availability_node = disturbance_node.first_child().select_node("oc:availability").node();
                 Double availability = get_double(availability_node.first_child());

                 pugi::xml_node mttr_node = disturbance_node.first_child().select_node("oc:mttr").node();
                 Double mttr = get_value_time(mttr_node.first_child());

                 failure->set_availability_mttr(availability, mttr);
             }
             else if (type == DISTRIBUTIONS || type == CYCLES) {
                 pugi::xml_node failure_duration_node = disturbance_node.first_child().select_node("oc:duration").node();
                 failure->set_duration(create_number_generator(failure_duration_node, node));

                 pugi::xml_node failure_interval_node = disturbance_node.first_child().select_node("oc:interval").node();
                 failure->set_interval(create_number_generator(failure_interval_node, node));
             }

             node->add_failure(failure.release());
         }
     }

     //void read_failures(pugi::xml_node xml_node, Node* node, all_types object);

     Variant* find_variant_by_id(const std::string& id) const;

     pugi::xml_document doc_;
     mu::Parser parser_;
     mu::value_type *vars_;
     std::map<std::string, bool> nodes_with_resource_callback_;
};

} // namespace xsim

#endif // XML_H
