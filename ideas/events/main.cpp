#include <string>
#include <iostream>
#include <list>

#include <events/Event.hpp>
#include <events/EventHandler.hpp>

class EventHandler;

template<typename T>
class Emiter
{
    public:
        void emit(T& event);
        void emit(T&& event)
        {
            std::cout<<"Emiter::emit(&&"<<event<<")"<<std::endl;
            emit(event);
        };

        template<typename ... Args>
        void emit(Args&& ... args)
        {
            std::cout<<"Emiter::emit(Args&& ...)"<<std::endl;
            T e(std::forward<Args>(args)...);
            emit(e);
        };


        ~Emiter();


        void connect(EventHandler& handler);
        void disconnect(EventHandler& handler);

    private:

        friend class EventHandler;

        void _register(EventHandler& handler);
        void _unregister(EventHandler& handler);

        std::list<EventHandler*> _handlers;
};


class Event : event::Event<Event>
{
    public:
        Event(int i, std::string str) : _i(i), _str(str){};
        
        friend std::ostream& operator<<(std::ostream& stream,const Event& self)
        {
            stream<<"family= "<<self.family()<<", _i= "<<self._i<<", _str= "<<self._str;
            return stream;
        }


    private:
        int _i;
        std::string _str;
};


class EventHandler : event::EventHandler<Event>
{
    public:
        void connect(Emiter<Event>& emiter)
        {
            emiter._register(*this);
            _register(emiter);
        };


        void disconnect(Emiter<Event>& emiter)
        {
            emiter._unregister(*this);
            _unregister(emiter);
        };


        ~EventHandler()
        {
            for(auto& emiter : _emiters)
                emiter->_unregister(*this);
        }

    private:
        friend class Emiter<Event>;

        void _register(Emiter<Event>& emiter)
        {
            _emiters.emplace_back(&emiter);
        }

        void _unregister(Emiter<Event>& emiter)
        {
            _emiters.remove(&emiter);
        }

        void onEvent(Event& e) override
        {
            std::cout<<"EventHandler::onEvent("<<e<<")"<<std::endl;
        };

        void onEvent(Emiter<Event>& src, Event& e)
        {
            std::cout<<"EventHandler::onEvent(src,"<<e<<")"<<std::endl;
        };

        std::list<Emiter<Event>*> _emiters;
};

template<typename T>
Emiter<T>::~Emiter()
{
    for(auto& handler : _handlers)
        handler->_unregister(*this);
    _handlers.clear();
}


template<typename T>
void Emiter<T>::emit(T& event)
{
    std::cout<<"Emiter::emit(&"<<event<<")"<<std::endl;
    for(auto& handler : _handlers)
        handler->onEvent(*this,event);
};



template<typename T>
void Emiter<T>::connect(EventHandler& handler)
{
    handler._register(*this);
    _register(handler);
}

template<typename T>
void Emiter<T>::disconnect(EventHandler& handler)
{
    handler._unregister(*this);
    _unregister(handler);
}

template<typename T>
void Emiter<T>::_register(EventHandler& handler)
{
    _handlers.emplace_back(&handler);
}

template<typename T>
void Emiter<T>::_unregister(EventHandler& handler)
{
    _handlers.remove(&handler);
}


int main(int argc,char* argv[])
{
    //object to object link
    {
        Event event(42,"Event");
        std::cout<<event<<std::endl;

        EventHandler handler;
        Emiter<Event> emiter;


        {
            std::cout<<"------ handler receive nothing ------"<<std::endl;


            std::cout<<"-- emit by &ref "<<std::endl;
            emiter.emit(event); //&ref
            std::cout<<"-- emit by construction "<<std::endl;
            emiter.emit(72,"pwet"); //emplace
            std::cout<<"-- emit by &&ref "<<std::endl;
            emiter.emit(Event(48,"pwet 2")); //&&ref
        }

        {
            std::cout<<"------ handler receive events ------"<<std::endl;
            handler.connect(emiter);
            emiter.emit(event); //&ref
        }

        {
            std::cout<<"------ handler receive nothing ------"<<std::endl;
            handler.disconnect(emiter);
            emiter.emit(event); //&ref
        }

        {
            std::cout<<"------ handler receive events ------"<<std::endl;
            EventHandler handler2;
            handler.connect(emiter);
            handler2.connect(emiter);
            emiter.emit(event); //&ref

        }

        /*handler.connect(emiter,std::function<void(Event&)>{
                        });

        handler.connect(emiter,[](Event&){

        });
        */
    }

    //event throught bus
    {
        Event event(67,"Event 2");
        EventHandler handler;

        EventBus bus;
        bus.register<Event>(handler);

        bus.emit(event);
    }



    return 0;
}
