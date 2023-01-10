#include "event.hpp"

namespace helios {
    eventData eventData::readyEventData(const json &jsonData) {
        eventData readyEvent;
        readyEvent.session_type = jsonData["d"]["session_type"];
        return readyEvent;
    }
}