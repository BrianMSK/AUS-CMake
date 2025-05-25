#include "bus_stop_sorter.h"
#include <iostream>
#include <iomanip>
#include <memory>

BusStopSorter::BusStopSorter() {
    sortAlgorithm_ = std::make_unique<ds::adt::QuickSort<BusStop>>();
}

std::vector<BusStop> BusStopSorter::sort(const std::vector<BusStop>& data, 
                                        std::function<bool(const BusStop&, const BusStop&)> comparator) const {
    if (data.empty()) {
        return {};
    }
    
    ds::amt::ImplicitSequence<BusStop> sequence;
    vectorToSequence(data, sequence);
    
    sortAlgorithm_->sort(sequence, comparator);
    
    return sequenceToVector(sequence);
}

std::vector<BusStop> BusStopSorter::sortByStreetMunicipality(const std::vector<BusStop>& data) const {
    auto compareStreetMunicipality = [](const BusStop& a, const BusStop& b) -> bool {
        int municipalityComparison = a.getMunicipality().compare(b.getMunicipality());
        if (municipalityComparison != 0) {
            return municipalityComparison < 0;
        }
        return a.getStreet().compare(b.getStreet()) < 0;
    };
    
    return sort(data, compareStreetMunicipality);
}

std::vector<BusStop> BusStopSorter::sortByID(const std::vector<BusStop>& data) const {
    auto compareID = [](const BusStop& a, const BusStop& b) -> bool {
        return a.getStopID() < b.getStopID();
    };
    
    return sort(data, compareID);
}

void BusStopSorter::printSortedData(const std::vector<BusStop>& sortedData, const std::string& sortType) const {
    std::cout << "\n=== Sorted data (" << sortType << ") ===\n";
    std::cout << "Found " << sortedData.size() << " bus stops:\n\n";
    
    std::cout << std::left << std::setw(8) << "ID" 
              << std::setw(25) << "Municipality" 
              << std::setw(30) << "Street" 
              << std::setw(12) << "Longitude" 
              << std::setw(12) << "Latitude" << "\n";
    std::cout << std::string(87, '-') << "\n";
    
    for (const auto& stop : sortedData) {
        std::cout << std::left << std::setw(8) << stop.getStopID()
                  << std::setw(25) << stop.getMunicipality()
                  << std::setw(30) << stop.getStreet()
                  << std::setw(12) << std::fixed << std::setprecision(6) << stop.getLongitude()
                  << std::setw(12) << std::fixed << std::setprecision(6) << stop.getLatitude() << "\n";
    }
    
    if (sortType.find("Municipality/Street") != std::string::npos) {
        std::cout << "\nSorting criteria: Municipality (alphabetical), then Street (alphabetical)\n";
    } else if (sortType.find("ID") != std::string::npos) {
        std::cout << "\nSorting criteria: Stop ID (ascending)\n";
    }
}

void BusStopSorter::vectorToSequence(const std::vector<BusStop>& data, ds::amt::ImplicitSequence<BusStop>& sequence) const {
    sequence.clear();
    
    for (const auto& stop : data) {
        sequence.insertLast().data_ = stop;
    }
}

std::vector<BusStop> BusStopSorter::sequenceToVector(const ds::amt::ImplicitSequence<BusStop>& sequence) const {
    std::vector<BusStop> result;
    result.reserve(sequence.size());
    
    for (size_t i = 0; i < sequence.size(); ++i) {
        auto* block = sequence.access(i);
        if (block) {
            result.push_back(block->data_);
        }
    }
    
    return result;
}
