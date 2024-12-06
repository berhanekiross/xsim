#ifndef SIMULATION_H
#define SIMULATION_H

#include <xsim_config>
#include <chrono>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <typeinfo>
#include <sstream>
#include <random>

#include "date/date.h"
#include "date/iso_week.h"

#include "common.h"
#include "component.h"
#include "eventinfo.h"
#include "object.h"
#include "prioritysignal.h"
#include "poolallocator.h"

#pragma warning(disable : 4996)

namespace xsim {

struct Variable {
    std::string name;
    std::string id;
    std::string data_type;
    double value_;

    double* value_ptr()
    {
        return &value_;
    }

    int as_int() const
    {
        return static_cast<int>(std::round(value_));
    }

    double as_double() const
    {
        return value_;
    }

    std::string to_string() const
    {
        std::stringstream ss;
        ss << value_;
        return ss.str();
    }
};

class ActivePeriod;
class Assembly;
class Batch;
class Buffer;
class Breakpoint;
class CriticalWip;
class Conveyor;
class Demand;
class ExitPort;
class Event;
class Disassembly;
class EventTimeCallback;
class Facade;
class Kanban;
class FailureZone;
class LogBuffer;
class MaxWip;
class Entity;
class Node;
class Order;
class Operation;
class ParallelOperation;
class Takt;
class NumberGenerator;
class ShiftCalendar;
class Sink;
class XSimLLVM;
class Source;
class Store;
class Variant;
class Skill;
class ResourceManager;

typedef std::mt19937 RandomGenerator;

/**
 * @brief The simulation manager.
 *
 * Creates and owns all simulation objects and performs the simulation of the
 * events.
 */
class XSIM_EXPORT Simulation {
 typedef std::function<void (void)> TimeCallback;

 public:
     typedef std::pair<Node*, Entity*> EntityLocation;

     PrioritySignal<void()> simulation_initialized;
     PrioritySignal<void()> simulation_finalized;
     PrioritySignal<void()> initialized;
     PrioritySignal<void()> finalized;
     PrioritySignal<void()> statistics_reset;
     Signal<void (const std::string &)> log_changed;

     /**
      * @brief Deleted copy constructor
      */
     Simulation(Simulation const&) = delete;

     /**
      * @brief Deleted assignment operator
      */
     void operator=(Simulation const&) = delete;

     /** @brief Destructor */
     ~Simulation();

     /** @brief Creates the Simulation instance */
     static void create_instance(const std::string& source_dir, const std::string& lib_dir, const std::string& build_dir);

     /** @brief Destroys the Simulation instance */
     static void destroy_instance();

     /** @returns The simulation instance. */
     static Simulation* instance();

     /** @brief Clears the simulation to its blank/initial state */
     void clear();

     /**
      * @brief Sets the seed to the random number generator
      *
      * @param  seed The seed.
      */
     void set_seed(int seed);

     /**
      * @brief Attaches the given @p data to the simulation input.
      *
      * This function should only be used to transfer user data into the simulation instance. It
      * will not update the input user data stored in the database. Instead use @ref
      * set_user_data_output to store user data for later use.
      *
      * @param data The data to save.
      */
     void set_user_data_input(const std::string& data);

     /**
      * @return The user data set with @ref set_user_data_input.
      */
     std::string user_data_input() const;

     /**
      * @brief Attaches the given @p data to the simulation output.
      *
      * Stores any user data in the database for later use.
      *
      * @param data The data to save.
      */
     void set_user_data_output(const std::string& data);

     /**
      * @param replication The replication index, zero based, to get user data for.
      *
      * @return The user data set with @ref set_user_data_output.
      */
     std::string user_data_output(int replication) const;

     /** @brief Initialize the simulation. */
     void simulation_init();

     /** @brief Finalize the simulation. */
     void simulation_finalize();

     /**
      * @brief Initialize the simulation objects at the start of a simulation replication.
      *        
      *        This function must be called before 'simulate' or the first call to 'simulate_step'.
      *        It's should not be called before 'run'.
      */
     void init();

     /** @brief Finalize the simulation objects at the end of a simulation replication. */
     void finalize();

     /**
      * @brief Sets the start time point of the simulation.
      *
      * @param  start_date The new start time point of the simulation.
      */
     void set_start_time_point(std::chrono::system_clock::time_point start_date);

     /**
      * @returns The current simulation time as a time point.
      */
     std::chrono::system_clock::time_point time_point_now() const;

     /**
      * @returns The year, month and day of the current simulation time.
      */
     date::year_month_day year_month_day() const;

     /**
      * @returns The year and month of the current simulation time.
      */
     date::year_month year_month() const;

     /**
      * @returns The number of days since the simulation start.
      */
     date::sys_days days() const;

     /**
      * @returns The year of the current simulation time.
      */
     date::year year() const;

     /**
      * @returns The month of the current simulation time.
      */
     date::month month() const;

     /**
      * @returns The day of the current simulation time.
      */
     date::day day() const;

     /**
      * @returns The week number of the current simulation time.
      */
     iso_week::weeknum week() const;

     /**
      * @returns The day of week of the current simulation time.
      */
     date::weekday day_of_week() const;

     /**
      * @returns The time of day, in seconds, of the current simulation time.
      */
     std::chrono::duration<float> time_of_day() const;

     /**
      * @brief Get the current simulation time.
      *
      * @return The current simulation time.
      */
     simtime now() const;

     /**
      * @brief Get the elapsed time with the warm-up period removed.
      *
      * @returns The elapsed time.
      */
     simtime elapsed_time() const;

     /**
      * @brief Schedules an event for execution.
      *
      * @param evt The event that should be executed.
      * @param time The time when the event should be executed, relative to the
      * current simulation time.
      */
     void schedule(Event *evt, simtime time);

     /**
      * @brief Schedule an event directly before another event.
      *
      * @param evt The event that should be exectuted.
      * @param insert The event before which the new event should be inserted.
      * @param time The time when the event should be executed, relative to the
      * current simulation time.
      */
     void schedule_before(Event *evt, Event *insert, simtime time);

     /**
      * @brief Schedules an event for immediate execution.
      *
      * @param evt The event that should be executed.
      */
     void schedule_now(Event *evt);

     /**
      * @brief Remove event from event list.
      *
      * @param evt The event to remove.
      */
     void remove_event(Event *evt);

     /**
      * @returns  The event that is currently being processed.
      */
     const Event* current_event() const;

     /**
      * @brief Executes one simulation event.
      *
      * @return The state of simulation.
      */
     int simulate_step();

     /**
      * @brief Reset statistics collected during the warmup period.
      */
     void reset_stats();

     /**
      * @brief Get the event list.
      *
      * @return The current event list.
      */
     std::vector<EventInfo> event_list();

     /**
      * @brief Get the next event to be processed.
      *
      * @return The current event.
      */
     EventInfo next_event();

     /**
      * @brief Loads a model from an xml string.
      *
      * @param xml The xml string with the model to load.
      */
     void load_from_string(std::string xml_str);

     /**
      * @brief Loads a model from an xml file.
      *
      * @param filename The xml file containing the model to load.
      */
     void load_from_file(std::string filename);

     void init_simulation(std::string filename);

     /**
      * @brief Saves the model to a xml file.
      *
      * @param  filename The xml file where the model will be saved.
      */
     void save_model(const std::string& filename);

     /**
      * @brief Saves the model to a xml formatted string
      *
      * @returns A xml formatted string.
      */
     std::string save_model();

     /**
      * @brief Set warmup period.
      *
      * @param time The warmup period in seconds.
      */
     void set_warmup(simtime time);

     /**
      * @brief Set simulation horizon.
      *
      * @param time The simulation horizon in seconds.
      */
     void set_horizon(simtime time);

     /**
      * @brief Set the number of simulation replications.
      *
      * @param replications The number of replications.
      */
     void set_replications(unsigned int replications);

     /**
      * @brief Starts the simulation and writes the results to a file.
      *
      * @param filename The file that the output (xml) of the simulation is
      * written to.
      */
     void run(std::string filename);

     /**
      * @brief Starts the simulation and returns the results as a string.
      *
      * @return An xml string containing the results of the simulation.
      */
     std::string run();

     /**
      * @brief Adds a template
      *
      * @param component The template component to add.
      */
     void add_template(Component* component);

     /**
      * @brief Adds a variable.
      *
      * @param  name      The name of the variable.
      * @param  id        The identifier.
      * @param  data_type Type of the data.
      * @param  value     The value.
      */
     void add_variable(
            const std::string& name,
            const std::string& id,
            const std::string& data_type,
            const std::string& value);

     /**
      * @returns All variables.
      */
     const std::vector<Variable*>& variables() const;

     /**
      * @brief Gets a variable.
      *
      * @param  id The identifier of the variable.
      *
      * @returns The variable if found, otherwise nullptr.
      */
     Variable* get_variable(const std::string& id) const;

     /**
      * @returns All template components.
      */
     const std::vector<Component*>& templates() const;

     /**
      * @brief Adds a skill to the model.
      *
      * @param name The name of the skill.
      */
     void add_skill(std::string name);

     /**
      * @returns All skills.
      */
     const std::vector<std::string>& skills() const;

     /**
      * @brief Add a callback that gets called at a specified simulation time.
      *
      * @param time The simulation time when the callback should occur.
      * @param callback The function to call.
      * @param priority The priority of the event.
      *
      * @return The new event.
      */
     EventTimeCallback* add_time_callback(simtime time,
                                          std::function<void (void)> function,
                                          int priority = PRIORITY_TIMECALLBACK);

     /**
      * @brief Add a breakpoint. Breakpoints are only active when single
      * stepping.
      *
      * @return The new breakpoint.
      */
     Breakpoint* add_breakpoint();

     /**
      * @brief Delete a breakpoint.
      *
      * @param breakpoint The breakpoint to delete.
      */
     void delete_breakpoint(Breakpoint *breakpoint);

     /**
      * @brief Get a breakpoint.
      *
      * @param index The index of the breakpoint to get.
      *
      * @return The breakpoint at the given index.
      */
     Breakpoint* get_breakpoint(int index) const;

     /**
      * @brief Check and set breakpoints to all events in the event list.
      */
     void update_breakpoints();

     /**
      * @brief Toggle breakpoint on event in the event list.
      *
      * @param index The index of the event to the toggle the breakpoint on.
      * @param value True turns the breakpoint on.
      */
     void set_breakpoint(int index, bool value);

     /**
      * @brief Toggle breakpoints on or off.
      *
      * @param value True turns on breakpoints.
      */
     void set_breakpoints_enabled(bool value);

     /**
      * @brief Set event that should stop the simulation. Works only for single
      * stepping.
      *
      * @param index The index to the event in the event list to stop on.
      */
     void set_stop_event(int index);

      /**
      * @brief Clear the stop event.
      */
     void clear_stop_event();

     /**
      * @brief Get the random number generator used by the simulation.
      *
      * @return The random number generator.
      */
     RandomGenerator& random_generator() const;

     /**
      * @brief Get the number of simulation replications.
      *
      * @return The number of replications.
      */
     unsigned int replications() const;

     /**
      * @return The current, zero based, replication index.
      */
     unsigned int replication() const;

     /**
      * @brief Get the warmup period.
      *
      * @return The warmup period in seconds.
      */
     simtime warmup() const;

     /**
      * @brief Get the horizon.
      *
      * @return The horizon in seconds.
      */
     simtime horizon() const;

     /**
      * @brief Cancels the simulation.
      *
      * Cancels the simulation before the next event when using 'simulate_step'
      * and after the simulation is finished when using 'simulate', canceling
      * any remaining replications.
      */
     void cancel_simulation();

     /**
      * @brief Check if the simulation has been canceled.
      *
      * @return True if the simulation is canceled.
      */
     bool simulation_canceled() const;

     /**
      * @brief Add a new active period.
      *
      * Active periods are used to calculate shifting bottlenecks.
      *
      * @param node The node that the active period refers to.
      * @param start The start time of the active period.
      * @param end The end time of the active period.
      * @param open_end True if the active period have not yet ended.
      */
     void add_active_period(Node* node, double start, double end, bool open_end);

     /**
      * @brief Check if the shifting bottleneck detection is on.
      *
      * @return True if it is on.
      */
     bool shifting_bottleneck_detection() const;

     /**
      * @brief Turns shifting bottleneck detection on or off.
      *
      * @param value True if it should be used.
      */
     void set_shifting_bottleneck_detection(bool value);

     /**
      * @brief Calculates the shifting bottlenecks.
      *
      * @param remove_periods If true active periods are removed from the
      * active period list when used. This must be used if this function is
      * called multiple times within one simulation.
      *
      * @note Set 'remove_periods' to false if there is no need for online
      * bottleneck statistics, i.e. if it is only run once at the end of the
      * simulation, because it much faster that way.
      */
     void calculate_shifting_bottlenecks(bool remove_periods = true);

     /**
      * @brief Get the node that is determined to be the bottleneck.
      *
      * @return The node that is the bottleneck.
      */
     Node* bottleneck();

     /**
      * @brief Get the skill id with a given name.
      *
      * @return If found it returns the skill id, otherwise -1.
      */
     int get_skill_id(std::string name) const;

     /**
      * @brief Get the root component.
      *
      * @returns The root component.
      */
     Component* root_component() const;

     /**
      * @brief Gets the next batch identifier
      *
      * The batch id is guaranteed to be unique.
      *
      * @returns The next batch identifier.
      */
     unsigned int get_next_batch_id();

     /**
      * @brief Gets all objects that are of a specific type.
      *        
      * @note This will return all objects that can be cast to T. So a call to
      * "polymorphic_objects<Node>()" will return all objects that are of type Node.
      *
      * @tparam T The type to search for.
      * @param recursive True to search recursively, false to search the root component only.
      * @param include_templates True to include templates in the search, false to exclude them.
      *
      * @return Objects of type T.
      */
     template<typename T>
     std::vector<T*> polymorphic_objects(bool recursive = true,
                                         bool include_templates = false) const
     {
         if (!root_component_)
             return std::vector<T*>();

         std::vector<T*> objects = root_component_->polymorphic_objects<T>(recursive);

         if (include_templates) {
             for (Component* component : templates()) {
                 std::vector<T*> template_objects = component->polymorphic_objects<T>(recursive);
                 objects.insert(objects.end(), template_objects.begin(), template_objects.end());
             }
         }

         return objects;
     }

     /**
      * @brief Gets all objects that are of a specified class.
      *
      * @note Will only return objects that are of the exact same class.
      *
      * @tparam T The class type that should be returned.
      *
      * @return Objects of type T.
      */
     template<typename T>
     std::vector<T*> class_objects(bool recursive = true) const
     {
         return root_component_->class_objects<T>(recursive);
     }

     /**
      * @brief Gets all objects of a specific object type.
      *
      * This function only returns objects that have the exact same object
      * type.
      *
      * @param  type The object type that should be returned.
      *
      * @returns All objects that matches the specified object type.
      */
     std::vector<Object*> type_objects(const std::string& type) const;

     /**
      * @brief Searches for an object with a specified id.
      *
      * @tparam T The class to object should be cast to.
      * @param  id                The identifier.
      * @param  recursive         True to search recursively, false to search the root component only.
      * @param  include_templates True to include templates in the search, false to exclude them.
      *
      * @returns The object cast to the specified type, or nullptr if the object is not found or it is
      *          of the wrong type.
      */
     template <typename T = Object>
     T* find_object_by_id(const std::string & id,
                          bool recursive = true,
                          bool include_templates = false) const
     {
         T* object = root_component_->find_object_by_id<T>(id, recursive);

         if (!object && include_templates) {
             for (Component* component : templates()) {
                 object = component->find_object_by_id<T>(id, recursive);
                 if (object)
                     break;
             }
         }

         return object;
     }

     /**
      * @brief Searches for an object with a specified name.
      *
      * @tparam T The class to object should be cast to.
      * @param  name              The name.
      * @param  recursive         True to search recursively, false to search the root component only.
      * @param  include_templates True to include templates in the search, false to exclude them.
      *
      * @returns The object cast to the specified type, or nullptr if the object is not found or it is
      *          of the wrong type.
      */
     template <typename T = Object>
     T* find_object_by_name(const std::string& name,
                          bool recursive = true,
                          bool include_templates = false) const
     {
         T* object = root_component_->find_object_by_name<T>(name, recursive);

         if (!object && include_templates) {
             for (Component* component : templates()) {
                 object = component->find_object_by_name<T>(name, recursive);
                 if (object)
                     break;
             }
         }

         return object;
     }

     /**
      * @brief Returns the location (node) where each entity currently is.
      *
      * @return All entities and their respective locations.
      */
     std::vector<EntityLocation> entity_locations() const;

     /**
      * @brief Get the average cycle time of all variants.
      *
      * @return The cycle time.
      */
     simtime cycle_time() const;

     /**
      * @brief Get the average work in process of all nodes.
      *
      * @return The average work in process.
      */
     double wip() const;

     /**
      * @brief Get the throughput of the model.
      *
      * @return The throughput.
      */
     double throughput() const;

     /**
      * @brief Get the total number of entities that have exited the
      * model.
      *
      * @return Total exits.
      */
     unsigned int total_exits() const;

     /**
      * @brief Get all throughput values from each replication.
      *
      * @return Throughput replications.
      */
     std::vector<double> throughput_replications() const;

     /**
      * @brief Get all work in process values from each replication.
      *
      * @return Work in process replications.
      */
     std::vector<double> wip_replications() const;

     /**
      * @brief Get all cycle time values from each replication.
      *
      * @return Cycle time replications.
      */
     std::vector<double> cycle_time_replications() const;

     /**
      * @brief Get all total exits from each replication.
      *
      * @return Total exits replications.
      */
     std::vector<double> total_exits_replications() const;

     /**
      * @brief Facilitates communication between the simulation and external receivers.
      *
      * Write data to the ostream with the '<<' operator and end by writing 'std::endl'. The
      * 'std::endl' will flush the ostream and trigger the log_changed signal that external
      * receivers can subscribe to.
      *
      * @return The log output stream.
      */
     std::ostream& log() const;

     /**
      * @brief Add a simulation output.
      *
      * Each @p name should have one @p value for each simulation replication.
      *
      * @param name The name of the output.
      * @param value The value of the output.
      */
     void add_output(const std::string &name, double value);

     /**
      * @return All simulation output names and their respective values.
      */
     const std::map<std::string, std::vector<double>>& get_output_replications() const;

     XSimLLVM* jit() const;

	 void save_module_file(const std::string &module_name, const std::string &code);
     void save_internal_file(const std::string &module_name, const std::string &code);
     void parse_modules(const std::string &code);
	 void submit_code(std::string text, std::string name);
     const std::vector<std::string>& get_modules() const;
     const std::vector<std::string>& get_internal_files() const;

     /**
      * @brief Gets the start time of the simulation
      *
      * @returns The start time of the simulation.
      */
     std::chrono::system_clock::time_point start_time_point() const;

     /**
      * @brief Gets the the small size allocator
      *
      * @returns A reference to a PoolAllocator.
      */
     PoolAllocator<128>& allocator() { return allocator_; }

 private:
     /** @brief Private default constructor */
     Simulation() = delete;

     Simulation(const std::string & source_dir, const std::string & lib_dir, const std::string & build_dir);
    

     /** @brief The Simulation instance */
     inline static Simulation* instance_ = nullptr;

     /** @brief Creates the root component */
     void create_root_component();

     /**
      * @brief A log buffer that stores the intermediate data written the log stream.
      */
     LogBuffer *log_buf_;

     /**
      * @brief The output stream used for logging simulation messages.
      */
     std::ostream *log_stream_;

     /**
      * @brief Starts the simulation.
      *
      * @param time The simulation stop time.
      * @param replications The number of replications to perform.
      */
     void simulate(simtime time, int replications = 1);

     /**
      * @brief Insert an event.
      *
      * Time of the event is implicitly decided by the start event, whenever
      * that event is scheduled to happen that is the time the insert event
      * will also have. The inserted event can also not be scheduled before
      * the start event.
      *
      * @param start The event after which the new event should be inserted.
      * @param insert The event that should be inserted.
      */
     void insert_event(Event *start, Event *insert);

     /**
      * @brief Insert an event directly after another event.
      *
      * Time of the event is implicitly decided by the first event, whenever
      * that event is scheduled to happen that is the time the insert event
      * will also have.
      *
      * @param evt The event after which the new event should be inserted.
      * @param insert The event that should be inserted.
      */
     void insert_event_after(Event *evt, Event *insert);

     /**
      * @brief Free all events from the event list.
      */
     void free_events();

     /**
      * @brief Process the event list until there are no more events or the
      * simulation stop time is reached.
      *
      * @param time The simulation stop time.
      */
     void simulate_events(simtime time);

     /**
      * @brief Check if event matches any breakpoints.
      *
      * @param evt The event to set breakpoint on.
      */
     void check_breakpoints(Event *evt);

     /**
      * @brief Saves a event to a file.
      *
      * @param evt The event to save.
      */
     void trace(Event *evt, std::ofstream &trace_file);

     /**
      * @brief A global counter to keep all batch ids unique.
      */
     unsigned int batch_id_;

     /**
      * @brief Store different statistics for each replication.
      */
     std::vector<double> total_exits_replications_;
     std::vector<double> throughput_replications_;
     std::vector<simtime> cycle_time_replications_;
     std::vector<double> wip_replications_;
     std::map<std::string, std::vector<double>> output_replications_;

     /**
      * @brief The simulation horizon that is read from the xml file.
      */
     simtime horizon_;

     /**
      * @brief The simulation warmup that is read from the xml file.
      */
     simtime warmup_;

     /**
      * @brief The number of replications that is read from the xml file.
      */
     unsigned int replications_;

     /**
      * @brief The current replication index.
      */
     unsigned int replication_;

     /**
      * @brief The random generator used to generate all random numbers. This
      * is the only allowed generator, no other generator should be used
      * throughout the entire simulation.
      */
     RandomGenerator *rng_;

     /**
      * @brief Set to true to cancel the simulation.
      */
     bool simulation_canceled_;

     /**
      * @brief The current simulation time.
      */
     simtime simclock_;

     /**
      * @brief Set to true if shifting bottleneck detection should be used.
      */
     bool shifting_bottleneck_detection_;

     /**
      * @brief All active periods stored be per node.
      */
     std::map<Node*, std::vector<ActivePeriod*>> active_periods_;

     /**
      * @brief The event queue.
      */
     //EventQueue events_;
     std::map<double, Event*> events_;

     Event *current_event_;

     /**
      * @brief The root component that contains all objects.
      */
     Component *root_component_;

     /**
      * @brief All managed nodes.
      */
     std::vector<std::string> skill_ids_;
     std::vector<Component*> templates_;
     std::vector<Variable*> variables_;

     /**
      * @brief All breakpoints.
      */
     std::list<Breakpoint*> breakpoints_;

     /**
      * @brief True if breakpoints are active.
      */
     bool breakpoints_active_;

     /**
      * @brief The event to stop on. Works only for single stepping.
      */
     Event *stop_event_;

     /** @brief The start time point of the simulation */
     std::chrono::system_clock::time_point start_time_point_;

     /** @brief A custom pool allocator */
     PoolAllocator<128> allocator_;

     XSimLLVM *jit_;

	 std::string source_dir_;
	 std::string build_dir_;
	 std::string lib_dir_;
     std::vector<std::string> modules_;
     std::vector<std::string> internal_files_;
     //std::vector<std::pair<std::string, std::string>> modules_;

     /**
      * @brief The user data that was sent into the simulation.
      */
     std::string user_data_input_;

     /**
      * @brief The user data, one for each replication, that is produced in the simulation.
      */
     std::vector<std::string> user_data_output_;
};

constexpr auto sim = &Simulation::instance;

} // namespace xsim

#endif // SIMULATION_H
