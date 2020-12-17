#pragma once

#include <QObject>

#include <QVector3D>

struct Position
{
  QVector3D val;
};

struct XDelta
{
  float val;
};

namespace ui
{
  class IModelEntity : public QObject
  {
    Q_OBJECT;

  public:
    virtual void translate(XDelta dx) = 0;
    virtual Position position() const = 0;

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
