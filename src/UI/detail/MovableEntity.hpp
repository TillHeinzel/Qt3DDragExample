#pragma once
#include <QEntity>

#include <Qt3DCore/QTransform>

#include "IModel.hpp"

class MoveDragger;

class MovableEntity : public Qt3DCore::QEntity
{
public:
  MovableEntity(Qt3DCore::QEntity* rootEntity,
                Qt3DCore::QTransform* transform,
                std::shared_ptr<ui::IModelEntity> model);
};
