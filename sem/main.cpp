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
  // convenience for leaf
  explicit BusStopNode(BusStop bs)
      : type_(BUSSTOP),
        name_(std::to_string(
            bs.getStopID())), // or bs.getStreet(), whatever you prefer
        busStop_(std::move(bs)) {}

  // --- getters & setters for type
  BSNTypes getType() const { return type_; }
  void setType(BSNTypes t) { type_ = t; }

  // --- getters & setters for name
  const std::string &getName() const { return name_; }
  void setName(const std::string &n) { name_ = n; }
  void setName(std::string &&n) { name_ = std::move(n); }

  // --- busStop management
  bool hasBusStop() const { return busStop_.has_value(); }

  // returns nullptr if none
  const BusStop *getBusStop() const { return busStop_ ? &*busStop_ : nullptr; }
  BusStop *getBusStop() { return busStop_ ? &*busStop_ : nullptr; }

  // set or overwrite the BusStop (makes this a BUSSTOP node)
  void setBusStop(const BusStop &bs) {
    type_ = BUSSTOP;
    busStop_ = bs;
    name_ = std::to_string(bs.getStopID());
  }
  void clearBusStop() { busStop_.reset(); }

  // --- equality
  bool operator==(const BusStopNode &o) const {
    return type_ == o.type_ && name_ == o.name_ &&
           busStop_ ==
               o.busStop_; // optional<T>::operator== does the right thing
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

  // Declare hiearchy and create local busStopNode_
  ds::amt::MultiWayExplicitHierarchy<BusStopNode> busStopsHierarchy;
  BusStopNode busStopNode_;
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

  // ROOT SET
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

int main() {
  std::vector<BusStop> locBusStops = loadBusStopsFromCSV("GRT_Stops.csv");
  auto locBusStopsHiearchy =
      loadBusStopsHiearchyFromCSV("GRT_Stops_sorted.csv");
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
