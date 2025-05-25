#include "bus_stop_filter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <limits>

const std::vector<BusStopNode>& BusStopFilter::getBusStopsVec() const { 
  return busStopsVec_; 
}

BusStopFilter::HierarchyT& BusStopFilter::getHierarchy() { 
  return busStopsHierarchy_; 
}

BusStopFilter::Navigator::Navigator(BusStopFilter &f) 
  : filter(f), h(f.busStopsHierarchy_), curr(h.accessRoot()) {}

void BusStopFilter::Navigator::run() {
  while (true) {
    printMenu(); 
    int cmd;
    if (!(std::cin >> cmd)) { 
      std::cin.clear(); 
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
      std::cout << "Bad choice (please enter 0, 1, 2 or 3)\n"; 
      cmd = 4; 
    }
    if      (cmd==0) break;
    else if (cmd==1) goUp();
    else if (cmd==2) chooseChild();
    else if (cmd==3) runPredicates();
    else if (cmd==4) findByID();
    else if (cmd==5) filterVectorMenu();
    else std::cout<<"Unknown\n";
  }
}

void BusStopFilter::Navigator::printMenu() {
  std::cout << "\n=== You are at: [" << nodeDescription(curr) << "]  (has " << h.degree(*curr) << " children)\n"
               "0) quit\n1) go up\n2) go to child #\n3) run level-1 filter here\n4) find stop by ID\n5) filter vector\nChoose: ";
}

std::string BusStopFilter::Navigator::nodeDescription(Block *b) {
  auto &d = b->data_;
  switch (d.getType()) {
    case COMPANY: return "COMPANY:" + d.getName();
    case MUNICIPALITY: return "MUNICIPALITY:" + d.getName();
    case STREET: return "STREET:" + d.getName();
    case BUSSTOP: return "BUSSTOP #" + d.getName();
  }
  return {};
}

void BusStopFilter::Navigator::goUp() {
  auto *p = h.accessParent(*curr);
  if (!p) std::cout<<"Already at root\n";
  else curr = p;
}

void BusStopFilter::Navigator::chooseChild() {
  std::cout<<"Enter child index [0–"<<h.degree(*curr)-1<<"]: ";
  size_t i; std::cin>>i;
  auto *son=h.accessSon(*curr,i);
  if (i < h.degree(*curr) && h.accessSon(*curr, i)) {
    curr = h.accessSon(*curr, i);
  } else {
    std::cout << "Invalid index\n";
  }
}

template<class Pred> 
void BusStopFilter::Navigator::applyPredicate(const std::string &name, Pred p) {
  
  // Use hierarchy iterators directly - Level 2 iterators passed to Level 1 algorithm!
  auto hierarchyBegin = h.begin();  // PreOrderHierarchyIterator from hierarchy
  auto hierarchyEnd = h.end();      // PreOrderHierarchyIterator from hierarchy
  
  // Move iterator to start from current node
  while (hierarchyBegin != hierarchyEnd && &(*hierarchyBegin) != &(curr->data_)) {
    ++hierarchyBegin;
  }
  
  // DIRECT USE OF LEVEL 1 ALGORITHM WITH HIERARCHY ITERATORS
  // Level 2 iterators passed directly to unchanged Level 1 algorithm!
  auto filtered = filter.filterAlgorithm_.template filter<std::vector<BusStopNode>>(
    hierarchyBegin,      // Direct hierarchy iterator - Level 2 iterator!
    hierarchyEnd,        // Direct hierarchy iterator - Level 2 iterator!
    p                    // Node-based predicate that works with BusStopNode objects
  );
  
  std::cout<<"\n-- "<<name<<" : found "<<filtered.size()<<" nodes\n";
  for (const auto &nodeResult : filtered) {
    if (nodeResult.hasBusStop()) {
      const BusStop* stop = nodeResult.getBusStop();
      if (stop) {
        std::cout<<"  ID="<<stop->getStopID()<<"  ("<<stop->getMunicipality()<<", "<<stop->getStreet()<<")  long="<<stop->getLongitude()<<"\n";
      }
    }
  }
}

// Helper methods to reduce code duplication
int BusStopFilter::Navigator::getPredicateChoice() {
  std::cout << "\nPick predicate type:\n1) street-name contains value\n2) municipality == value\n3) region (min/max lat/lon)\nChoice: ";
  int c;
  if (!(std::cin >> c)) {
    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Bad choice (please enter 1, 2, or 3)\n";
    return -1;
  }
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
  return c;
}

std::string BusStopFilter::Navigator::getStreetInput() {
  std::cout << "Enter street name to search for: ";
  std::string st;
  std::getline(std::cin, st);
  return st;
}

std::string BusStopFilter::Navigator::getMunicipalityInput() {
  std::cout << "Enter municipality name: ";
  std::string muni;
  std::getline(std::cin, muni);
  return muni;
}

bool BusStopFilter::Navigator::getRegionInput(double& minLat, double& maxLat, double& minLon, double& maxLon) {
  std::cout << "Enter min latitude: ";
  if (!(std::cin >> minLat)) {
    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid input for min latitude\n";
    return false;
  }
  std::cout << "Enter max latitude: ";
  if (!(std::cin >> maxLat)) {
    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid input for max latitude\n";
    return false;
  }
  std::cout << "Enter min longitude: ";
  if (!(std::cin >> minLon)) {
    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid input for min longitude\n";
    return false;
  }
  std::cout << "Enter max longitude: ";
  if (!(std::cin >> maxLon)) {
    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid input for max longitude\n";
    return false;
  }
  return true;
}

void BusStopFilter::Navigator::runPredicates() {
  int c = getPredicateChoice();
  if (c == -1) return;
  
  switch(c) {
    case 1: {
      std::string st = getStreetInput();
      applyPredicate("Street contains " + st, nodeIsOnStreet(st));
      break;
    }
    case 2: {
      std::string muni = getMunicipalityInput();
      applyPredicate("Municipality == " + muni, nodeIsInMunicipality(muni));
      break;
    }
    case 3: {
      double minLat, maxLat, minLon, maxLon;
      if (getRegionInput(minLat, maxLat, minLon, maxLon)) {
        applyPredicate("Region [" + std::to_string(minLat) + "," + std::to_string(maxLat) + "] x [" + std::to_string(minLon) + "," + std::to_string(maxLon) + "]", nodeIsInRegion(minLat, maxLat, minLon, maxLon));
      }
      break;
    }
    default:
      std::cout << "Bad choice\n";
      break;
  }
}

void BusStopFilter::Navigator::findByID() {
  std::cout << "Enter stop ID: ";
  int id;
  if (!(std::cin >> id)) {
    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid ID input\n";
    return;
  }
  BusStop* bs = filter.findStopByID(id);
  if (bs) {
    std::cout << "Found: ID=" << bs->getStopID() << ", Municipality=" << bs->getMunicipality()
              << ", Street=" << bs->getStreet() << ", Lon=" << bs->getLongitude()
              << ", Lat=" << bs->getLatitude() << "\n";
  } else {
    std::cout << "No stop with ID " << id << "\n";
  }
}

void BusStopFilter::Navigator::filterVectorMenu() {
  int c = getPredicateChoice();
  if (c == -1) return;
  
  std::vector<BusStop> result;
  switch (c) {
    case 1: {
      std::string st = getStreetInput();
      result = filter.filterVector(nodeIsOnStreet(st));
      break;
    }
    case 2: {
      std::string muni = getMunicipalityInput();
      result = filter.filterVector(nodeIsInMunicipality(muni));
      break;
    }
    case 3: {
      double minLat, maxLat, minLon, maxLon;
      if (getRegionInput(minLat, maxLat, minLon, maxLon)) {
        result = filter.filterVector(nodeIsInRegion(minLat, maxLat, minLon, maxLon));
      } else {
        return;
      }
      break;
    }
    default:
      std::cout << "Invalid choice\n";
      return;
  }
  std::cout << "Filtered vector found " << result.size() << " stops:\n";
  for (auto& s : result) {
    std::cout << "  ID=" << s.getStopID() << ", (" << s.getMunicipality() << ", " << s.getStreet() << ")\n";
  }
}

void BusStopFilter::runNavigator() { 
  Navigator nav(*this); 
  nav.run(); 
}

BusStop* BusStopFilter::findStopByID(int id) {
  BusStop** ptrRef = nullptr;
  if (busStopsTable_.tryFind(id, ptrRef)) return *ptrRef;
  return nullptr;
}

template <typename Pred>
std::vector<BusStop> BusStopFilter::filterVector(Pred p) {
    // Use the Level 1 algorithm directly with the BusStopNode vector
    auto filteredNodes = filterAlgorithm_.template filter<std::vector<BusStopNode>>(
        busStopsVec_.begin(), 
        busStopsVec_.end(), 
        p  // Node-based predicate
    );
    
    // Extract BusStop objects from filtered BusStopNode objects
    std::vector<BusStop> result;
    result.reserve(filteredNodes.size());
    for (const auto& node : filteredNodes) {
        if (node.hasBusStop()) {
            const BusStop* stop = node.getBusStop();
            if (stop) {
                result.push_back(*stop);
            }
        }
    }
    
    return result;
}

void BusStopFilter::loadFromCSV(const std::string &fileName) {
  busStopsVec_.clear();
  busStopsHierarchy_.clear();
  busStopsTable_.clear();

  auto &root = busStopsHierarchy_.emplaceRoot();
  root.data_.setType(COMPANY);
  root.data_.setName("GRT");

  std::ifstream file(fileName);
  if (!file.is_open()) {
    throw std::runtime_error("Subor sa nedal otvorit " + fileName);
  }

  {
    std::ifstream countFile(fileName);
    std::string tmp;
    std::getline(countFile, tmp);
    size_t lineCount = 0;
    while (std::getline(countFile, tmp)) ++lineCount;
    busStopsVec_.reserve(lineCount);
  }

  std::string line;
  std::getline(file, line);
  char delim = (line.find(';') != std::string::npos) ? ';' : ',';

  std::string currentMunicipalityName;
  std::string currentStreetName;
  size_t muniIndex = 0, streetIndex = 0;
  typename HierarchyT::BlockType *currMuni = nullptr;
  typename HierarchyT::BlockType *currStreet = nullptr;

  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string substr;
    std::vector<std::string> tokens;
    int i = 0;
    while (std::getline(ss, substr, delim)) {
      ++i;
      if (!(i == 4 || i == 6 || (i >= 10 && i <= 12))) continue;
      substr = substr.empty() ? "0" : substr;
      tokens.push_back(substr);
    }

    BusStop stop(std::stoi(tokens[0]), std::stod(tokens[2]),
                 std::stod(tokens[3]), tokens[1], tokens[4]);

    // Create BusStopNode directly with the BusStop
    BusStopNode node;
    node.setBusStop(stop);
    busStopsVec_.push_back(node);
    
    // Insert pointer to the BusStop inside the node into the table
    const BusStop* stopPtr = busStopsVec_.back().getBusStop();
    if (stopPtr) {
      busStopsTable_.insert(stop.getStopID(), const_cast<BusStop*>(stopPtr));
    }

    if (stop.getMunicipality() != currentMunicipalityName) {
      currentMunicipalityName = stop.getMunicipality();
      currMuni = &busStopsHierarchy_.emplaceSon(root, muniIndex++);
      currMuni->data_.setType(MUNICIPALITY);
      currMuni->data_.setName(currentMunicipalityName);
      currentStreetName.clear();
      streetIndex = 0;
    }
    if (stop.getStreet() != currentStreetName) {
      currentStreetName = stop.getStreet();
      currStreet = &busStopsHierarchy_.emplaceSon(*currMuni, streetIndex++);
      currStreet->data_.setType(STREET);
      currStreet->data_.setName(currentStreetName);
    }
    size_t busStopIndex = currStreet->sons_->size();
    auto &currBusStop = busStopsHierarchy_.emplaceSon(*currStreet, busStopIndex);
    currBusStop.data_.setType(BUSSTOP);
    currBusStop.data_.setName(tokens[0]);
    currBusStop.data_.setBusStop(stop);
  }
}
