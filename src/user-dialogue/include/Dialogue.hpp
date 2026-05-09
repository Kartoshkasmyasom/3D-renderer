#pragma once

#include "Parser.hpp"
#include "Token.hpp"

#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace Renderer {

class Dialogue {
 public:
  Dialogue() = default;
  void print_message(std::string_view message) const;

  template <typename Messages, typename Validator>
  auto get_user_request(const Messages&, const Validator&)
      -> std::optional<typename Validator::Request> {
    std::cout << Messages::hint << std::endl;

    while (true) {
      std::cout << "> ";

      std::string input;
      if (!std::getline(std::cin, input)) {
        std::cin.clear();
        std::cout << "Input error. Try again.\n";
        continue;
      }

      if (input == "exit") {
        std::cout << "Input cancelled.\n";
        return std::nullopt;
      }

      Parser parser;
      auto request = Validator::validate(parser.tokenize(input));
      if (request.has_value()) {
        return request;
      }

      std::cout << Messages::failure << std::endl;
    }
  }
};

}  // namespace Renderer
