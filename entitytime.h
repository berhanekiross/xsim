#ifndef ENTITYTIME_H
#define ENTITYTIME_H

#include <xsim_config>

namespace xsim {

class Entity;

struct XSIM_EXPORT EntityTime {
    EntityTime() : entity(0), time(0) {}
    EntityTime(Entity *entity, simtime time) : entity(entity), time(time) {}

    Entity* entity;
    simtime time;
};

} // namespace xsim

#endif // ENTITYTIME_H

