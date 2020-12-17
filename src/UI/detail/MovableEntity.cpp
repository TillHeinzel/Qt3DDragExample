#include "MovableEntity.hpp"

#include <Qt3DRender/QObjectPicker>
#include <Qt3DRender/QPickEvent>

#include "detail/MoveDragger.hpp"

namespace
{
  auto addMoveDragger(Qt3DCore::QEntity* parent, std::shared_ptr<ui::IModelEntity> model)
  {
    auto moveDragger = new MoveDragger(parent, model);
    moveDragger->setEnabled(true);
  }
} // namespace

MovableEntity::MovableEntity(Qt3DCore::QEntity* rootEntity,
                             Qt3DCore::QTransform* transform,
                             std::shared_ptr<ui::IModelEntity> model) :
  QEntity(rootEntity)
{
  addMoveDragger(this, model);
  addComponent(transform);

  QObject::connect(model.get(), &ui::IModelEntity::dataChanged, this, [model, transform]() {
    qDebug() << "new position: " << model->position().val;
    transform->setTranslation(model->position().val);
  });
}
