#pragma once

#include "bus_stop.h"
#include "bus_stop_node.h"
#include "../libds/amt/explicit_hierarchy.h"
#include <vector>
#include <string>

// Utility function to collect all bus stops under a hierarchy node
void collectAllBusStopsUnder(
    ds::amt::MultiWayExplicitHierarchy<BusStopNode>::BlockType *node,
    ds::amt::MultiWayExplicitHierarchy<BusStopNode> &hierarchy,
    std::vector<BusStop> &out);

// Global predicate functions (inline lambdas)
inline auto isOnStreet = [](const std::string& streetName) {
  return [streetName](const BusStop& s) { return s.getStreet().find(streetName) != std::string::npos; };
};

inline auto isInMunicipality = [](const std::string& municipality) {
  return [municipality](const BusStop& s) { return s.getMunicipality() == municipality; };
};

inline auto isInRegion = [](double minLat, double maxLat, double minLon, double maxLon) {
  return [minLat, maxLat, minLon, maxLon](const BusStop& s) {
    return s.getLatitude() >= minLat && s.getLatitude() <= maxLat &&
           s.getLongitude() >= minLon && s.getLongitude() <= maxLon;
  };
};
