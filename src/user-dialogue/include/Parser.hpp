#pragma once

#include "Token.hpp"

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

namespace Renderer {

class Parser {
 public:
  Parser() = default;
  std::vector<Token> tokenize(const std::string& input) const;

 private:
  TokenType classify(const std::string& part) const;
};

}  // namespace Renderer
