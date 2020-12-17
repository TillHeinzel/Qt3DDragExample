#pragma once

#include <memory>

#include "UI/IModel.hpp"

namespace ui
{
  int runUI(int argc, char** argv, std::shared_ptr<IModel> model);
}