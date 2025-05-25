#pragma once

#include "bus_stop.h"
#include "bus_stop_node.h"
#include "predicates.h"
#include "filter_algorithm.h"
#include "bus_stop_sorter.h"
#include "../libds/amt/explicit_hierarchy.h"
#include "../libds/adt/table.h"
#include <vector>
#include <string>

class BusStopFilter {
public:
  using HierarchyT = ds::amt::MultiWayExplicitHierarchy<BusStopNode>;

  void loadFromCSV(const std::string &fileName);

  const std::vector<BusStopNode>& getBusStopsVec() const;
  HierarchyT& getHierarchy();

  class Navigator {
  public:
    using Block = typename HierarchyT::BlockType;
    Navigator(BusStopFilter &f);
    void run();
    
  private:
    BusStopFilter &filter;
    HierarchyT &h;
    Block *curr;

    void printMenu();
    std::string nodeDescription(Block *b);
    void goUp();
    void chooseChild();
    
    template<class Pred> 
    void applyPredicate(const std::string &name, Pred p);
    
    int getPredicateChoice();
    std::string getStreetInput();
    std::string getMunicipalityInput();
    bool getRegionInput(double& minLat, double& maxLat, double& minLon, double& maxLon);
    
    void runPredicates();
    void findByID();
    void filterVectorMenu();
    void sortingMenu();  // New sorting functionality
  };

  void runNavigator();

  BusStop* findStopByID(int id);

  template <typename Pred>
  std::vector<BusStop> filterVector(Pred p);

private:
  std::vector<BusStopNode> busStopsVec_;
  HierarchyT busStopsHierarchy_;
  ds::adt::SortedSTab<int, BusStop*> busStopsTable_;
  FilterAlgorithm filterAlgorithm_; // Dedicated standalone filtering algorithm object
  BusStopSorter sorter_; // Dedicated standalone sorting algorithm object
};
