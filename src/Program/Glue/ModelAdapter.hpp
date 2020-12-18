#pragma once
#include <iostream>
#include <utility>

#include <QVector3D>
#include <qobjectdefs.h>

#include "Model/Model.hpp"
#include "UI/IModel.hpp"

class ModelEntityAdapter : public ui::IModelEntity
{
  Q_OBJECT;

public:
  void moveTo(const QVector3D& newPosition) override;
  void rotate() override;

  QVector3D position() const override;
  float yRotation() const override;

  // Ctor
public:
  ModelEntityAdapter(QVector3D position, float yRotation) : position_(std::move(position)), yRotation_(yRotation) {}

private:
  QVector3D position_;
  float yRotation_{};
};

class ModelAdapter : public ui::IModel
{
public:
  ModelAdapter(model::Model model) : model_(std::move(model)) {}

  std::shared_ptr<ui::IModelEntity> get() const override
  {
    return std::make_shared<ModelEntityAdapter>(QVector3D(0.0f, 0.36f, 0.0f), 0.0f);
  }

private:
  model::Model model_;
};
