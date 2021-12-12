#include "Sources/fLib/global-functions.h"

#include <fstream>

//#define BUILDING

bool DEBUGGING = false;
unsigned BUILD_VER;

void Build() {
  FILE* f = nullptr; char buffer[25];
  f = fopen(R"(..\Build.ver)", "r+");
  fread(buffer, sizeof(unsigned), 1, f);

  unsigned build = atoi(buffer);
  BUILD_VER = ++build;
  unsigned bits = 0, t = build;

  while (t != 0) {
    if (t & 1) bits++;
    t >>= 1;
  }

  unsigned size = bits / 8;
  if ((bits % 8) != 0) size++;

  fseek(f, 0, 0);
  fwrite(itoa(build, buffer, 10), size, 1, f);
  fclose(f);
}

void Build2() {
  FILE* f = nullptr; char buffer[25];
  f = fopen(R"(..\Build.ver)", "r+");
  fread(buffer, sizeof(unsigned), 1, f);

  unsigned build = atoi(buffer);
  BUILD_VER = build;
  unsigned bits = 0, t = build;

  while (t != 0) {
    if (t & 1) bits++;
    t >>= 1;
  }

  unsigned size = bits / 8;
  if ((bits % 8) != 0) size++;

  fseek(f, 0, 0);
  fwrite(itoa(build, buffer, 10), size, 1, f);
  fclose(f);
}

int main(int argc, char** argv) {

#if defined(BUILDING)
  Build();
#else
  Build2();
#endif

  global::ProcessParams(argc, argv);

  return 0;
}
