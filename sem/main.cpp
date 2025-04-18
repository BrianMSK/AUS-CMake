#include "../libds/amt/explicit_hierarchy.h"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <vector>

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
  explicit BusStopNode(BSNTypes t, std::string n = {})
      : type_(t), name_(std::move(n)), busStop_(std::nullopt) {}
  explicit BusStopNode(BusStop bs)
      : type_(BUSSTOP), name_(std::to_string(bs.getStopID())),
        busStop_(std::move(bs)) {}

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

class BusStopFilter {
public:
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
};

ds::amt::MultiWayExplicitHierarchy<BusStopNode>
loadBusStopsHiearchyFromCSV(const std::string fileName) {

  ds::amt::MultiWayExplicitHierarchy<BusStopNode> busStopsHierarchy;
  std::string currentMunicipalityName = "";
  std::string currentStreetName = "";
  size_t muniIndex_ = 0;
  size_t streetIndex_ = 0;
  auto currMuni =
      static_cast<ds::amt::MultiWayExplicitHierarchy<BusStopNode>::BlockType *>(
          nullptr);
  auto currStreet =
      static_cast<ds::amt::MultiWayExplicitHierarchy<BusStopNode>::BlockType *>(
          nullptr);

  auto &root = busStopsHierarchy.emplaceRoot();
  root.data_.setType(COMPANY);
  root.data_.setName("GRT");

  std::ifstream file(fileName);

  if (!file.is_open()) {
    throw std::runtime_error("Subor sa nedal otvorit " + fileName);
  }

  std::string line;
  std::getline(file, line);

  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string substr;
    std::vector<std::string> tokens;
    int i = 0;
    while (getline(ss, substr, ';')) {
      ++i;
      if (!(i == 4 || i == 6 || (i >= 10 && i <= 12))) {
        continue;
      }
      substr = substr.empty() ? "0" : substr;
      tokens.push_back(substr);
    }
    auto stop = BusStop(std::stoi(tokens[0]), std::stod(tokens[2]),
                        std::stod(tokens[3]), tokens[1], tokens[4]);
    if (stop.getMunicipality() != currentMunicipalityName) {
      currentMunicipalityName = stop.getMunicipality();
      currMuni = &busStopsHierarchy.emplaceSon(root, muniIndex_);
      ++muniIndex_;
      currMuni->data_.setType(MUNICIPALITY);
      currMuni->data_.setName(currentMunicipalityName);

      currentStreetName = "";
      streetIndex_ = 0;
    }
    if (stop.getStreet() != currentStreetName) {
      currentStreetName = stop.getStreet();
      currStreet = &busStopsHierarchy.emplaceSon(*currMuni, streetIndex_);
      ++streetIndex_;
      currStreet->data_.setType(STREET);
      currStreet->data_.setName(currentStreetName);
    }
    size_t busStopIndex = currStreet->sons_->size();
    auto &currBusStop = busStopsHierarchy.emplaceSon(*currStreet, busStopIndex);
    currBusStop.data_.setType(BUSSTOP);
    currBusStop.data_.setName(tokens[0]);
    currBusStop.data_.setBusStop(stop);
  }
  return busStopsHierarchy;
}

std::vector<BusStop> loadBusStopsFromCSV(const std::string fileName) {
  std::vector<BusStop> busStopsVec;
  std::ifstream file(fileName);

  if (!file.is_open()) {
    throw std::runtime_error("Subor sa nedal otvorit " + fileName);
  }

  std::string line;
  std::getline(file, line);

  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string substr;
    std::vector<std::string> tokens;
    int i = 0;
    while (getline(ss, substr, ',')) {
      ++i;
      if (!(i == 4 || i == 6 || (i >= 10 && i <= 12))) {
        continue;
      }
      substr = substr.empty() ? "0" : substr;
      tokens.push_back(substr);
    }
    auto stop = BusStop(std::stoi(tokens[0]), std::stod(tokens[2]),
                        std::stod(tokens[3]), tokens[1], tokens[4]);
    busStopsVec.push_back(stop);
  }
  return busStopsVec;
}

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

class HierarchyNavigator {
public:
  using HierarchyT = ds::amt::MultiWayExplicitHierarchy<BusStopNode>;
  using Block = HierarchyT::BlockType;

  HierarchyNavigator(HierarchyT &h) : _h(h), _curr(h.accessRoot()) {}

  void run() {
    while (true) {
      printMenu();
      int cmd;
      std::cin >> cmd;
      if (cmd == 0)
        break;
      else if (cmd == 1)
        goUp();
      else if (cmd == 2)
        chooseChild();
      else if (cmd == 3)
        runPredicates();
      else
        std::cout << "Unknown\n";
    }
  }

private:
  HierarchyT &_h;
  Block *_curr;

  void printMenu() {
    std::cout << "\n=== You are at: [" << nodeDescription(_curr) << "]  (has "
              << _h.degree(*_curr)
              << " children)\n"
                 "0) quit\n"
                 "1) go up\n"
                 "2) go to child #\n"
                 "3) run level‑1 filter here\n"
                 "Choose: ";
  }

  std::string nodeDescription(Block *b) {
    auto &d = b->data_;
    switch (d.getType()) {
    case COMPANY:
      return std::string("COMPANY:") + d.getName();
    case MUNICIPALITY:
      return std::string("MUNICIPALITY:") + d.getName();
    case STREET:
      return std::string("STREET:") + d.getName();
    case BUSSTOP:
      return std::string("BUSSTOP #") + d.getName();
    }
    return {};
  }

  void goUp() {
    auto *p = _h.accessParent(*_curr);
    if (!p)
      std::cout << "Already at root\n";
    else
      _curr = p;
  }

  void chooseChild() {
    std::cout << "Enter child index [0–" << _h.degree(*_curr) - 1 << "]: ";
    size_t idx;
    std::cin >> idx;
    if (idx < _h.degree(*_curr) && _h.accessSon(*_curr, idx)) {
      _curr = _h.accessSon(*_curr, idx);
    } else {
      std::cout << "Invalid index\n";
    }
  }

  template <class Pred> void applyPredicate(const std::string &name, Pred p) {
    std::vector<BusStop> all;
    collectAllBusStopsUnder(_curr, _h, all);
    BusStopFilter F;
    auto filtered = F.filterT<std::vector<BusStop>>(all.begin(), all.end(), p);
    std::cout << "\n-- " << name << " : found " << filtered.size()
              << " stops\n";
    for (auto &s : filtered) {
      std::cout << "  ID=" << s.getStopID() << "  (" << s.getMunicipality()
                << ", " << s.getStreet() << ")  long=" << s.getLongitude()
                << "\n";
    }
  }

  void runPredicates() {
    std::cout << "\nPick predicate:\n"
                 "1) longitude < -80.4841\n"
                 "2) street-name contains \"Ave\"\n"
                 "3) municipality == \"Kitchener\"\n"
                 "Choice: ";
    int c;
    if (!(std::cin >> c)) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Bad choice (please enter 1, 2 or 3)\n";
      return;
    }
    switch (c) {
    case 1:
      applyPredicate("Lon < -80.4841",
                     [](auto &s) { return s.getLongitude() < -80.4841; });
      break;
    case 2:
      applyPredicate("Street contains Ave", [](auto &s) {
        return s.getStreet().find("Ave") != std::string::npos;
      });
      break;
    case 3:
      applyPredicate("Municipality==Kitchener", [](auto &s) {
        return s.getMunicipality() == "Kitchener";
      });
      break;
    default:
      std::cout << "Bad choice\n";
      break;
    }
  }
};

int main() {
  auto locBusStopsHiearchy =
      loadBusStopsHiearchyFromCSV("GRT_Stops_sorted.csv");

  HierarchyNavigator nav(locBusStopsHiearchy);
  nav.run();

  return 0;
}

int backup() {

  std::vector<BusStop> locBusStops = loadBusStopsFromCSV("GRT_Stops.csv");
  std::string municipality = "Kitchener";
  std::string street = "Regina St";
  double minLat = 43.4;
  double maxLat = 43.6;
  double minLong = -80.5;
  double maxLong = -80.3;

  auto isMunicipality = [municipality](const BusStop &stop_) -> bool {
    return municipality == stop_.getMunicipality();
  };
  auto isOnStreet = [street](const BusStop &stop_) -> bool {
    return stop_.getStreet().find(street) != std::string::npos;
  };
  auto isInRegion = [minLat, maxLat, minLong,
                     maxLong](const BusStop &stop_) -> bool {
    return stop_.getLongitude() >= minLong && stop_.getLongitude() <= maxLong &&
           stop_.getLatitude() >= minLat && stop_.getLatitude() <= maxLat;
  };

  BusStopFilter BSF;
  std::vector<BusStop> filtByMunic = BSF.filterT<std::vector<BusStop>>(
      locBusStops.begin(), locBusStops.end(), isMunicipality);
  std::vector<BusStop> filtByStreet = BSF.filterT<std::vector<BusStop>>(
      locBusStops.begin(), locBusStops.end(), isOnStreet);
  std::vector<BusStop> filtByRegion = BSF.filterT<std::vector<BusStop>>(
      locBusStops.begin(), locBusStops.end(), isInRegion);

  int i = 0;
  for (const auto &var : filtByStreet) {
    ++i;
    std::cout << "ID: " << var.getStopID() << ", Street: " << var.getStreet()
              << ", Municipality: " << var.getMunicipality()
              << ", Longitude: " << var.getLongitude()
              << ", Latitude: " << var.getLatitude() << std::endl;
  }
  std::cout << "Pocet zaznamov: " << i << std::endl;
  return 0;
}
