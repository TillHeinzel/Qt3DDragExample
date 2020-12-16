#pragma once

#include <QEntity>
#include <QWidget>

class SceneWidget : public QWidget
{
public:
  SceneWidget();

  Qt3DCore::QEntity* rootEntity()
  {
    return rootEntity_;
  }

private:
  Qt3DCore::QEntity* rootEntity_;
};
