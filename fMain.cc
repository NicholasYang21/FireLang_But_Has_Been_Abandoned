#include "Sources/fLib/global-functions.hpp"

//#define BUILDING

bool DEBUGGING = false;
const char* BUILD_VER = "5d10b0b";

int main(int argc, char** argv) {

  global::ProcessParams(argc, argv);

  return 0;
}
