#include "bus_stop_node.h"

BusStopNode::BusStopNode() : type_(COMPANY), name_(), busStop_(std::nullopt) {}

BSNTypes BusStopNode::getType() const { return type_; }

void BusStopNode::setType(BSNTypes t) { type_ = t; }

const std::string &BusStopNode::getName() const { return name_; }

void BusStopNode::setName(const std::string &n) { name_ = n; }

void BusStopNode::setName(std::string &&n) { name_ = std::move(n); }

bool BusStopNode::hasBusStop() const { return busStop_.has_value(); }

const BusStop *BusStopNode::getBusStop() const { return busStop_ ? &*busStop_ : nullptr; }

BusStop *BusStopNode::getBusStop() { return busStop_ ? &*busStop_ : nullptr; }

void BusStopNode::setBusStop(const BusStop &bs) {
  type_ = BUSSTOP;
  busStop_ = bs;
  name_ = std::to_string(bs.getStopID());
}

void BusStopNode::clearBusStop() { busStop_.reset(); }

bool BusStopNode::operator==(const BusStopNode &o) const {
  return type_ == o.type_ && name_ == o.name_ && busStop_ == o.busStop_;
}
