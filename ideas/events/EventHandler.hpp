#ifndef EVENT_EVENTHANDLER_HPP
#define EVENT_EVENTHANDLER_HPP

#include <unordered_map>
#include <type_traits>
#include <functional>

#include <events/VEventHandler.hpp>

namespace event
{
    template<typename T> class Emitter;
    class EventBus;

    template<typename T>
    class EventHandler : public priv::VEventHandler
    {
        public:

            using FuncType = std::function<void(T&)>;

            EventHandler(const FuncType& callback = [](T&){/*do nothing*/});

            template<typename U>
            EventHandler(void (U::*method)(T&) );

            virtual ~EventHandler();

            void connect(Emitter<T>& emitter);
            void connect(Emitter<T>& emitter,const FuncType& callback);
            void disconnect(Emitter<T>& emitter);

        private:
            friend class Emitter<T>;
            friend class EventBus;

            void _register(Emitter<T>* emitter);
            void _register(Emitter<T>* emitter,const FuncType& callback);
            void _unregister(Emitter<T>* emitter);

            void exec(Emitter<T>*emitter,T& event);
            void exec(T& event);

            std::unordered_map<Emitter<T>*,const FuncType> _emitters;
            const FuncType _callback;
    };
}

#include <events/Emitter.hpp>
#include <typeinfo>

namespace event
{
    template<typename T>
    EventHandler<T>::EventHandler(const std::function<void(T&)>& callback) : VEventHandler(T::family()), _callback(callback)
    {
        static_assert(std::is_base_of<Event<T>,T>::value, "EventHandler<T>: T must be a class derived from Event<T>");
    }
    template<typename T>
    template<typename U>
    EventHandler<T>::EventHandler(void (U::*method)(T&) ) :
        EventHandler(std::bind(method,static_cast<U*>(this),std::placeholders::_1))
    {
        static_assert(std::is_base_of<EventHandler<T>,U>::value, "EventHandler<T>: the method as parameter must be from a class derived from EventHandler<T>");
    }

    template<typename T>
    EventHandler<T>::~EventHandler()
    {
        for(auto&& emitter : _emitters)
            emitter.first->_unregister(this);
    }

    template<typename T>
    void EventHandler<T>::connect(Emitter<T>& emitter)
    {
        emitter._register(this);
        _register(&emitter);
    }

    template<typename T>
    void EventHandler<T>::connect(Emitter<T>& emitter,const FuncType& callback)
    {
        emitter._register(this);
        _register(&emitter,callback);
    }

    template<typename T>
    void EventHandler<T>::disconnect(Emitter<T>& emitter)
    {
        emitter._unregister(this);
        _unregister(&emitter);
    }

    template<typename T>
    void EventHandler<T>::_register(Emitter<T>* emitter)
    {
        _emitters.emplace(emitter,_callback);
    }

    template<typename T>
    void EventHandler<T>::_register(Emitter<T>* emitter,const FuncType& callback)
    {
        _emitters.emplace(emitter,callback);
    }

    template<typename T>
    void EventHandler<T>::_unregister(Emitter<T>* emitter)
    {
        auto search = _emitters.find(emitter);
        if(search != _emitters.end())
            _emitters.erase(search);
    }

    template<typename T>
    void EventHandler<T>::exec(Emitter<T>* emitter,T& event)
    {
        _emitters.at(emitter)(event);
    }

    template<typename T>
    void EventHandler<T>::exec(T& event)
    {
        _callback(event);
    }

}
#endif
