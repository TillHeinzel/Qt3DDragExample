#pragma once
#include <iostream>
#include <utility>

#include "Model/Model.hpp"
#include "UI/IModel.hpp"

class ModelEntityAdapter : public ui::IModelEntity
{
  Q_OBJECT;

public:
  void translate(XDelta dx) override
  {
    std::cout << "moved: " << dx.val << std::endl;
    position_.setX(position_.x() + dx.val);
    emit dataChanged();
  }

  Position position() const override
  {
    return {position_};
  }

  ModelEntityAdapter(QVector3D position) : position_(position) {}

private:
  QVector3D position_;
};

class ModelAdapter : public ui::IModel
{
public:
  ModelAdapter(model::Model model) : model_(std::move(model)) {}

  std::shared_ptr<ui::IModelEntity> get() const override
  {
    return std::make_shared<ModelEntityAdapter>(QVector3D(0.0f, -4.0f, 0.0f));
  }

private:
  model::Model model_;
};
