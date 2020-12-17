/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "CubeManager.hpp"

#include <iostream>

#include <Qt3DCore/qtransform.h>

#include <Qt3DRender/QObjectPicker>
#include <Qt3DRender/QPickEvent>

#include <QDebug>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QSphereMesh>

CubeManager::CubeManager(Qt3DCore::QEntity* rootEntity) : rootEntity_(rootEntity)
{
  // Cuboid shape data
  Qt3DExtras::QCuboidMesh* cuboid = new Qt3DExtras::QCuboidMesh();

  // CuboidMesh Transform
  Qt3DCore::QTransform* cuboidTransform = new Qt3DCore::QTransform();
  cuboidTransform->setScale(4.0f);
  cuboidTransform->setTranslation(QVector3D(5.0f, -4.0f, 0.0f));

  Qt3DExtras::QPhongMaterial* cuboidMaterial = new Qt3DExtras::QPhongMaterial();
  cuboidMaterial->setDiffuse(QColor(QRgb(0x665423)));

  // Cuboid
  cube_ = new Qt3DCore::QEntity(rootEntity_);
  cube_->addComponent(cuboid);
  cube_->addComponent(cuboidMaterial);
  cube_->addComponent(cuboidTransform);

  // Sphere shape data
  Qt3DExtras::QSphereMesh* sphereMesh = new Qt3DExtras::QSphereMesh();
  sphereMesh->setRings(20);
  sphereMesh->setSlices(20);
  sphereMesh->setRadius(2);

  // Sphere mesh transform
  Qt3DCore::QTransform* sphereTransform = new Qt3DCore::QTransform();

  sphereTransform->setScale(1.3f);
  sphereTransform->setTranslation(QVector3D(-5.0f, -4.0f, 0.0f));

  Qt3DExtras::QPhongMaterial* sphereMaterial = new Qt3DExtras::QPhongMaterial();
  sphereMaterial->setDiffuse(QColor(QRgb(0xa69929)));

  // Sphere
  dragger_ = new Qt3DCore::QEntity(rootEntity_);
  dragger_->addComponent(sphereMesh);
  dragger_->addComponent(sphereMaterial);
  dragger_->addComponent(sphereTransform);

  auto* picker = new Qt3DRender::QObjectPicker();
  picker->setEnabled(true);
  picker->setDragEnabled(true);
  dragger_->addComponent(picker);

  QObject::connect(picker, &Qt3DRender::QObjectPicker::clicked, []() {
    std::cout << "clicked" << std::endl;
  });
  QObject::connect(picker, &Qt3DRender::QObjectPicker::moved, []() {
    std::cout << "moved" << std::endl;
  });
}