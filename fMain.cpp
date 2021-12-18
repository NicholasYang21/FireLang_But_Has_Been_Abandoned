#include "Sources/fLib/global-functions.h"

//#define BUILDING

bool DEBUGGING = false;
const char* BUILD_VER = "d856b2b";

int main(int argc, char** argv) {

  global::ProcessParams(argc, argv);

  return 0;
}
