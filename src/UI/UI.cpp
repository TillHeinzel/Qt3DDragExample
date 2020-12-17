#include "UI.hpp"

#include <QGuiApplication>
#include <QtWidgets/QApplication>

#include "detail/CubeManager.hpp"
#include "detail/SceneWidget.hpp"

int runUI(int argc, char** argv)
{
  QApplication app(argc, argv);

  auto* sceneWidget = new SceneWidget();

  auto cubeManager = CubeManager(sceneWidget->rootEntity());

  // Show window
  sceneWidget->show();
  sceneWidget->resize(1200, 800);

  return app.exec();
}
