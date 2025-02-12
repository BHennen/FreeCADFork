/***************************************************************************
 *   Copyright (c) 2013 Werner Mayer <wmayer[at]users.sourceforge.net>     *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/

#include "PreCompiled.h"
#ifndef _PreComp_
# include <QDialog>
#endif

#include <Base/Tools.h>
#include <Gui/BitmapFactory.h>
#include <Gui/MainWindow.h>

#include "ImageOrientationDialog.h"
#include "ui_ImageOrientationDialog.h"


using namespace ImageGui;

ImageOrientationDialog::ImageOrientationDialog()
  : QDialog(Gui::getMainWindow()), ui(new Ui_ImageOrientationDialog)
{
    DirType = 0;
    ui->setupUi(this);
    onPreview();

    connect(ui->Reverse_checkBox, &QCheckBox::clicked,    this, &ImageOrientationDialog::onPreview);
    connect(ui->XY_radioButton  , &QRadioButton::clicked, this, &ImageOrientationDialog::onPreview);
    connect(ui->XZ_radioButton  , &QRadioButton::clicked, this, &ImageOrientationDialog::onPreview);
    connect(ui->YZ_radioButton  , &QRadioButton::clicked, this, &ImageOrientationDialog::onPreview);
}

ImageOrientationDialog::~ImageOrientationDialog()
{
    delete ui;
}

void ImageOrientationDialog::accept()
{
    double offset = ui->Offset_doubleSpinBox->value().getValue();
    bool reverse = ui->Reverse_checkBox->isChecked();
    if (ui->XY_radioButton->isChecked()) {
        if (reverse) {
            Pos = Base::Placement(Base::Vector3d(0,0,offset),Base::Rotation(-1.0,0.0,0.0,0.0));
            DirType = 1;
        }
        else {
            Pos = Base::Placement(Base::Vector3d(0,0,offset),Base::Rotation());
            DirType = 0;
        }
    }
    else if (ui->XZ_radioButton->isChecked()) {
        if (reverse) {
            Pos = Base::Placement(Base::Vector3d(0,offset,0),Base::Rotation(Base::Vector3d(0,sqrt(2.0)/2.0,sqrt(2.0)/2.0),M_PI));
            DirType = 3;
        }
        else {
            Pos = Base::Placement(Base::Vector3d(0,offset,0),Base::Rotation(Base::Vector3d(-1,0,0),1.5*M_PI));
            DirType = 2;
        }
    }
    else if (ui->YZ_radioButton->isChecked()) {
        if (reverse) {
            Pos = Base::Placement(Base::Vector3d(offset,0,0),Base::Rotation(-0.5,0.5,0.5,-0.5));
            DirType = 5;
        }
        else {
            Pos = Base::Placement(Base::Vector3d(offset,0,0),Base::Rotation(0.5,0.5,0.5,0.5));
            DirType = 4;
        }
    }

    QDialog::accept();
}

void ImageOrientationDialog::onPreview()
{
    std::string icon;
    bool reverse = ui->Reverse_checkBox->isChecked();
    if (ui->XY_radioButton->isChecked()) {
        if (reverse)
            icon = "view-bottom";
        else
            icon = "view-top";
    }
    else if (ui->XZ_radioButton->isChecked()) {
        if (reverse)
            icon = "view-rear";
        else
            icon = "view-front";
    }
    else if (ui->YZ_radioButton->isChecked()) {
        if (reverse)
            icon = "view-left";
        else
            icon = "view-right";
    }

    ui->previewLabel->setPixmap(
        Gui::BitmapFactory().pixmapFromSvg(icon.c_str(),
        ui->previewLabel->size()));
}

#include "moc_ImageOrientationDialog.cpp"
