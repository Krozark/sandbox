#ifndef EVENT_EVENTBUS_HPP
#define EVENT_EVENTBUS_HPP

#include <list>

namespace event
{
    namespace priv
    {
        class VEventHandler;
    }

    template<typename> class EventHandler;

    class EventBus
    {
        public:
            ~EventBus();

            template<typename T>
            void connect(EventHandler<T>& handler);

            /*template<typename T>
            void connect(EventHandler<T>& handler,const std::function<void(T& event)>& callback);*/


            template<typename T>
            void disconnect(EventHandler<T>& handler);

            template<typename T>
            void emit(T& event);
        private:

            friend class priv::VEventHandler;

            void _unregister(unsigned int family,priv::VEventHandler* handler);

            std::unordered_map<unsigned int,std::list<priv::VEventHandler*>> _handlers;
    };
}

#include <events/EventHandler.hpp>

#include <iostream>

namespace event
{

    EventBus::~EventBus()
    {
        for(auto&& pair : _handlers)
        {
            for(auto&& handler : pair.second)
                handler->_unregister(this);
        }
    }

    template<typename T>
    void EventBus::connect(EventHandler<T>& handler)
    {
        static_assert(std::is_base_of<Event<T>,T>::value, "EventBus::connect<T>(EventHandler<T>&): T must be a class derived from Event<T>");

        std::cout<<"EventBus::connect("<<T::family()<<")"<<std::endl;

        static_cast<priv::VEventHandler&>(handler)._register(this);
        _handlers[T::family()].emplace_back(&handler);

    }

    /*template<typename T>
    void EventBus::connect(EventHandler<T>& handler,const std::function<void(T& event)>& callback)
    {
        static_assert(std::is_base_of<Event<T>,T>::value, "EventBus::connect<T>(EventHandler<T>&): T must be a class derived from Event<T>");
        std::cout<<"EventBus::connect("<<T::family()<<",callback)"<<std::endl;
    }*/

    template<typename T>
    void EventBus::disconnect(EventHandler<T>& handler)
    {
        static_assert(std::is_base_of<Event<T>,T>::value, "EventBus::disconnect<T>(EventHandler<T>&): T must be a class derived from Event<T>");

        std::cout<<"EventBus::disconnect("<<T::family()<<")"<<std::endl;

        handler._unregister(this);
        _unregister(T::family,&handler);
    }

    template<typename T>
    void EventBus::emit(T& event)
    {
        static_assert(std::is_base_of<Event<T>,T>::value, "EventBus::emit(T&): T must be a class derived from Event<T>");
        std::cout<<"EventBus::emit("<<T::family()<<")"<<std::endl;

        for(auto&& handler : _handlers[T::family()])
            static_cast<EventHandler<T>*>(handler)->exec(event);

    }

    void EventBus::_unregister(unsigned int family,priv::VEventHandler* handler)
    {
        _handlers[family].remove(handler);
        std::cout<<"EventBus::_unregister("<<family<<")"<<std::endl;
    }
}
#endif
