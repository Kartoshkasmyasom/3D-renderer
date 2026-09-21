#pragma once

#include <string>

namespace Renderer {

enum class TokenType {
  Word,
  Number,
  Path,
};

struct Token {
  TokenType type;
  std::string text;
};

}  // namespace Renderer
