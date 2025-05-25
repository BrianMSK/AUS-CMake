#pragma once

#include "bus_stop.h"
#include <string>
#include <optional>

enum BSNTypes { COMPANY, MUNICIPALITY, STREET, BUSSTOP };

class BusStopNode {
private:
  BSNTypes type_;
  std::string name_;
  std::optional<BusStop> busStop_;

public:
  BusStopNode();

  BSNTypes getType() const;
  void setType(BSNTypes t);

  const std::string &getName() const;
  void setName(const std::string &n);
  void setName(std::string &&n);

  bool hasBusStop() const;

  const BusStop *getBusStop() const;
  BusStop *getBusStop();

  void setBusStop(const BusStop &bs);
  void clearBusStop();

  bool operator==(const BusStopNode &o) const;
};
