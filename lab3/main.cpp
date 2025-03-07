#include <iostream>
#include <libds/mm/compact_memory_manager.h>

struct Osoba {
  int id;
  short vek;
  std::string name;
  Osoba &operator=(const Osoba &) = default;
};

struct Student : public Osoba {
  double average;
  std::vector<char> grades;
  Student &operator=(const Student &) = default;
};

int main() {
  ds::mm::CompactMemoryManager<int> cmm_int;
  int *p1 = cmm_int.allocateMemory();
  *p1 = 10;
  *cmm_int.allocateMemory() = 20;
  cmm_int.print(std::cout);

  ds::mm::CompactMemoryManager<Osoba> cmm_osoba;
  *cmm_osoba.allocateMemory() = {10, 20, "Jonas Peterek"};
  cmm_osoba.print(std::cout);

  ds::mm::CompactMemoryManager<Student> cmm_student;
  *cmm_student.allocateMemory() = {10, 2, "Denis", 2.3, {'A', 'B', 'C'}};
  cmm_student.print(std::cout);
  return 0;
}
