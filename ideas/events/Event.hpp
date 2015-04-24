#ifndef EVENT_EVENT_HPP
#define EVENT_EVENT_HPP

#include <events/VEvent.hpp>

namespace event
{
    template<typename T>
    class Event : public priv::VEvent
    {
        public:
            Event(const Event<T>&) = default;
            Event& operator=(const Event<T>&) = default;
        
            Event(){};
            ~Event(){};

            static unsigned int family()
            {
                static unsigned int family = VEvent::_familyCounter++;
                return family;
            };

            //void emit();

        private:
    };
}
#endif
