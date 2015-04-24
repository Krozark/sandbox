#include <string>
#include <iostream>
#include <list>

#include <events/Event.hpp>
#include <events/EventHandler.hpp>
#include <events/Emitter.hpp>

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

int main(int argc,char* argv[])
{
    //object to object link
    {
        Event event(42,"Event");
        std::cout<<event<<std::endl;

        event::EventHandler<Event> handler;
        event::Emitter<Event> emiter;
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
            event::EventHandler<Event> handler2;
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
    /*{
        Event event(67,"Event 2");
        event::EventHandler<Event> handler;

        event::EventBus bus;
        bus.register<Event>(handler);

        bus.emit(event);
    }*/



    return 0;
}
