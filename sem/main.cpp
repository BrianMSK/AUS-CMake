#include <fstream>
#include <iostream>
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
  std::vector<BusStop> locBusStops = loadBusStopsFromCSV("GRT_Stops_orig.csv");

  std::string municipality = "Kitchener";
  std::string street = "King St";
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
  for (const auto &var : filtByMunic) {
    ++i;
    std::cout << "ID: " << var.getStopID() << ", Street: " << var.getStreet()
              << ", Municipality: " << var.getMunicipality()
              << ", Longitude: " << var.getLongitude()
              << ", Latitude: " << var.getLatitude() << std::endl;
  }
  std::cout << "Pocet zaznamov: " << i << std::endl;
  return 0;
}
