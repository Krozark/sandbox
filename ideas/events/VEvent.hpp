#ifndef EVENT_PRIV_VEVENT_HPP
#define EVENT_PRIV_VEVENT_HPP

namespace event
{
    namespace priv
    {
        class VEvent
        {
            public:
                VEvent(const VEvent&) = default;
                VEvent& operator=(const VEvent&) = default;

                virtual ~VEvent();


            protected:
                static unsigned int _familyCounter;
                VEvent();
        };

    }
}
#endif
