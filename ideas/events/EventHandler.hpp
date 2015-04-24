#ifndef EVENT_EVENTHANDLER_HPP
#define EVENT_EVENTHANDLER_HPP

#include <type_traits>

namespace event
{
    template<typename T>
    class EventHandler
    {
        public:
            EventHandler(const EventHandler&) = default;
            EventHandler& operator=(const EventHandler&) = default;

            EventHandler()
            {
                static_assert(std::is_base_of<Event<T>,T>::value, "EventHandler<T>: T must be a class derived from Event<T>");
            };

            virtual ~EventHandler(){};

        //private:
            void virtual onEvent(T&) = 0;

    };
}
#endif
