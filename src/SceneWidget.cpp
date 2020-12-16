#include "SceneWidget.hpp"

#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcamera.h>
#include <Qt3DRender/qcameralens.h>
#include <qfirstpersoncameracontroller.h>

#include <QtGui/QScreen>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>

#include <Qt3DRender/qpointlight.h>
#include <Qt3DRender/qtechnique.h>
#include <Qt3DRender/qtexture.h>

#include <Qt3DCore/qtransform.h>

#include <Qt3DExtras/qforwardrenderer.h>

#include <Qt3DExtras/qt3dwindow.h>

namespace
{
  auto make3DView()
  {
    auto* view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
    return view;
  }

  auto containerize(Qt3DExtras::Qt3DWindow* view)
  {
    QWidget* container = QWidget::createWindowContainer(view);
    QSize screenSize = view->screen()->size();
    container->setMinimumSize(QSize(200, 100));
    container->setMaximumSize(screenSize);

    return container;
  }

  auto setup3DView()
  {
    auto* view = make3DView();

    auto* container = containerize(view);

    return std::make_tuple(view, container);
  }

  void addPointLight(Qt3DCore::QEntity* rootEntity, QVector3D position)
  {
    Qt3DCore::QEntity* lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight* light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform* lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(position);
    lightEntity->addComponent(lightTransform);
  }

  auto makeCamera(Qt3DExtras::Qt3DWindow* view)
  {
    Qt3DRender::QCamera* cameraEntity = view->camera();

    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(0, 0, 20.0f));
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));

    return cameraEntity;
  }

  void addCamera(Qt3DCore::QEntity* rootEntity, Qt3DRender::QCamera* camera)
  {
    Qt3DExtras::QFirstPersonCameraController* camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
    camController->setCamera(camera);
  }
} // namespace

SceneWidget::SceneWidget() : rootEntity_(new Qt3DCore::QEntity())
{
  setWindowTitle(QStringLiteral("Move the Cube"));

  auto* view = make3DView();

  // Camera
  auto* camera = makeCamera(view);
  addCamera(rootEntity_, camera);
  addPointLight(rootEntity_, camera->position());

  // Set root object of the scene
  view->setRootEntity(rootEntity_);

  // setup layout
  QHBoxLayout* hLayout = new QHBoxLayout(this);
  hLayout->addWidget(containerize(view), 1);
}
