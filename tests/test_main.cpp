#include <array>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "Action.hpp"
#include "Dialogue.hpp"
#include "MessageTraits.hpp"
#include "Parser.hpp"
#include "Validators.hpp"
#include "World.hpp"
#include "commands/Commands.hpp"

namespace {

using Renderer::Action;
using Renderer::Command;
using Renderer::Dialogue;
using Renderer::MessageTraits;
using Renderer::MoveObjectRequest;
using Renderer::Parser;
using Renderer::Token;
using Renderer::TokenType;
using Renderer::TriangulatedObject;
using Renderer::Validator;
using Renderer::Vector3;
using Renderer::World;

class StreamBufferRedirect {
 public:
  StreamBufferRedirect(std::ios& stream, std::streambuf* replacement)
      : stream_(stream), original_(stream.rdbuf(replacement)) {
    stream_.clear();
  }

  ~StreamBufferRedirect() {
    stream_.rdbuf(original_);
    stream_.clear();
  }

  StreamBufferRedirect(const StreamBufferRedirect&) = delete;
  StreamBufferRedirect& operator=(const StreamBufferRedirect&) = delete;

 private:
  std::ios& stream_;
  std::streambuf* original_;
};

void require(bool condition, const std::string& message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

void require_near(float actual, float expected, const std::string& message) {
  require(std::fabs(actual - expected) < 0.0001f, message);
}

Token word(const std::string& text) { return Token{TokenType::Word, text}; }

Token number(const std::string& text) { return Token{TokenType::Number, text}; }

Token path(const std::string& text) { return Token{TokenType::Path, text}; }

struct RangeCase {
  float value;
  bool accepted;
};

template <Action action>
void check_triplet_range(
    float minimum, float maximum, std::vector<RangeCase> cases,
    const std::array<float Validator<action>::Request::*, 3>& fields) {
  constexpr bool whole_numbers = action == Action::ChangeColor;
  const float middle = whole_numbers ? std::floor((minimum + maximum) / 2.0f)
                                     : (minimum + maximum) / 2.0f;
  const float infinity = std::numeric_limits<float>::infinity();
  cases.insert(cases.end(), {
      {middle, true},
      {minimum, true},
      {maximum, true},
      {std::nextafter(minimum, -infinity), false},
      {std::nextafter(minimum, maximum), !whole_numbers},
      {std::nextafter(maximum, minimum), !whole_numbers},
      {std::nextafter(maximum, infinity), false},
      {std::numeric_limits<float>::lowest(), false},
      {std::numeric_limits<float>::max(), false},
  });

  const auto encode = [](float value) {
    std::ostringstream text;
    text << std::setprecision(std::numeric_limits<float>::max_digits10)
         << value;
    return text.str();
  };

  for (const auto& test : cases) {
    for (std::size_t axis = 0; axis < fields.size(); ++axis) {
      // Keep the other components valid to detect missing per-axis checks.
      std::vector<Token> tokens(3, number(encode(middle)));
      tokens[axis] = number(encode(test.value));
      const std::string context = "component " + std::to_string(axis) +
                                  ", value " + tokens[axis].text;
      const auto request = Validator<action>::validate(tokens);
      require(request.has_value() == test.accepted,
              context + (test.accepted ? " should be accepted"
                                       : " should be rejected"));
      if (request) {
        for (std::size_t component = 0; component < fields.size(); ++component) {
          const float expected = component == axis ? test.value : middle;
          require((*request).*fields[component] == expected,
                  context + ": parsed component " +
                      std::to_string(component) + " should be preserved");
        }
      }
    }
  }
}

void change_color_checks_range_boundaries() {
  using Request = Validator<Action::ChangeColor>::Request;
  check_triplet_range<Action::ChangeColor>(
      0.0f, 255.0f,
      {{-1.0f, false}, {1.0f, true}, {254.0f, true}, {256.0f, false},
       {-100.0f, false}, {1000.0f, false}, {0.5f, false},
       {127.5f, false}, {254.5f, false}},
      {&Request::r, &Request::g, &Request::b});
}

void move_object_checks_range_boundaries() {
  using Request = Validator<Action::MoveObject>::Request;
  check_triplet_range<Action::MoveObject>(
      -1000.0f, 1000.0f,
      {{-1001.0f, false}, {-999.0f, true}, {999.0f, true}, {1001.0f, false},
       {-10000.0f, false}, {10000.0f, false}},
      {&Request::x, &Request::y, &Request::z});
}

void rotate_object_checks_range_boundaries() {
  using Request = Validator<Action::RotateObject>::Request;
  check_triplet_range<Action::RotateObject>(
      0.0f, 360.0f,
      {{-1.0f, false}, {1.0f, true}, {359.0f, true}, {361.0f, false},
       {-720.0f, false}, {720.0f, false}},
      {&Request::x_degrees, &Request::y_degrees, &Request::z_degrees});
}

void scale_object_checks_range_boundaries() {
  using Request = Validator<Action::ScaleObject>::Request;
  check_triplet_range<Action::ScaleObject>(
      0.1f, 10.0f,
      {{-0.9f, false}, {1.1f, true}, {9.0f, true}, {11.0f, false},
       {-100.0f, false}, {100.0f, false}, {0.0f, false}},
      {&Request::x, &Request::y, &Request::z});
}

void parser_classifies_words_numbers_and_paths() {
  const Parser parser;
  const auto tokens = parser.tokenize("move 10 -2.5 ./models/cube.obj");

  require(tokens.size() == 4, "parser should create four tokens");
  require(tokens[0].type == TokenType::Word && tokens[0].text == "move",
          "parser should classify words");
  require(tokens[1].type == TokenType::Number && tokens[1].text == "10",
          "parser should classify integer numbers");
  require(tokens[2].type == TokenType::Number && tokens[2].text == "-2.5",
          "parser should classify float numbers");
  require(tokens[3].type == TokenType::Path &&
              tokens[3].text == "./models/cube.obj",
          "parser should classify paths");
}

void change_color_accepts_rgb_triplet() {
  const std::vector<Token> tokens{number("255"), number("128"), number("0")};
  const auto request = Validator<Action::ChangeColor>::validate(tokens);

  require(request.has_value(), "change color should accept RGB triplets");
  require_near(request->r, 255.0f, "red channel should be parsed");
  require_near(request->g, 128.0f, "green channel should be parsed");
  require_near(request->b, 0.0f, "blue channel should be parsed");
}

void change_color_rejects_out_of_range_values() {
  const std::vector<Token> tokens{number("256"), number("128"), number("0")};
  const auto request = Validator<Action::ChangeColor>::validate(tokens);

  require(!request.has_value(), "change color should reject values above 255");
}

void move_object_accepts_negative_coordinates() {
  const std::vector<Token> tokens{number("-1"), number("2.5"), number("3")};
  const auto request = Validator<Action::MoveObject>::validate(tokens);

  require(request.has_value(), "move object should accept valid coordinates");
  require_near(request->x, -1.0f, "x coordinate should be parsed");
  require_near(request->y, 2.5f, "y coordinate should be parsed");
  require_near(request->z, 3.0f, "z coordinate should be parsed");
}


void check_object_displacement(float dx, float dy, float dz) {
  const std::string input = std::to_string(dx) + " " + std::to_string(dy) +
                            " " + std::to_string(dz);
  const auto request =
      Validator<Action::MoveObject>::validate(Parser{}.tokenize(input));
  require(request.has_value(), "displacement " + input + " should be accepted");
  require_near(request->x, dx, input + ": parsed x");
  require_near(request->y, dy, input + ": parsed y");
  require_near(request->z, dz, input + ": parsed z");

  const std::array<Vector3, 3> starts{
      Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, -20.0f, 30.0f),
      Vector3(-dx / 2.0f, -dy / 2.0f, -dz / 2.0f)};
  for (std::size_t index = 0; index < starts.size(); ++index) {
    World world;
    Command<Action::ImportObject>{}.execute(world, TriangulatedObject{});
    auto& position = world.selected_object()->transform().position;
    position = starts[index];
    const std::string context = input + ", start " + std::to_string(index);

    Command<Action::MoveObject>{}.execute(world, *request);

    require_near(position.x(), starts[index].x() + dx, context + ": final x");
    require_near(position.y(), starts[index].y() + dy, context + ": final y");
    require_near(position.z(), starts[index].z() + dz, context + ": final z");
  }
}

void move_object_moves_in_all_eight_octants() {
  for (float x_sign : {-1.0f, 1.0f}) {
    for (float y_sign : {-1.0f, 1.0f}) {
      for (float z_sign : {-1.0f, 1.0f}) {
        check_object_displacement(x_sign * 1.5f, y_sign * 2.5f, z_sign * 3.5f);
      }
    }
  }
}

void move_object_moves_along_each_axis_in_both_directions() {
  for (float sign : {-1.0f, 1.0f}) {
    check_object_displacement(sign * 1.5f, 0.0f, 0.0f);
    check_object_displacement(0.0f, sign * 2.5f, 0.0f);
    check_object_displacement(0.0f, 0.0f, sign * 3.5f);
  }
}

void move_object_moves_in_each_coordinate_plane() {
  for (float first_sign : {-1.0f, 1.0f}) {
    for (float second_sign : {-1.0f, 1.0f}) {
      check_object_displacement(first_sign * 1.5f, second_sign * 2.5f, 0.0f);
      check_object_displacement(first_sign * 1.5f, 0.0f, second_sign * 3.5f);
      check_object_displacement(0.0f, first_sign * 2.5f, second_sign * 3.5f);
    }
  }
}

void move_object_zero_displacement_preserves_position() {
  check_object_displacement(0.0f, 0.0f, 0.0f);
}

void move_object_applies_relative_displacement() {
  World world;
  Command<Action::ImportObject>{}.execute(world, TriangulatedObject{});

  Command<Action::MoveObject> move;
  move.execute(world, MoveObjectRequest{1.0f, 2.0f, 3.0f});
  move.execute(world, MoveObjectRequest{-0.5f, 1.0f, -2.0f});

  const auto& position = world.selected_object()->transform().position;
  require_near(position.x(), 0.5f, "x displacement should accumulate");
  require_near(position.y(), 3.0f, "y displacement should accumulate");
  require_near(position.z(), 1.0f, "z displacement should accumulate");
}

void rotate_object_rejects_values_above_360_degrees() {
  const std::vector<Token> tokens{number("0"), number("90"), number("361")};
  const auto request = Validator<Action::RotateObject>::validate(tokens);

  require(!request.has_value(), "rotate object should reject angles above 360");
}

void scale_object_rejects_zero_scale() {
  const std::vector<Token> tokens{number("1"), number("0"), number("1")};
  const auto request = Validator<Action::ScaleObject>::validate(tokens);

  require(!request.has_value(), "scale object should reject zero scale");
}

void check_rotated_basis(
    const std::string& input, const std::array<Vector3, 3>& expected,
    const Vector3& initial_rotation = Vector3::Zero()) {
  const auto request =
      Validator<Action::RotateObject>::validate(Parser{}.tokenize(input));
  require(request.has_value(), "rotation " + input + " should be accepted");

  World world;
  Command<Action::ImportObject>{}.execute(world, TriangulatedObject{});
  Command<Action::ImportObject>{}.execute(world, TriangulatedObject{});
  Command<Action::PreviousObject>{}.execute(world, {});
  auto* selected = world.selected_object();
  require(selected != nullptr, "an object should be selected for rotation");
  selected->transform().rotation = initial_rotation;
  selected->transform().position = Vector3(10.0f, -20.0f, 30.0f);
  std::vector<Renderer::Matrix4> before;
  for (const auto& object : world) {
    before.push_back(object.transform().matrix());
  }

  Command<Action::RotateObject>{}.execute(world, *request);
  const auto matrix = selected->transform().matrix();
  for (int axis = 0; axis < 3; ++axis) {
    // A direction has w = 0, so translation cannot hide an incorrect rotation.
    Renderer::Vector4 basis = Renderer::Vector4::Zero();
    basis[axis] = 1.0f;
    const Renderer::Vector4 actual = matrix * basis;
    for (int component = 0; component < 3; ++component) {
      require_near(actual[component], expected[axis][component],
                   "rotation " + input + ", basis " + std::to_string(axis) +
                       ", component " + std::to_string(component));
    }
  }
  require_near(selected->transform().position.x(), 10.0f, "rotation preserves x");
  require_near(selected->transform().position.y(), -20.0f, "rotation preserves y");
  require_near(selected->transform().position.z(), 30.0f, "rotation preserves z");
  std::size_t index = 0;
  for (const auto& object : world) {
    if (&object != selected) {
      require(object.transform().matrix().isApprox(before[index]),
              "rotation should not change unselected objects");
    }
    ++index;
  }
}

void rotate_object_positive_angles_around_each_axis() {
  check_rotated_basis("90 0 0", {Vector3(1, 0, 0), Vector3(0, 0, 1), Vector3(0, -1, 0)});
  check_rotated_basis("0 90 0", {Vector3(0, 0, -1), Vector3(0, 1, 0), Vector3(1, 0, 0)});
  check_rotated_basis("0 0 90", {Vector3(0, 1, 0), Vector3(-1, 0, 0), Vector3(0, 0, 1)});
  const float diagonal = std::sqrt(0.5f);
  check_rotated_basis("0 45 0", {Vector3(diagonal, 0, -diagonal),
                                Vector3(0, 1, 0), Vector3(diagonal, 0, diagonal)});
}

void rotate_object_rejects_negative_angle_around_x() {
  const auto request = Validator<Action::RotateObject>::validate(
      Parser{}.tokenize("-90 0 0"));
  require(!request.has_value(), "negative x angle should be rejected");
}

void rotate_object_rejects_negative_angle_around_y() {
  const auto request = Validator<Action::RotateObject>::validate(
      Parser{}.tokenize("0 -90 0"));
  require(!request.has_value(), "negative y angle should be rejected");
}

void rotate_object_rejects_negative_angle_around_z() {
  const auto request = Validator<Action::RotateObject>::validate(
      Parser{}.tokenize("0 0 -90"));
  require(!request.has_value(), "negative z angle should be rejected");
}

void rotate_object_around_multiple_axes() {
  // Expected basis directions for rotations applied in X, then Y, then Z order.
  check_rotated_basis("90 90 0", {Vector3(0, 0, -1), Vector3(1, 0, 0), Vector3(0, -1, 0)});
  check_rotated_basis("90 0 90", {Vector3(0, 1, 0), Vector3(0, 0, 1), Vector3(1, 0, 0)});
  check_rotated_basis("0 90 90", {Vector3(0, 0, -1), Vector3(-1, 0, 0), Vector3(0, 1, 0)});
  check_rotated_basis("90 90 90", {Vector3(0, 0, -1), Vector3(0, 1, 0), Vector3(1, 0, 0)});
}

void rotate_object_rejects_mixed_sign_angles() {
  for (const std::string input : {"-90 90 90", "90 -90 90", "90 90 -90"}) {
    const auto request =
        Validator<Action::RotateObject>::validate(Parser{}.tokenize(input));
    require(!request.has_value(), "mixed-sign rotation " + input + " should be rejected");
  }
}

void rotate_object_zero_preserves_initial_orientation() {
  check_rotated_basis("0 0 0", {Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1)});
}

void rotate_object_zero_preserves_existing_orientation() {
  check_rotated_basis("0 0 0", {Vector3(0, 0, -1), Vector3(0, 1, 0), Vector3(1, 0, 0)},
                     Vector3(0.0f, Renderer::to_radians(90.0f), 0.0f));
}

void rotate_object_accumulates_successive_angles() {
  World world;
  Command<Action::ImportObject>{}.execute(world, TriangulatedObject{});
  const std::array<std::string, 4> inputs{
      "10 20 30", "20 40 60", "0 0 0", "360 360 360"};
  const std::array<Vector3, 4> expected_degrees{
      Vector3(10, 20, 30), Vector3(30, 60, 90),
      Vector3(30, 60, 90), Vector3(390, 420, 450)};
  for (std::size_t step = 0; step < inputs.size(); ++step) {
    const auto request = Validator<Action::RotateObject>::validate(
        Parser{}.tokenize(inputs[step]));
    require(request.has_value(), "rotation increments should be accepted");
    Command<Action::RotateObject>{}.execute(world, *request);
    const auto& rotation = world.selected_object()->transform().rotation;
    for (int axis = 0; axis < 3; ++axis) {
      require_near(rotation[axis], Renderer::to_radians(expected_degrees[step][axis]),
                   "rotation should accumulate at step " + std::to_string(step) +
                       ", axis " + std::to_string(axis));
    }
  }
}

void rotate_object_adds_to_existing_orientation() {
  check_rotated_basis("0 45 0",
                     {Vector3(0, 0, -1), Vector3(0, 1, 0), Vector3(1, 0, 0)},
                     Vector3(0.0f, Renderer::to_radians(45.0f), 0.0f));
}

Vector3 transformed_box_dimensions(const Renderer::Transform& transform) {
  // Eight corners of a box with distinct dimensions: 2 x 4 x 6.
  // Use the same model matrix as rendering, without requiring a GPU context.
  const auto matrix = transform.matrix();
  Vector3 minimum = Vector3::Constant(std::numeric_limits<float>::max());
  Vector3 maximum = Vector3::Constant(std::numeric_limits<float>::lowest());
  for (float x : {-1.0f, 1.0f}) {
    for (float y : {-2.0f, 2.0f}) {
      for (float z : {-3.0f, 3.0f}) {
        const Renderer::Vector4 point = matrix * Renderer::Vector4(x, y, z, 1.0f);
        minimum = minimum.cwiseMin(point.head<3>());
        maximum = maximum.cwiseMax(point.head<3>());
      }
    }
  }
  return maximum - minimum;
}

void check_scaled_dimensions(const std::string& input,
                             const Vector3& expected_dimensions) {
  const auto request =
      Validator<Action::ScaleObject>::validate(Parser{}.tokenize(input));
  require(request.has_value(), "scale " + input + " should be accepted");

  for (const Vector3& position :
       {Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, -20.0f, 30.0f)}) {
    World world;
    Command<Action::ImportObject>{}.execute(world, TriangulatedObject{});
    auto& transform = world.selected_object()->transform();
    transform.position = position;
    const Vector3 before = transformed_box_dimensions(transform);
    require_near(before.x(), 2.0f, "initial width");
    require_near(before.y(), 4.0f, "initial height");
    require_near(before.z(), 6.0f, "initial depth");

    Command<Action::ScaleObject>{}.execute(world, *request);
    const Vector3 after = transformed_box_dimensions(transform);
    for (int axis = 0; axis < 3; ++axis) {
      const std::string context = "scale " + input + ", axis " + std::to_string(axis);
      require_near(after[axis], expected_dimensions[axis], context + ": dimension");
      require_near(transform.position[axis], position[axis], context + ": position");
      if (expected_dimensions[axis] > before[axis]) {
        require(after[axis] > before[axis], context + " should grow");
      } else if (expected_dimensions[axis] < before[axis]) {
        require(after[axis] < before[axis], context + " should shrink");
      } else {
        require_near(after[axis], before[axis], context + " should stay unchanged");
      }
    }
  }
}

void scale_object_increases_dimensions() {
  check_scaled_dimensions("2 2 2", Vector3(4.0f, 8.0f, 12.0f));
}

void scale_object_decreases_dimensions() {
  check_scaled_dimensions("0.5 0.5 0.5", Vector3(1.0f, 2.0f, 3.0f));
}

void scale_object_changes_each_axis_independently() {
  check_scaled_dimensions("2 1 1", Vector3(4.0f, 4.0f, 6.0f));
  check_scaled_dimensions("0.5 1 1", Vector3(1.0f, 4.0f, 6.0f));
  check_scaled_dimensions("1 2 1", Vector3(2.0f, 8.0f, 6.0f));
  check_scaled_dimensions("1 0.5 1", Vector3(2.0f, 2.0f, 6.0f));
  check_scaled_dimensions("1 1 2", Vector3(2.0f, 4.0f, 12.0f));
  check_scaled_dimensions("1 1 0.5", Vector3(2.0f, 4.0f, 3.0f));
}

void scale_object_changes_multiple_axes_nonuniformly() {
  check_scaled_dimensions("2 0.5 1", Vector3(4.0f, 2.0f, 6.0f));
  check_scaled_dimensions("0.5 1 3", Vector3(1.0f, 4.0f, 18.0f));
  check_scaled_dimensions("1 3 0.5", Vector3(2.0f, 12.0f, 3.0f));
  check_scaled_dimensions("2 0.5 3", Vector3(4.0f, 2.0f, 18.0f));
}

void scale_object_identity_preserves_dimensions() {
  check_scaled_dimensions("1 1 1", Vector3(2.0f, 4.0f, 6.0f));
}

void delete_object_accepts_only_yes() {
  const auto yes = Validator<Action::DeleteObject>::validate({word("Y")});
  const auto no = Validator<Action::DeleteObject>::validate({word("n")});

  require(yes.has_value(), "delete object should accept y");
  require(!no.has_value(), "delete object should reject non-y answers");
}

void dialogue_retries_until_input_is_valid() {
  std::istringstream input("wrong input\n10 20 30\n");
  std::ostringstream output;
  StreamBufferRedirect input_redirect(std::cin, input.rdbuf());
  StreamBufferRedirect output_redirect(std::cout, output.rdbuf());

  const Dialogue dialogue;
  const auto request = dialogue.get_user_request(
      MessageTraits<Action::ChangeColor>{}, Validator<Action::ChangeColor>{});

  require(request.has_value(),
          "dialogue should keep reading after invalid input");
  require_near(request->r, 10.0f, "red channel should use retried input");
  require_near(request->g, 20.0f, "green channel should use retried input");
  require_near(request->b, 30.0f, "blue channel should use retried input");
  require(output.str().find("Invalid RGB color") != std::string::npos,
          "dialogue should explain why input was rejected");
}

void import_object_requires_readable_obj_file() {
  const auto temp_dir = std::filesystem::temp_directory_path();
  const auto obj_path = temp_dir / "renderer_test_model.obj";
  const auto txt_path = temp_dir / "renderer_test_model.txt";

  {
    std::ofstream file(obj_path);
    file << "o test\n";
  }
  {
    std::ofstream file(txt_path);
    file << "not an obj\n";
  }

  const auto valid =
      Validator<Action::ImportObject>::validate({path(obj_path.string())});
  const auto invalid_extension =
      Validator<Action::ImportObject>::validate({path(txt_path.string())});
  const auto missing = Validator<Action::ImportObject>::validate(
      {path((temp_dir / "renderer_missing_model.obj").string())});

  std::filesystem::remove(obj_path);
  std::filesystem::remove(txt_path);

  require(valid.has_value(), "import object should accept readable .obj files");
  require(!invalid_extension.has_value(),
          "import object should reject non-obj files");
  require(!missing.has_value(), "import object should reject missing files");
}

}  // namespace

int main() {
  const std::vector<std::pair<std::string, void (*)()>> tests{
      {"change_color_checks_range_boundaries", change_color_checks_range_boundaries},
      {"move_object_checks_range_boundaries", move_object_checks_range_boundaries},
      {"rotate_object_checks_range_boundaries", rotate_object_checks_range_boundaries},
      {"scale_object_checks_range_boundaries", scale_object_checks_range_boundaries},
      {"parser_classifies_words_numbers_and_paths",
       parser_classifies_words_numbers_and_paths},
      {"change_color_accepts_rgb_triplet", change_color_accepts_rgb_triplet},
      {"change_color_rejects_out_of_range_values",
       change_color_rejects_out_of_range_values},
      {"move_object_accepts_negative_coordinates",
       move_object_accepts_negative_coordinates},
      {"move_object_applies_relative_displacement",
       move_object_applies_relative_displacement},
      {"move_object_moves_in_all_eight_octants",
       move_object_moves_in_all_eight_octants},
      {"move_object_moves_along_each_axis_in_both_directions",
       move_object_moves_along_each_axis_in_both_directions},
      {"move_object_moves_in_each_coordinate_plane",
       move_object_moves_in_each_coordinate_plane},
      {"move_object_zero_displacement_preserves_position",
       move_object_zero_displacement_preserves_position},
      {"rotate_object_rejects_values_above_360_degrees",
       rotate_object_rejects_values_above_360_degrees},
      {"rotate_object_positive_angles_around_each_axis",
       rotate_object_positive_angles_around_each_axis},
      {"rotate_object_rejects_negative_angle_around_x", rotate_object_rejects_negative_angle_around_x},
      {"rotate_object_rejects_negative_angle_around_y", rotate_object_rejects_negative_angle_around_y},
      {"rotate_object_rejects_negative_angle_around_z", rotate_object_rejects_negative_angle_around_z},
      {"rotate_object_around_multiple_axes", rotate_object_around_multiple_axes},
      {"rotate_object_rejects_mixed_sign_angles", rotate_object_rejects_mixed_sign_angles},
      {"rotate_object_zero_preserves_initial_orientation",
       rotate_object_zero_preserves_initial_orientation},
      {"rotate_object_zero_preserves_existing_orientation",
       rotate_object_zero_preserves_existing_orientation},
      {"rotate_object_accumulates_successive_angles",
       rotate_object_accumulates_successive_angles},
      {"rotate_object_adds_to_existing_orientation",
       rotate_object_adds_to_existing_orientation},
      {"scale_object_rejects_zero_scale", scale_object_rejects_zero_scale},
      {"scale_object_increases_dimensions", scale_object_increases_dimensions},
      {"scale_object_decreases_dimensions", scale_object_decreases_dimensions},
      {"scale_object_changes_each_axis_independently",
       scale_object_changes_each_axis_independently},
      {"scale_object_changes_multiple_axes_nonuniformly",
       scale_object_changes_multiple_axes_nonuniformly},
      {"scale_object_identity_preserves_dimensions",
       scale_object_identity_preserves_dimensions},
      {"delete_object_accepts_only_yes", delete_object_accepts_only_yes},
      {"dialogue_retries_until_input_is_valid",
       dialogue_retries_until_input_is_valid},
      {"import_object_requires_readable_obj_file",
       import_object_requires_readable_obj_file},
  };

  int failed = 0;
  for (const auto& test : tests) {
    try {
      test.second();
      std::cout << "[PASS] " << test.first << '\n';
    } catch (const std::exception& error) {
      ++failed;
      std::cerr << "[FAIL] " << test.first << ": " << error.what() << '\n';
    }
  }

  if (failed != 0) {
    std::cerr << failed << " test(s) failed\n";
    return 1;
  }

  std::cout << tests.size() << " test(s) passed\n";
  return 0;
}
