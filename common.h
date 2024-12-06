#ifndef COMMON_H
#define COMMON_H

#include <xsim_config>
#include <cmath>
#include <string>
#include <vector>

#include "date/date.h"
#include "date/iso_week.h"

#pragma warning(disable : 4996)

namespace xsim {

class Entity;
class Node;

const int PRIORITY_NOW = -1;
const int PRIORITY_RESET_STATS = 0;
const int PRIORITY_SHIFT_CALENDAR_START = 1;
const int PRIORITY_OUT_UNBLOCKED = 2;
const int PRIORITY_SHIFT = 3;
const int PRIORITY_STARTNEWBATCH = 4;
const int PRIORITY_TAKTCOMPLETE_UNBLOCKED = 4;
const int PRIORITY_DISRUPTION_BEGIN_PROCESSING = 4;
const int PRIORITY_DISRUPTION_BEGIN = 5;
const int PRIORITY_BREAK = 4;
const int PRIORITY_CONVEYOR_OPEN = 6;
const int PRIORITY_UPDATE_CONVEYOR = 2;
const int PRIORITY_OUT = 5;
const int PRIORITY_REQUEST_RESOURCES = 6;
const int PRIORITY_RESOURCE_READY = 6;
const int PRIORITY_CREATEMU = 5;
const int PRIORITY_CREATEDEMAND = 3;
const int PRIORITY_SETUP = 5;
const int PRIORITY_DISASSEMBLY = 5;
const int PRIORITY_ASSEMBLY = 5;
const int PRIORITY_DISRUPTION_END = 6;
const int PRIORITY_TAKTCOMPLETE = 7;
const int PRIORITY_ENTRYTIMEOUT = 7;
const int PRIORITY_TRIGGERSYNCHRONIZEDEXITS = 7;
const int PRIORITY_ANIMATE_CONVEYOR = 0;
const int PRIORITY_TIMECALLBACK = 8;

const int STEP_MORE = 0;
const int STEP_BREAKPOINT = 1;
const int STEP_STOPPED = 2;
const int STEP_END = 3;

const simtime tolerance = 0.00001;

/**
 * @brief Enum of failure types.
 */
enum FailureTypes { PERCENT, DISTRIBUTIONS, CYCLES };
enum FailureTimeReference { SIMULATION, PROCESSING, OPERATIONAL };

enum ResourceType { PROCESSING_RESOURCE, REPAIR_RESOURCE, SETUP_RESOURCE };

class bad_setting : public std::exception {
public:
    bad_setting(std::string msg) : msg_(msg) {}
    ~bad_setting() throw() {}
    virtual const char* what() const throw()
    {
        return msg_.c_str();
    }

private:
    std::string msg_;
};

struct XSIM_EXPORT ShiftingBottleneck {
    bool operator>(const ShiftingBottleneck &right)
    {
        if (this->sole_ + this->shifting_ > right.sole_ + right.shifting_)
            return true;
        else
            return false;
    }

    ShiftingBottleneck() : sole_(0), shifting_(0) {}
    ShiftingBottleneck(simtime sole, simtime shifting) :
        sole_(sole), shifting_(shifting) {}

    simtime sole_;
    simtime shifting_;
};

struct XSIM_EXPORT ConveyorAnimate {
    Entity *entity;
    int start;
    int length;
};


int exec(const char* cmd, std::string& output);

bool equalt(simtime lhs, simtime rhs);
bool equalf(double lhs, double rhs);
bool lessf(double lhs, double rhs);
bool greaterf(double lhs, double rhs);
bool greatert(simtime lhs, simtime rhs);
bool greatequl(simtime lhs, simtime rhs);
bool lessequl(simtime lhs, simtime rhs);
std::string to_string(double value);
std::string to_string(unsigned int value);
XSIM_EXPORT bool get_time_from_string(const std::string &str, double &seconds);
double get_double_from_short_time_string(std::string str);
bool to_bool(std::string str);
double get_mean(std::vector<double> &replications);

// Calculate standard deviation using the n-1 method, since we do not have the
// complete population.
double get_standard_deviation(std::vector<double> &replications, double mean);

double get_confidence_interval(double n, double mean,
        double standard_deviation);
std::string time_as_string(double time);
std::string double_to_time_string(double time);
std::string format_type(const std::string &type);

XSIM_EXPORT int to_number(date::year year);
XSIM_EXPORT unsigned to_number(date::month month);
XSIM_EXPORT unsigned to_number(date::day);
XSIM_EXPORT unsigned to_number(date::weekday weekday);
XSIM_EXPORT unsigned to_number(iso_week::weeknum weeknum);
XSIM_EXPORT float to_number(std::chrono::duration<float> time_of_day);

template <typename T>
struct reversion_wrapper { T& iterable; };

template <typename T>
auto begin (reversion_wrapper<T> w) { return std::rbegin(w.iterable); }

template <typename T>
auto end (reversion_wrapper<T> w) { return std::rend(w.iterable); }

template <typename T>
reversion_wrapper<T> reverse (T&& iterable) { return { iterable }; }


} // namespace xsim

#endif // COMMON_H
