#include "initializeContent.hpp"

#include <Qt3DCore/QTransform>

#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPhongMaterial>

#include "detail/MovableEntity.hpp"

namespace
{
  auto makeMesh()
  {
    return new Qt3DExtras::QCuboidMesh();
  }

  auto makeTransform(const QVector3D& position)
  {
    Qt3DCore::QTransform* transform = new Qt3DCore::QTransform();
    transform->setScale(4.0f);
    transform->setTranslation(position);
    return transform;
  }

  auto makeMaterial()
  {
    Qt3DExtras::QPhongMaterial* material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(QColor(QRgb(0x665423)));
    return material;
  }

  void addCubeTo(Qt3DCore::QEntity* rootEntity, std::shared_ptr<ui::IModelEntity> model)
  {
    auto entity = new MovableEntity(rootEntity, makeTransform(model->position().val), model);

    entity->addComponent(makeMesh());
    entity->addComponent(makeMaterial());
  }
} // namespace

void initializeContent(Qt3DCore::QEntity* rootEntity, std::shared_ptr<ui::IModel> model)
{
  addCubeTo(rootEntity, model->get());
}
