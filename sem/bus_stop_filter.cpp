#include "bus_stop_filter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <limits>

template <typename containerT, typename Iterator, typename Predicate>
containerT BusStopFilter::filterT(Iterator itBeg_, Iterator itEnd_, Predicate predicate_) {
  containerT structureLoc;
  while (itBeg_ != itEnd_) {
    if (predicate_(*itBeg_)) {
      structureLoc.push_back(*itBeg_);
    }
    ++itBeg_;
  }
  return structureLoc;
}

const std::vector<BusStop>& BusStopFilter::getBusStopsVec() const { 
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
  std::vector<BusStop> all;
  collectAllBusStopsUnder(curr, h, all);
  auto filtered = filter.template filterT<std::vector<BusStop>>(all.begin(), all.end(), p);
  std::cout<<"\n-- "<<name<<" : found "<<filtered.size()<<" stops\n";
  for (auto &s:filtered) std::cout<<"  ID="<<s.getStopID()<<"  ("<<s.getMunicipality()<<", "<<s.getStreet()<<")  long="<<s.getLongitude()<<"\n";
}

void BusStopFilter::Navigator::runPredicates() {
  std::cout << "\nPick predicate type:\n1) street-name contains value\n2) municipality == value\n3) region (min/max lat/lon)\nChoice: ";
  int c;
  if (!(std::cin >> c)) {
    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Bad choice (please enter 1, 2, or 3)\n";
    return;
  }
  
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
  
  switch(c) {
    case 1: {
      std::cout << "Enter street name to search for: ";
      std::string st;
      std::getline(std::cin, st);
      applyPredicate("Street contains " + st, isOnStreet(st));
      break;
    }
    case 2: {
      std::cout << "Enter municipality name: ";
      std::string muni;
      std::getline(std::cin, muni);
      applyPredicate("Municipality == " + muni, isInMunicipality(muni));
      break;
    }
    case 3: {
      std::cout << "Enter min latitude: ";
      double minLat;
      if (!(std::cin >> minLat)) {
        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input for min latitude\n";
        return;
      }
      std::cout << "Enter max latitude: ";
      double maxLat;
      if (!(std::cin >> maxLat)) {
        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input for max latitude\n";
        return;
      }
      std::cout << "Enter min longitude: ";
      double minLon;
      if (!(std::cin >> minLon)) {
        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input for min longitude\n";
        return;
      }
      std::cout << "Enter max longitude: ";
      double maxLon;
      if (!(std::cin >> maxLon)) {
        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input for max longitude\n";
        return;
      }
      applyPredicate("Region [" + std::to_string(minLat) + "," + std::to_string(maxLat) + "] x [" + std::to_string(minLon) + "," + std::to_string(maxLon) + "]", isInRegion(minLat, maxLat, minLon, maxLon));
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
  std::cout << "\nPick vector filter type:\n"
               "1) street contains value\n"
               "2) municipality == value\n"
               "3) region (min/max lat/lon)\n"
               "Choice: ";
  int c;
  if (!(std::cin >> c)) {
    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid choice\n";
    return;
  }
  
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
  
  std::vector<BusStop> result;
  switch (c) {
    case 1: {
      std::cout << "Enter street name to search for: ";
      std::string st;
      std::getline(std::cin, st);
      result = filter.filterVector(isOnStreet(st));
      break;
    }
    case 2: {
      std::cout << "Enter municipality name: ";
      std::string muni;
      std::getline(std::cin, muni);
      result = filter.filterVector(isInMunicipality(muni));
      break;
    }
    case 3: {
      std::cout << "Enter min latitude: ";
      double minLat;
      if (!(std::cin >> minLat)) {
        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input for min latitude\n";
        return;
      }
      std::cout << "Enter max latitude: ";
      double maxLat;
      if (!(std::cin >> maxLat)) {
        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input for max latitude\n";
        return;
      }
      std::cout << "Enter min longitude: ";
      double minLon;
      if (!(std::cin >> minLon)) {
        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input for min longitude\n";
        return;
      }
      std::cout << "Enter max longitude: ";
      double maxLon;
      if (!(std::cin >> maxLon)) {
        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input for max longitude\n";
        return;
      }
      result = filter.filterVector(isInRegion(minLat, maxLat, minLon, maxLon));
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
  return filterT<std::vector<BusStop>>(busStopsVec_.begin(), busStopsVec_.end(), p);
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

    busStopsVec_.push_back(stop);
    busStopsTable_.insert(stop.getStopID(), &busStopsVec_.back());

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
