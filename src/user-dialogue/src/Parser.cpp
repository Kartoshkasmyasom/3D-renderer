#include "Parser.hpp"

#include <cmath>

namespace Renderer {
namespace {

bool is_number(const std::string& text) {
  if (text.empty()) {
    return false;
  }

  std::istringstream stream(text);
  float value = 0.0f;
  stream >> value;

  return stream && stream.eof() && std::isfinite(value);
}

bool looks_like_path(const std::string& text) {
  return text.find('/') != std::string::npos ||
         text.find('\\') != std::string::npos ||
         text.find('.') != std::string::npos;
}

}  // namespace

std::vector<Token> Parser::tokenize(const std::string& input) const {
  std::istringstream stream(input);
  std::string part;
  std::vector<Token> tokens;

  while (stream >> part) {
    tokens.push_back(Token{classify(part), part});
  }

  return tokens;
}

TokenType Parser::classify(const std::string& part) const {
  if (is_number(part)) {
    return TokenType::Number;
  }
  if (looks_like_path(part)) {
    return TokenType::Path;
  }
  return TokenType::Word;
}
}  // namespace Renderer
