#include "event.hpp"

namespace helios {
    eventData eventData::readyEventData(const json &jsonData) {
        eventData readyEvent;
        if(jsonData["d"].contains("shard")) {
            readyEvent.shardId = jsonData["d"]["shard"].front();
            readyEvent.numShards = jsonData["d"]["shard"].back();
        }

        readyEvent.session_type = jsonData["d"]["session_type"];
        return readyEvent;
    }
}