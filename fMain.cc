//#include "Sources/fLib/global-functions.hpp"
#include "Sources/fParser/AST.hpp"

//#define BUILDING

bool DEBUGGING = false;
const char* BUILD_VER = "20220212";

int main(int argc, char** argv) {

  global::ProcessParams(argc, argv);

  ast::Node new_;
  new_.Print();

  return 0;
}
