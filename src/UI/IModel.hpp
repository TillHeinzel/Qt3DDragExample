#pragma once

#include <QObject>

#include <QVector3D>

namespace ui
{
  class IModelEntity : public QObject
  {
    Q_OBJECT;

  public:
    virtual void moveTo(const QVector3D& newPosition) = 0;
    virtual void rotate() = 0;

    virtual QVector3D position() const = 0;
    virtual float yRotation() const = 0;

  signals:
    void dataChanged();

    // boilerplate
  public:
    virtual ~IModelEntity() = default;
  };

  class IModel
  {
  public:
    virtual std::shared_ptr<IModelEntity> get() const = 0;

    // boilerplate
  public:
    virtual ~IModel() = default;
  };
} // namespace ui
