#include "WrapperSHA1.h"

#include <memory>

#include "Sha1.hpp"


std::string cipher::sha1Hash(const std::string& value)
{
  auto checksum = std::make_shared<SHA1>();
  checksum->update(value);
  return checksum->final();
}
