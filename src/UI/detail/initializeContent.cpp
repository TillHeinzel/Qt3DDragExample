#include "initializeContent.hpp"

#include <Qt3DCore/QTransform>

#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPhongMaterial>

#include <Qt3DInput/QMouseHandler>

#include <Qt3DRender/QObjectPicker>
#include <Qt3DRender/QPickEvent>

#include <QMouseDevice>

namespace
{
  auto makeMesh()
  {
    auto* mesh = new Qt3DExtras::QCuboidMesh();
    return mesh;
  }

  auto makeTransform()
  {
    Qt3DCore::QTransform* transform = new Qt3DCore::QTransform();
    transform->setScale3D({2.0f, 0.72f, 1.0f});
    return transform;
  }

  auto makeMaterial()
  {
    Qt3DExtras::QPhongMaterial* material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(QColor(QRgb(0xFFF03A)));
    return material;
  }

  auto makeTranslationInteractionComponent(std::shared_ptr<ui::IModelEntity> model)
  {
    auto* picker = new Qt3DRender::QObjectPicker();
    picker->setEnabled(true);
    picker->setDragEnabled(true);

    QObject::connect(picker, &Qt3DRender::QObjectPicker::moved, [model](Qt3DRender::QPickEvent* event) {
      model->moveTo({event->worldIntersection().x(), model->position().y(), event->worldIntersection().z()});
    });

    QObject::connect(picker, &Qt3DRender::QObjectPicker::pressed, [model](Qt3DRender::QPickEvent* event) {
      qDebug() << "pressed at: " << event->worldIntersection() << "; local: " << event->localIntersection();
    });

    return picker;
  }

  auto makeRotationInteractionComponent(Qt3DInput::QMouseDevice* mouseDevice, std::shared_ptr<ui::IModelEntity> model)
  {
    auto mouseHandler = new Qt3DInput::QMouseHandler();
    mouseHandler->setSourceDevice(mouseDevice);

    QObject::connect(mouseHandler, &Qt3DInput::QMouseHandler::wheel, [model](Qt3DInput::QWheelEvent* /*event*/) {
      model->rotate();
    });

    return mouseHandler;
  }

  void loadTransformFromModel(Qt3DCore::QTransform* transform, std::shared_ptr<ui::IModelEntity> model)
  {
    transform->setTranslation(model->position());
    transform->setRotationY(model->yRotation());
  }

  void updateOnModelChange(std::shared_ptr<ui::IModelEntity> model, Qt3DCore::QTransform* transform)
  {
    QObject::connect(model.get(), &ui::IModelEntity::dataChanged, transform, [model, transform]() {
      loadTransformFromModel(transform, model);
    });
  }

  void addBrickTo(Qt3DCore::QEntity* rootEntity,
                 std::shared_ptr<ui::IModelEntity> model,
                 Qt3DInput::QMouseDevice* mouseDevice)
  {
    auto entity = new Qt3DCore::QEntity(rootEntity);

    auto transform = makeTransform();
    entity->addComponent(transform);
    loadTransformFromModel(transform, model);
    updateOnModelChange(model, transform);

    entity->addComponent(makeTranslationInteractionComponent(model));
    entity->addComponent(makeRotationInteractionComponent(mouseDevice, model));

    entity->addComponent(makeMesh());
    entity->addComponent(makeMaterial());
  }
} // namespace

void initializeContent(Qt3DCore::QEntity* rootEntity, std::shared_ptr<ui::IModel> model)
{
  auto* mouseDevice = new Qt3DInput::QMouseDevice(rootEntity);

  addBrickTo(rootEntity, model->get(), mouseDevice);
}
