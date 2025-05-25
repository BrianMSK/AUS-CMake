#pragma once

#include "bus_stop.h"
#include "../libds/adt/sorts.h"
#include "../libds/amt/implicit_sequence.h"
#include <vector>
#include <functional>
#include <string>
#include <memory>

class BusStopSorter {
public:
    BusStopSorter();
    ~BusStopSorter() = default;
    
    std::vector<BusStop> sort(const std::vector<BusStop>& data, 
                              std::function<bool(const BusStop&, const BusStop&)> comparator) const;
    
    std::vector<BusStop> sortByStreetMunicipality(const std::vector<BusStop>& data) const;
    
    std::vector<BusStop> sortByID(const std::vector<BusStop>& data) const;
    
    void printSortedData(const std::vector<BusStop>& sortedData, const std::string& sortType) const;

private:
    std::unique_ptr<ds::adt::QuickSort<BusStop>> sortAlgorithm_;
    
    void vectorToSequence(const std::vector<BusStop>& data, ds::amt::ImplicitSequence<BusStop>& sequence) const;
    
    std::vector<BusStop> sequenceToVector(const ds::amt::ImplicitSequence<BusStop>& sequence) const;
};
