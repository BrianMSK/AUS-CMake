#include "bus_stop.h"

BusStop::BusStop(int stopID, double longtitude, double lat, std::string street,
        std::string muni)
    : StopID(stopID), Longitude(longtitude), Latitude(lat), Street(street),
      Municipality(muni) {}

int BusStop::getStopID() const { return this->StopID; }

double BusStop::getLongitude() const { return this->Longitude; }

double BusStop::getLatitude() const { return this->Latitude; }

const std::string &BusStop::getStreet() const { return this->Street; }

const std::string &BusStop::getMunicipality() const { return this->Municipality; }

bool BusStop::operator==(const BusStop &o) const {
  return StopID == o.StopID && Longitude == o.Longitude &&
         Latitude == o.Latitude && Street == o.Street &&
         Municipality == o.Municipality;
}
