
#pragma once

#include <Qt3DCore/qentity.h>

#include "IModel.hpp"

void initializeContent(Qt3DCore::QEntity* rootEntity, std::shared_ptr<ui::IModel> model);
