#include "UI.hpp"

#include <QGuiApplication>
#include <QtWidgets/QApplication>

#include "detail/SceneWidget.hpp"
#include "detail/initializeContent.hpp"

namespace ui
{
  int runUI(int argc, char** argv, std::shared_ptr<IModel> model)
  {
    QApplication app(argc, argv);

    auto* sceneWidget = new SceneWidget();

    initializeContent(sceneWidget->rootEntity(), model);

    // Show window
    sceneWidget->show();
    sceneWidget->resize(1200, 800);

    return app.exec();
  }
} // namespace ui
