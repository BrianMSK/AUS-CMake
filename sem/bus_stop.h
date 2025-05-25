#pragma once

#include <string>

class BusStop {
private:
  int StopID;
  double Longitude;
  double Latitude;
  std::string Street;
  std::string Municipality;

public:
  BusStop(int stopID, double longtitude, double lat, std::string street,
          std::string muni);
  
  int getStopID() const;
  double getLongitude() const;
  double getLatitude() const;
  const std::string &getStreet() const;
  const std::string &getMunicipality() const;
  
  bool operator==(const BusStop &o) const;
};
