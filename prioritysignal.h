#ifndef PRIORITYSIGNAL_H
#define PRIORITYSIGNAL_H

#include <xsim_config>
#include <map>

#include "signal.hpp"

template <typename RT>
class PrioritySignal;
template<typename RT, typename... Args>
class PrioritySignal<RT(Args...)> {
public:
    PrioritySignal() :
        firing_(false),
        current_priority_(std::numeric_limits<int>::lowest())
    {}

    template <typename L>
    bool connect(L* instance, int priority)
    {
        if (firing_ && priority <= current_priority_)
            return false;
        else
            signals_[priority].template connect<L>(instance);
        return true;
    }

    template <typename L>
    bool connect(L& instance, int priority)
    {
        if (firing_ && priority <= current_priority_)
            return false;
        else
            signals_[priority].template connect<L>(instance);
        return true;
    }

    template <auto mem_ptr, typename T>
    bool connect(T* instance, int priority)
    {
        if (firing_ && priority <= current_priority_)
            return false;
        else
            signals_[priority].template connect<mem_ptr, T>(instance);
        return true;
    }

    template <auto mem_ptr, typename T>
    bool connect(T& instance, int priority)
    {
        if (firing_ && priority <= current_priority_)
            return false;
        else
            signals_[priority].template connect<mem_ptr, T>(instance);
        return true;
    }

    template <RT(*fun_ptr)(Args...)>
    bool connect(int priority)
    {
        if (firing_ && priority <= current_priority_)
            return false;
        else
            signals_[priority].template connect<fun_ptr>();
        return true;
    }

    template <typename T, RT(T::* mem_ptr)(Args...)>
    bool connect(T& instance, int priority)
    {
        if (firing_ && priority <= current_priority_)
            return false;
        else
            signals_[priority].template connect<T, mem_ptr>(instance);
        return true;
    }

    template <typename T, RT(T::* mem_ptr)(Args...) const>
    bool connect(T& instance, int priority)
    {
        if (firing_ && priority <= current_priority_)
            return false;
        else
            signals_[priority].template connect<T, mem_ptr>(instance);
        return true;
    }

    template <typename T, RT(T::* mem_ptr)(Args...)>
    bool connect(T* instance, int priority)
    {
        if (firing_ && priority <= current_priority_)
            return false;
        else
            signals_[priority].template connect<T, mem_ptr>(instance);
        return true;
    }

    template <typename T, RT(T::* mem_ptr)(Args...) const>
    bool connect(T* instance, int priority)
    {
        if (firing_ && priority <= current_priority_)
            return false;
        else
            signals_[priority].template connect<T, mem_ptr>(instance);
        return true;
    }

    template <typename L>
    bool disconnect(L* instance, int priority)
    {
        if (firing_ && priority == current_priority_)
            return false;
        else
            signals_[priority].template disconnect<L>(instance);
        return true;
    }

    template <typename L>
    bool disconnect(L& instance, int priority)
    {
        if (firing_ && priority == current_priority_)
            return false;
        else
            signals_[priority].template disconnect<L>(instance);
        return true;
    }

    template <RT(*fun_ptr)(Args...)>
    bool disconnect(int priority)
    {
        if (firing_ && priority == current_priority_)
            return false;
        else
            signals_[priority].template disconnect<fun_ptr>();
        return true;
    }

    template <typename T, RT(T::* mem_ptr)(Args...)>
    bool disconnect(T* instance, int priority)
    {
        if (firing_ && priority == current_priority_)
            return false;
        else
            signals_[priority].template disconnect<T, mem_ptr>(instance);
        return true;
    }

    template <typename T, RT(T::* mem_ptr)(Args...) const>
    bool disconnect(T* instance, int priority)
    {
        if (firing_ && priority == current_priority_)
            return false;
        else
            signals_[priority].template disconnect<T, mem_ptr>(instance);
        return true;
    }

    template <typename T, RT(T::* mem_ptr)(Args...)>
    bool disconnect(T& instance, int priority)
    {
        if (firing_ && priority == current_priority_)
            return false;
        else
            signals_[priority].template disconnect<T, mem_ptr>(instance);
        return true;
    }

    template <typename T, RT(T::* mem_ptr)(Args...) const>
    bool disconnect(T& instance, int priority)
    {
        if (firing_ && priority == current_priority_)
            return false;
        else
            signals_[priority].template disconnect<T, mem_ptr>(instance);
        return true;
    }

    template <auto mem_ptr, typename T>
    bool disconnect(T* instance, int priority)
    {
        if (firing_ && priority == current_priority_)
            return false;
        else
            signals_[priority].template disconnect<T, mem_ptr>(instance);
        return true;
    }

    template <auto mem_ptr, typename T>
    bool disconnect(T& instance, int priority)
    {
        if (firing_ && priority == current_priority_)
            return false;
        else
            signals_[priority].template disconnect<T, mem_ptr>(instance);
        return true;
    }

    template <typename... Uref>
    void fire(Uref&&... args)
    {
        firing_ = true;
        for (auto& [priority, signal] : signals_) {
            current_priority_ = priority;
            signal.fire(std::forward<Uref>(args)...);
        }
        firing_ = false;
    }

    void disconnect_all()
    {
        for (auto& [priority, signal] : signals_)
            signal.disconnect_all();
        signals_.clear();
    }

private:
    std::map<int, Signal<RT(Args...)>> signals_;
    int firing_;
    int current_priority_;
};

#endif
