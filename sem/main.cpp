#include <fstream>
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
};

std::vector<BusStop> loadDataFromCSV(const std::string fileName) {
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
      tokens.push_back(substr);
    }
    auto stop = BusStop(std::stoi(tokens[0]), std::stod(tokens[2]),
                        std::stod(tokens[3]), tokens[1], tokens[4]);
    busStopsVec.push_back(stop);
  }
  return busStopsVec;
}

int main() {
  loadDataFromCSV("GRT_Stops_orig.csv");
  return 0;
}
