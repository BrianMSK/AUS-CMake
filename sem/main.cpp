#include "../libds/amt/explicit_hierarchy.h"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <limits>
#include "../libds/adt/table.h"

class BusStop {
private:
  int StopID;
  double Longitude;
  double Latitude;
  std::string Street;
  std::string Municipality;

public:
  BusStop(int stopID, double longtitude, double lat, std::string street,
          std::string muni)
      : StopID(stopID), Longitude(longtitude), Latitude(lat), Street(street),
        Municipality(muni) {}
  int getStopID() const { return this->StopID; }
  double getLongitude() const { return this->Longitude; }
  double getLatitude() const { return this->Latitude; }
  const std::string &getStreet() const { return this->Street; }
  const std::string &getMunicipality() const { return this->Municipality; }
  bool operator==(const BusStop &o) const {
    return StopID == o.StopID && Longitude == o.Longitude &&
           Latitude == o.Latitude && Street == o.Street &&
           Municipality == o.Municipality;
  }
};

enum BSNTypes { COMPANY, MUNICIPALITY, STREET, BUSSTOP };

class BusStopNode {
private:
  BSNTypes type_;
  std::string name_;
  std::optional<BusStop> busStop_;

public:
  BusStopNode() : type_(COMPANY), name_(), busStop_(std::nullopt) {}

  BSNTypes getType() const { return type_; }
  void setType(BSNTypes t) { type_ = t; }

  const std::string &getName() const { return name_; }
  void setName(const std::string &n) { name_ = n; }
  void setName(std::string &&n) { name_ = std::move(n); }

  bool hasBusStop() const { return busStop_.has_value(); }

  const BusStop *getBusStop() const { return busStop_ ? &*busStop_ : nullptr; }
  BusStop *getBusStop() { return busStop_ ? &*busStop_ : nullptr; }

  void setBusStop(const BusStop &bs) {
    type_ = BUSSTOP;
    busStop_ = bs;
    name_ = std::to_string(bs.getStopID());
  }
  void clearBusStop() { busStop_.reset(); }

  bool operator==(const BusStopNode &o) const {
    return type_ == o.type_ && name_ == o.name_ && busStop_ == o.busStop_;
  }
};

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

// Global predicate functions
auto isOnStreet = [](const std::string& streetName) {
  return [streetName](const BusStop& s) { return s.getStreet().find(streetName) != std::string::npos; };
};

auto isInMunicipality = [](const std::string& municipality) {
  return [municipality](const BusStop& s) { return s.getMunicipality() == municipality; };
};

auto isInRegion = [](double minLat, double maxLat, double minLon, double maxLon) {
  return [minLat, maxLat, minLon, maxLon](const BusStop& s) {
    return s.getLatitude() >= minLat && s.getLatitude() <= maxLat &&
           s.getLongitude() >= minLon && s.getLongitude() <= maxLon;
  };
};

class BusStopFilter {
public:
  using HierarchyT = ds::amt::MultiWayExplicitHierarchy<BusStopNode>;

  template <typename containerT, typename Iterator, typename Predicate>
  containerT filterT(Iterator itBeg_, Iterator itEnd_, Predicate predicate_) {
    containerT structureLoc;
    while (itBeg_ != itEnd_) {
      if (predicate_(*itBeg_)) {
        structureLoc.push_back(*itBeg_);
      }
      ++itBeg_;
    }
    return structureLoc;
  }

  void loadFromCSV(const std::string &fileName);

  const std::vector<BusStop>& getBusStopsVec() const { return busStopsVec_; }
  HierarchyT& getHierarchy() { return busStopsHierarchy_; }

  class Navigator {
  public:
    using Block = typename HierarchyT::BlockType;
    Navigator(BusStopFilter &f) : filter(f), h(f.busStopsHierarchy_), curr(h.accessRoot()) {}
    void run() {
      while (true) {
        printMenu(); int cmd;
        if (!(std::cin >> cmd)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::cout << "Bad choice (please enter 0, 1, 2 or 3)\n"; cmd = 4; }
        if      (cmd==0) break;
        else if (cmd==1) goUp();
        else if (cmd==2) chooseChild();
        else if (cmd==3) runPredicates();
        else if (cmd==4) findByID();
        else if (cmd==5) filterVectorMenu();
        else std::cout<<"Unknown\n";
      }
    }
  private:
    BusStopFilter &filter;
    HierarchyT &h;
    Block *curr;

    void printMenu() {
      std::cout << "\n=== You are at: [" << nodeDescription(curr) << "]  (has " << h.degree(*curr) << " children)\n"
                   "0) quit\n1) go up\n2) go to child #\n3) run level-1 filter here\n4) find stop by ID\n5) filter vector\nChoose: ";
    }
    std::string nodeDescription(Block *b) {
      auto &d = b->data_;
      switch (d.getType()) {
        case COMPANY: return "COMPANY:" + d.getName();
        case MUNICIPALITY: return "MUNICIPALITY:" + d.getName();
        case STREET: return "STREET:" + d.getName();
        case BUSSTOP: return "BUSSTOP #" + d.getName();
      }
      return {};
    }
    
    void goUp() {
      auto *p = h.accessParent(*curr);
      if (!p) std::cout<<"Already at root\n";
      else curr = p;
    }
    
    void chooseChild() {
      std::cout<<"Enter child index [0–"<<h.degree(*curr)-1<<"]: ";
      size_t i; std::cin>>i;
      auto *son=h.accessSon(*curr,i);
      if (i < h.degree(*curr) && h.accessSon(*curr, i)) {
        curr = h.accessSon(*curr, i);
      } else {
        std::cout << "Invalid index\n";
      }
    }
    
    template<class Pred> void applyPredicate(const std::string &name, Pred p) {
      std::vector<BusStop> all;
      collectAllBusStopsUnder(curr, h, all);
      auto filtered = filter.template filterT<std::vector<BusStop>>(all.begin(), all.end(), p);
      std::cout<<"\n-- "<<name<<" : found "<<filtered.size()<<" stops\n";
      for (auto &s:filtered) std::cout<<"  ID="<<s.getStopID()<<"  ("<<s.getMunicipality()<<", "<<s.getStreet()<<")  long="<<s.getLongitude()<<"\n";
    }

    void runPredicates() {
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

    void findByID() {
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

    void filterVectorMenu() {
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
  };

  void runNavigator() { Navigator nav(*this); nav.run(); }

  // Lookup by stop ID
  BusStop* findStopByID(int id) {
    BusStop** ptrRef = nullptr;
    if (busStopsTable_.tryFind(id, ptrRef)) return *ptrRef;
    return nullptr;
  }

  // Filter over the loaded vector
  template <typename Pred>
  std::vector<BusStop> filterVector(Pred p) {
    return filterT<std::vector<BusStop>>(busStopsVec_.begin(), busStopsVec_.end(), p);
  }

private:
  std::vector<BusStop> busStopsVec_;
  HierarchyT busStopsHierarchy_;
  ds::adt::SortedSTab<int, BusStop*> busStopsTable_; 
};

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

int main() {
  BusStopFilter bsFilter;
  bsFilter.loadFromCSV("GRT_Stops_sorted.csv");
  bsFilter.runNavigator();

  return 0;
}

// int checkpoint1() {

//   std::vector<BusStop> locBusStops = loadBusStopsFromCSV("GRT_Stops.csv");
//   std::string municipality = "Kitchener";
//   std::string street = "Regina St";
//   double minLat = 43.4;
//   double maxLat = 43.6;
//   double minLong = -80.5;
//   double maxLong = -80.3;

//   auto isMunicipality = [municipality](const BusStop &stop_) -> bool {
//     return municipality == stop_.getMunicipality();
//   };
//   auto isOnStreet = [street](const BusStop &stop_) -> bool {
//     return stop_.getStreet().find(street) != std::string::npos;
//   };
//   auto isInRegion = [minLat, maxLat, minLong,
//                      maxLong](const BusStop &stop_) -> bool {
//     return stop_.getLongitude() >= minLong && stop_.getLongitude() <= maxLong &&
//            stop_.getLatitude() >= minLat && stop_.getLatitude() <= maxLat;
//   };

//   BusStopFilter BSF;
//   std::vector<BusStop> filtByMunic = BSF.filterT<std::vector<BusStop>>(
//       locBusStops.begin(), locBusStops.end(), isMunicipality);
//   std::vector<BusStop> filtByStreet = BSF.filterT<std::vector<BusStop>>(
//       locBusStops.begin(), locBusStops.end(), isOnStreet);
//   std::vector<BusStop> filtByRegion = BSF.filterT<std::vector<BusStop>>(
//       locBusStops.begin(), locBusStops.end(), isInRegion);

//   int i = 0;
//   for (const auto &var : filtByStreet) {
//     ++i;
//     std::cout << "ID: " << var.getStopID() << ", Street: " << var.getStreet()
//               << ", Municipality: " << var.getMunicipality()
//               << ", Longitude: " << var.getLongitude()
//               << ", Latitude: " << var.getLatitude() << std::endl;
//   }
//   std::cout << "Pocet zaznamov: " << i << std::endl;
//   return 0;
// }
