#ifndef ACTIVEPERIOD_H
#define ACTIVEPERIOD_H

#include "common.h"

namespace xsim {

class Node;

class XSIM_EXPORT ActivePeriod {
public:
    ActivePeriod() : start_(0), end_(0), first_cutoff_(0), last_cutoff_(0),
    open_end_(false), safe_to_discard_(true), node_(0) {}
    ActivePeriod(Node *node, simtime start, simtime end, bool open_end = false) :
        start_(start), end_(end), first_cutoff_(start), last_cutoff_(end),
        tmp_first_cutoff_(start), open_end_(open_end), safe_to_discard_(true),
        node_(node) {}

    simtime start_;
    simtime end_;
    simtime first_cutoff_;
    simtime last_cutoff_;
    simtime tmp_first_cutoff_;

    bool open_end_;
    bool safe_to_discard_;

    Node *node_;
};

bool compare_active_period(const ActivePeriod *left, const ActivePeriod *right);

}

#endif // ACTIVEPERIOD_H
