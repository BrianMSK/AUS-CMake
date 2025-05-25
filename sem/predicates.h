#pragma once

#include "bus_stop.h"
#include "bus_stop_node.h"
#include "../libds/amt/explicit_hierarchy.h"
#include <vector>
#include <string>

inline auto nodeIsOnStreet = [](const std::string& streetName) {
  return [streetName](const BusStopNode& node) { 
    if (node.hasBusStop()) {
      const BusStop* stop = node.getBusStop();
      return stop && stop->getStreet().find(streetName) != std::string::npos;
    }
    return false;
  };
};

inline auto nodeIsInMunicipality = [](const std::string& municipality) {
  return [municipality](const BusStopNode& node) { 
    if (node.hasBusStop()) {
      const BusStop* stop = node.getBusStop();
      return stop && stop->getMunicipality() == municipality;
    }
    return false;
  };
};

inline auto nodeIsInRegion = [](double minLat, double maxLat, double minLon, double maxLon) {
  return [minLat, maxLat, minLon, maxLon](const BusStopNode& node) {
    if (node.hasBusStop()) {
      const BusStop* stop = node.getBusStop();
      return stop && stop->getLatitude() >= minLat && stop->getLatitude() <= maxLat &&
             stop->getLongitude() >= minLon && stop->getLongitude() <= maxLon;
    }
    return false;
  };
};
