#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include "nano_signal_slot.hpp"

template <typename Signature>
using Signal = Nano::Signal<Signature>;
using SignalObserver = Nano::Observer<>;

#endif
