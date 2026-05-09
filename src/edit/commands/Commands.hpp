#pragma once

#include "ModelContext.hpp"
#include "Requests.hpp"

namespace Renderer {

using ImportObjectRequest =
    typename RequestTraits<ConsoleAction::ImportObject>::Request;
using ChangeColorRequest =
    typename RequestTraits<ConsoleAction::ChangeColor>::Request;
using MoveObjectRequest =
    typename RequestTraits<ConsoleAction::MoveObject>::Request;
using RotateObjectRequest =
    typename RequestTraits<ConsoleAction::RotateObject>::Request;
using ScaleObjectRequest =
    typename RequestTraits<ConsoleAction::ScaleObject>::Request;
using DeleteObjectRequest =
    typename RequestTraits<ConsoleAction::DeleteObject>::Request;

struct ImportObjectCommand {
  void execute(ModelContext& context, const ImportObjectRequest& request);
};

struct ChangeColorCommand {
  void execute(ModelContext& context, const ChangeColorRequest& request);
};

struct MoveObjectCommand {
  void execute(ModelContext& context, const MoveObjectRequest& request);
};

struct RotateObjectCommand {
  void execute(ModelContext& context, const RotateObjectRequest& request);
};

struct ScaleObjectCommand {
  void execute(ModelContext& context, const ScaleObjectRequest& request);
};

struct DeleteObjectCommand {
  void execute(ModelContext& context, const DeleteObjectRequest& request);
};

}  // namespace Renderer
