#ifndef EVENT_PRIV_VEVENTHANDLER_HPP
#define EVENT_PRIV_VEVENTHANDLER_HPP

#include <list>

namespace event
{
    class EventBus;

    namespace priv
    {
        class VEventHandler
        {
            public:
                VEventHandler(const VEventHandler&) = delete;
                VEventHandler& operator=(const VEventHandler&) = delete;

                virtual ~VEventHandler();

            protected:
                VEventHandler(unsigned int family);
                
            private:
                friend class ::event::EventBus;

                void _register(EventBus* bus);
                void _unregister(EventBus* bus);

                const unsigned int _family;
                std::list<EventBus*> _bus;
        };
    }
}

#include <events/EventBus.hpp>

namespace event
{
    namespace priv
    {
        VEventHandler::VEventHandler(unsigned int family) : _family(family)
        {
        }

        VEventHandler::~VEventHandler()
        {
            for(auto&& bus : _bus)
                bus->_unregister(_family,this);
        }

        void VEventHandler::_register(EventBus* bus)
        {
            _bus.emplace_back(bus);
        }

        void VEventHandler::_unregister(EventBus* bus)
        {
            _bus.remove(bus);
        }
    }
}
#endif
