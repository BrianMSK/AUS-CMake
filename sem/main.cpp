#include "bus_stop_filter.h"

int main() {
  BusStopFilter bsFilter;
  bsFilter.loadFromCSV("GRT_Stops_sorted.csv");
  bsFilter.runNavigator();

  return 0;
}
