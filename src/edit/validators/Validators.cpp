#include "Validators.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <optional>
#include <sstream>
#include <string>

namespace Renderer {
namespace {

std::optional<float> parse_float(const Token& token) {
  if (token.type != TokenType::Number) {
    return std::nullopt;
  }

  std::istringstream stream(token.text);
  float value = 0.0f;
  stream >> value;
  return value;
}

bool in_range(float value, float min, float max) {
  return value >= min && value <= max;
}

std::optional<std::array<float, 3>> parse_float_triplet(
    const std::vector<Token>& tokens, float min, float max) {
  if (tokens.size() != 3) {
    return std::nullopt;
  }

  std::array<float, 3> values{};
  for (std::size_t index = 0; index < tokens.size(); ++index) {
    const auto value = parse_float(tokens[index]);
    if (!value.has_value() || !in_range(*value, min, max)) {
      return std::nullopt;
    }
    values[index] = *value;
  }

  return values;
}

std::string to_lower(std::string value) {
  std::transform(value.begin(), value.end(), value.begin(),
                 [](unsigned char symbol) {
                   return static_cast<char>(std::tolower(symbol));
                 });
  return value;
}

bool has_obj_extension(const std::filesystem::path& path) {
  return to_lower(path.extension().string()) == ".obj";
}

bool is_readable_regular_file(const std::filesystem::path& path) {
  std::error_code error;
  if (!std::filesystem::exists(path, error) || error) {
    return false;
  }
  if (!std::filesystem::is_regular_file(path, error) || error) {
    return false;
  }

  std::ifstream file(path);
  return file.good();
}

}  // namespace

std::optional<Validator<ConsoleAction::ImportObject>::Request> Validator<
    ConsoleAction::ImportObject>::validate(const std::vector<Token>& tokens) {
  if (tokens.size() != 1) {
    return std::nullopt;
  }

  if (tokens.front().type != TokenType::Path &&
      tokens.front().type != TokenType::Word) {
    return std::nullopt;
  }

  const std::filesystem::path path(tokens.front().text);
  if (!has_obj_extension(path) || !is_readable_regular_file(path)) {
    return std::nullopt;
  }

  return Request{path.string()};
}

std::optional<Validator<ConsoleAction::ChangeColor>::Request> Validator<
    ConsoleAction::ChangeColor>::validate(const std::vector<Token>& tokens) {
  const auto values = parse_float_triplet(tokens, 0.0f, 255.0f);
  if (!values.has_value()) {
    return std::nullopt;
  }
  return Request{(*values)[0], (*values)[1], (*values)[2]};
}

std::optional<Validator<ConsoleAction::MoveObject>::Request> Validator<
    ConsoleAction::MoveObject>::validate(const std::vector<Token>& tokens) {
  const auto values = parse_float_triplet(tokens, -1000.0f, 1000.0f);
  if (!values.has_value()) {
    return std::nullopt;
  }
  return Request{(*values)[0], (*values)[1], (*values)[2]};
}

std::optional<Validator<ConsoleAction::RotateObject>::Request> Validator<
    ConsoleAction::RotateObject>::validate(const std::vector<Token>& tokens) {
  const auto values = parse_float_triplet(tokens, 0.0f, 360.0f);
  if (!values.has_value()) {
    return std::nullopt;
  }
  return Request{(*values)[0], (*values)[1], (*values)[2]};
}

std::optional<Validator<ConsoleAction::ScaleObject>::Request> Validator<
    ConsoleAction::ScaleObject>::validate(const std::vector<Token>& tokens) {
  const auto values = parse_float_triplet(tokens, 0.1f, 10.0f);
  if (!values.has_value()) {
    return std::nullopt;
  }
  return Request{(*values)[0], (*values)[1], (*values)[2]};
}

std::optional<Validator<ConsoleAction::DeleteObject>::Request> Validator<
    ConsoleAction::DeleteObject>::validate(const std::vector<Token>& tokens) {
  if (tokens.size() != 1) {
    return std::nullopt;
  }
  if (to_lower(tokens.front().text) != "y") {
    return std::nullopt;
  }
  return Request{};
}

}  // namespace Renderer
