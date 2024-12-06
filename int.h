#ifndef INT_H
#define INT_H

#include <xsim_config>
#include <list>
#include <string>

namespace mu {
class Parser;
}

namespace xsim {

class Simulation;

class XSIM_EXPORT Int {
public:
    Int();
    Int(int value);
    Int(const std::string& value);
    Int(const Int& d);
    ~Int();
    Int& operator=(const Int& rhs);
    operator int() const;

    int value() const;
    std::string to_string() const;
    mu::Parser* parser() const;

private:
    mu::Parser* parser_;
    int value_;
    std::string value_string_;
};

} // namespace xsim

#endif // INT_H
