#ifndef EVENT_EVENTHANDLER_HPP
#define EVENT_EVENTHANDLER_HPP

namespace event
{
    class EventHandler
    {
        public:
            EventHandler();
            EventHandler(const EventHandler&) = delete;
            EventHandler& operator=(const EventHandler&) = delete;

        protected:

        private:
    };
}
#endif