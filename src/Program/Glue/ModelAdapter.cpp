#include "ModelAdapter.hpp"

namespace
{
  float roundToNearestMultipleOf(float value, float base)
  {
    return std::round(value / base) * base;
  }

  QVector3D snapToGrid(QVector3D newPosition)
  {
    constexpr static auto gridSpacing = 0.5f;

    newPosition.setX(roundToNearestMultipleOf(newPosition.x(), gridSpacing));
    newPosition.setZ(roundToNearestMultipleOf(newPosition.z(), gridSpacing));

    return newPosition;
  }

  QVector3D constrain(QVector3D newPosition)
  {
    constexpr static auto maxPos = 3.0f;

    newPosition.setX(std::clamp(newPosition.x(), -maxPos, maxPos));
    newPosition.setZ(std::clamp(newPosition.z(), -maxPos, maxPos));

    return newPosition;
  }
} // namespace

void ModelEntityAdapter::moveTo(const QVector3D& newPosition)
{
  position_ = constrain(snapToGrid(newPosition));
  emit dataChanged();
}

void ModelEntityAdapter::rotate()
{
  yRotation_ += 90;
  if(yRotation_ > 360.f) yRotation_ -= 360.f;
  emit dataChanged();
}

QVector3D ModelEntityAdapter::position() const
{
  return position_;
}

float ModelEntityAdapter::yRotation() const
{
  return yRotation_;
}

#include <doctest/doctest.hpp>

TEST_CASE("snapToGrid")
{
  REQUIRE(snapToGrid({0.0f, 0.0f, 0.0f}) == QVector3D{0.0f, 0.0f, 0.0f});
  REQUIRE(snapToGrid({0.3f, 0.0f, 0.0f}) == QVector3D{0.5f, 0.0f, 0.0f});
  REQUIRE(snapToGrid({0.6f, 0.0f, 0.0f}) == QVector3D{0.5f, 0.0f, 0.0f});
  REQUIRE(snapToGrid({0.25f, 0.0f, 0.0f}) == QVector3D{0.5f, 0.0f, 0.0f});
  REQUIRE(snapToGrid({0.0f, 0.0f, 0.3f}) == QVector3D{0.0f, 0.0f, 0.5f});
  REQUIRE(snapToGrid({0.0f, 0.0f, 0.6f}) == QVector3D{0.0f, 0.0f, 0.5f});
}

TEST_CASE("constrain")
{
  REQUIRE(constrain({0.0f, 0.0f, 0.0f}) == QVector3D{0.0f, 0.0f, 0.0f});
  REQUIRE(constrain({5.0f, 0.0f, 0.0f}) == QVector3D{3.0f, 0.0f, 0.0f});
  REQUIRE(constrain({0.0f, 0.0f, 5.0f}) == QVector3D{0.0f, 0.0f, 3.0f});
  REQUIRE(constrain({-5.0f, 0.0f, 0.0f}) == QVector3D{-3.0f, 0.0f, 0.0f});
  REQUIRE(constrain({0.0f, 0.0f, -5.0f}) == QVector3D{0.0f, 0.0f, -3.0f});
  REQUIRE(constrain({5.0f, 0.0f, -5.0f}) == QVector3D{3.0f, 0.0f, -3.0f});
}

TEST_CASE("Rotate")
{
  auto entity = ModelEntityAdapter({0.0f, 0.0f, 0.0f}, 0.0f);
  REQUIRE(entity.yRotation() == 0.0f);
  REQUIRE((entity.rotate(), entity.yRotation()) == 90.0f);
  REQUIRE((entity.rotate(), entity.yRotation()) == 180.0f);
  REQUIRE((entity.rotate(), entity.yRotation()) == 270.0f);
}