#ifndef EVENT_EVENTHANDLER_HPP
#define EVENT_EVENTHANDLER_HPP

#include <list>
#include <type_traits>

namespace event
{
    template<typename T> class Emitter;

    template<typename T>
    class EventHandler
    {
        public:

            EventHandler();
            virtual ~EventHandler();

            void connect(Emitter<T>& emitter);
            void disconnect(Emitter<T>& emitter);

        private:
            friend class Emitter<T>;

            virtual void _onEvent(T&);

            void _register(Emitter<T>& emitter);
            void _unregister(Emitter<T>& emitter);

            std::list<Emitter<T>*> _emitters;
    };
}

#include <events/Emitter.hpp>
#include <typeinfo>

namespace event
{
    template<typename T>
    EventHandler<T>::EventHandler()
    {
        static_assert(std::is_base_of<Event<T>,T>::value, "EventHandler<T>: T must be a class derived from Event<T>");
    }

    template<typename T>
    EventHandler<T>::~EventHandler()
    {
        for(auto&& emitter : _emitters)
            emitter->_unregister(*this);
    }

    template<typename T>
    void EventHandler<T>::connect(Emitter<T>& emitter)
    {
        emitter._register(*this);
        _register(emitter);
    }

    template<typename T>
    void EventHandler<T>::disconnect(Emitter<T>& emitter)
    {
        emitter._unregister(*this);
        _unregister(emitter);
    }

    template<typename T>
    void EventHandler<T>::_register(Emitter<T>& emitter)
    {
        _emitters.emplace_back(&emitter);
    }

    template<typename T>
    void EventHandler<T>::_unregister(Emitter<T>& emitter)
    {
        _emitters.remove(&emitter);
    }

    template<typename T>
    void EventHandler<T>::_onEvent(T& e)
    {
        std::cout<<"EventHandler::onEvent("<<typeid(T).name()<<")"<<std::endl;
    }
}
#endif
