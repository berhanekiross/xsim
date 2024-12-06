#ifndef CONVEYORITEM_H
#define CONVEYORITEM_H

#include <xsim_config>

namespace xsim {

class Entity;

struct XSIM_EXPORT ConveyorItem {
    Entity *entity;
    simtime time;
    double pos;
    bool blocked;
};

} // namespace xsim

#endif // CONVEYORITEM_H

