#ifndef EVENT_EMITTER_HPP
#define EVENT_EMITTER_HPP

#include <iostream>
#include <list>
#include <type_traits>

namespace event
{
    template<typename> class EventHandler;

    template<typename T>
    class Emitter
    {
        public:

            Emitter();
            virtual ~Emitter();

            void emit(T& event);

            void emit(T&& event);

            template<typename ... Args>
            void emit(Args&& ... args);

            void connect(EventHandler<T>& handler);
            void disconnect(EventHandler<T>& handler);

        private:
            friend class EventHandler<T>;

            void _register(EventHandler<T>* handler);
            void _unregister(EventHandler<T>* handler);

            std::list<EventHandler<T>*> _handlers;
    };
}

#include <events/EventHandler.hpp>

namespace event
{
    template<typename T>
    Emitter<T>::Emitter()
    {
        static_assert(std::is_base_of<Event<T>,T>::value, "Emitter<T>: T must be a class derived from Event<T>");
    }

    template<typename T>
    Emitter<T>::~Emitter()
    {
        for(auto&& handler : _handlers)
            handler->_unregister(this);
        _handlers.clear();
    }

    template<typename T>
    void Emitter<T>::emit(T& event)
    {
        std::cout<<"Emiter::emit(&"<<event<<")"<<std::endl;
        for(auto&& handler : _handlers)
            handler->exec(this,event);
    }

    template<typename T>
    void Emitter<T>::emit(T&& event)
    {
        std::cout<<"Emitter::emit(&&"<<event<<")"<<std::endl;
        emit(event);
    }

    template<typename T>
    template<typename ... Args>
    void Emitter<T>::emit(Args&& ... args)
    {
        std::cout<<"Emitter::emit(Args&& ...)"<<std::endl;
        T e(std::forward<Args>(args)...);
        emit(e);
    }


    template<typename T>
    void Emitter<T>::connect(EventHandler<T>& handler)
    {
         handler._register(this);
        _register(&handler);
    }

    template<typename T>
    void Emitter<T>::disconnect(EventHandler<T>& handler)
    {
        handler._unregister(this);
        _unregister(&handler);
    }

    template<typename T>
    void Emitter<T>::_register(EventHandler<T>* handler)
    {
        _handlers.emplace_back(handler);
    }

    template<typename T>
    void Emitter<T>::_unregister(EventHandler<T>* handler)
    {
        _handlers.remove(handler);
    }
}

#endif
