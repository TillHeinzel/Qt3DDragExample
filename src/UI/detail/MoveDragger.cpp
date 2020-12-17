#include "MoveDragger.hpp"

#include <iostream>

#include <Qt3DCore/qtransform.h>

#include <Qt3DRender/QObjectPicker>
#include <Qt3DRender/QPickEvent>

#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QSphereMesh>

#include "UI/IModel.hpp"

namespace
{
  auto makeMesh()
  {
    Qt3DExtras::QSphereMesh* mesh = new Qt3DExtras::QSphereMesh();
    mesh->setRings(20);
    mesh->setSlices(20);
    mesh->setRadius(2);
    return mesh;
  }

  auto makeMaterial()
  {
    Qt3DExtras::QPhongMaterial* material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(QColor(QRgb(0xa69929)));
    return material;
  }

  constexpr float relativePosition = -1.0f;

  auto makeTransform()
  {
    auto* transform = new Qt3DCore::QTransform();
    transform->setScale(0.1f);
    transform->setTranslation(QVector3D(relativePosition, 0.0f, 0.0f));
    return transform;
  }

} // namespace

struct MoveDragger::DragProcess
{
  DragProcess(std::shared_ptr<ui::IModelEntity> model, float startPosition) :
    model_(model),
    previousPosition_(startPosition)
  {
  }

  void moveTo(float newPosition)
  {
    model_->translate(XDelta{newPosition - previousPosition_});
    previousPosition_ = newPosition;
  }

private:
  std::shared_ptr<ui::IModelEntity> model_;
  float previousPosition_;
};

MoveDragger::MoveDragger(Qt3DCore::QEntity* parent, std::shared_ptr<ui::IModelEntity> model) : QEntity(parent)
{
  // Sphere
  addComponent(makeMesh());
  addComponent(makeMaterial());
  addComponent(makeTransform());
  addComponent(makeMouseInteraction(model));
}

Qt3DCore::QComponent* MoveDragger::makeMouseInteraction(std::shared_ptr<ui::IModelEntity> model)
{
  auto* picker = new Qt3DRender::QObjectPicker();
  picker->setEnabled(true);
  picker->setDragEnabled(true);

  QObject::connect(picker, &Qt3DRender::QObjectPicker::pressed, [this, model](Qt3DRender::QPickEvent* event) {
    drag_ = std::make_unique<DragProcess>(model, event->worldIntersection().x());
  });

  QObject::connect(picker, &Qt3DRender::QObjectPicker::moved, [this](Qt3DRender::QPickEvent* event) {
    drag_->moveTo(event->worldIntersection().x());
  });

  QObject::connect(picker, &Qt3DRender::QObjectPicker::released, [this](Qt3DRender::QPickEvent*) {
    drag_ = nullptr;
  });

  return picker;
}
