#include <string>
#include <iostream>

#include <events/Event.hpp>
#include <events/EventHandler.hpp>
#include <events/Emitter.hpp>
#include <events/EventBus.hpp>

class TestEvent : public event::Event<TestEvent>
{
    public:
        TestEvent(int i, std::string str) : _i(i), _str(str){};

        friend std::ostream& operator<<(std::ostream& stream,const TestEvent& self)
        {
            stream<<"family= "<<self.family()<<", _i= "<<self._i<<", _str= "<<self._str;
            return stream;
        }

    private:
        int _i;
        std::string _str;
};

class HandlerClass : public event::EventHandler<TestEvent>
{
    public:
        void onEvent(TestEvent& event)
        {
            std::cout<<"HandlerClass::onEvent("<<event<<")"<<std::endl;
        }

         HandlerClass() : EventHandler(&HandlerClass::onEvent)
        {
        };
};


int main(int argc,char* argv[])
{
    //object to object link
    /*{
        TestEvent event(42,"TestEvent");
        std::cout<<event<<std::endl;

        event::EventHandler<TestEvent> handler([](TestEvent& event){
            std::cout<<"handler default function: "<<event<<std::endl;
        });
        event::Emitter<TestEvent> emitter1;

        {
            std::cout<<std::endl<<"------ handler should receive nothing ------"<<std::endl;

            std::cout<<"-- emit by &ref "<<std::endl;
            emitter1.emit(event);
            std::cout<<"-- emit by construction "<<std::endl;
            emitter1.emit(72,"pwet");
            std::cout<<"-- emit by &&ref "<<std::endl;
            emitter1.emit(TestEvent(48,"pwet 2"));
        }

        {
            std::cout<<std::endl<<"------ handler should receive events ------"<<std::endl;
            handler.connect(emitter1);
            emitter1.emit(event);
        }

        {
            std::cout<<std::endl<<"------ handler should receive nothing ------"<<std::endl;
            handler.disconnect(emitter1);
            emitter1.emit(event);
        }

        {
            std::cout<<std::endl<<"------ handlers 1,2 should receive events ------"<<std::endl;
            event::EventHandler<TestEvent> handler2;

            event::Emitter<TestEvent> emitter2;

            handler.connect(emitter1);

            handler2.connect(emitter1,[](TestEvent& event){
                std::cout<<"handler2 from emitter1: "<<event<<std::endl;
            });
            handler2.connect(emitter2,[](TestEvent& event){
                std::cout<<"handler2 from emitter2: "<<event<<std::endl;
            });
            emitter1.emit(event);
            emitter2.emit(event);
        }

        {
            std::cout<<std::endl<<"------ handlers class should receive events ------"<<std::endl;

            HandlerClass handlerClass;
            handlerClass.connect(emitter1);

            emitter1.emit(event);
        }
    }*/

    //event throught bus
    {
        event::EventBus bus;

        TestEvent event(67,"Event 2");
        event::EventHandler<TestEvent> handler([](TestEvent& event){
            std::cout<<"handler default function: "<<event<<std::endl;
        });

        bus.connect<TestEvent>(handler);
        /*bus.connect<TestEvent>(handler,[](TestEvent& event){
            std::cout<<"bus connect function: "<<event<<std::endl;
       });*/

        bus.emit(event);
    }



    return 0;
}
