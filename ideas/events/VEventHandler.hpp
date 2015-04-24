#ifndef EVENT_PRIV_VEVENTHANDLER_HPP
#define EVENT_PRIV_VEVENTHANDLER_HPP

namespace event
{
    namespace priv
    {
        class VEventHandler
        {
            public:
                VEventHandler();
                VEventHandler(const VEventHandler&) = delete;
                VEventHandler& operator=(const VEventHandler&) = delete;

            protected:

            private:
        };
    }
}
#endif