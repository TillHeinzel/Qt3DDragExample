#pragma once

#include <QEntity>

#include "IModel.hpp"

class MoveDragger : public Qt3DCore::QEntity
{
public:
  MoveDragger(Qt3DCore::QEntity* parent, std::shared_ptr<ui::IModelEntity> model);

private:
  struct DragProcess;
  std::unique_ptr<DragProcess> drag_;

  Qt3DCore::QComponent* makeMouseInteraction(std::shared_ptr < ui::IModelEntity> model);
};
