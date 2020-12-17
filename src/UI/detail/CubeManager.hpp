
#pragma once

#include <Qt3DCore/qentity.h>

class CubeManager
{
public:
  explicit CubeManager(Qt3DCore::QEntity* rootEntity);

private:
  Qt3DCore::QEntity* rootEntity_;
  Qt3DCore::QEntity* cube_;
  Qt3DCore::QEntity* dragger_;
};
