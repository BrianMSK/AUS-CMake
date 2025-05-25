#include "predicates.h"

void collectAllBusStopsUnder(
    ds::amt::MultiWayExplicitHierarchy<BusStopNode>::BlockType *node,
    ds::amt::MultiWayExplicitHierarchy<BusStopNode> &hierarchy,
    std::vector<BusStop> &out) {
  hierarchy.processPreOrder(node, [&out](auto *b) {
    if (b->data_.getType() == BUSSTOP) {
      out.push_back(*b->data_.getBusStop());
    }
  });
}
