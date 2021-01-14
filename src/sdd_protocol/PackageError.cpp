#include "sdd_protocol/PackageError.h"
using namespace sdd;

PackageError::PackageError(const std::string &what) : std::runtime_error(what) {

}

PackageParseError::PackageParseError(const std::string &what) : PackageError(what){

}

PackageControlSumError::PackageControlSumError(const std::string &what)  : PackageParseError(what)
{}