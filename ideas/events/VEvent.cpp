#include <events/VEvent.hpp>

namespace event
{
    namespace priv
    {
        unsigned int VEvent::_familyCounter = 0;

        VEvent:: ~VEvent()
        {
        }
        
        VEvent::VEvent()
        {
        }
    }
}
