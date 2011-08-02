// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::FixedLightWidget */

#include "clients_gui_pch.h"
#include "FixedLightWidget.h"
#include "moc_FixedLightWidget.cpp"
#include "DirectionWidget.h"
#include "ColorButton.h"
#include <graphics/FixedLighting.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: FixedLightWidget constructor
// Created: AGE 2007-01-03
// -----------------------------------------------------------------------------
FixedLightWidget::FixedLightWidget( QWidget* parent, FixedLighting& lighting )
    : Q3HBox( parent )
    , lighting_( lighting )
{
    lighting_.SetLightDirection( geometry::Vector3f( 0, 0, 1 ) );
    lighting_.SetAmbient( 24/255.f, 24/255.f, 24/255.f );
    lighting_.SetDiffuse( 224/255.f, 224/255.f, 224/255.f );
    {
        Q3GroupBox* directionGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Light direction" ), this );
        DirectionWidget* direction = new DirectionWidget( directionGroup );
        connect( direction, SIGNAL( DirectionChanged( const geometry::Vector3f& ) ), this, SLOT( OnDirectionChanged( const geometry::Vector3f& ) ) );
    }
    Q3GroupBox* colorBox = new Q3GroupBox( 2, Qt::Horizontal, tr( "Light colors" ), this );
    {
        Q3VBox* labelBox = new Q3VBox( colorBox );
        new QLabel( tr( "Ambient" ), labelBox );
        new QLabel( tr( "Diffuse" ), labelBox );
    }
    {
        Q3VBox* buttonBox = new Q3VBox( colorBox );
        ambient_ = new ColorButton( buttonBox, "", QColor( 24, 24, 24 ) );
        connect( ambient_, SIGNAL( ColorChanged( const QColor& ) ), this, SLOT( OnAmbientChanged( const QColor& ) ) );
        diffuse_ = new ColorButton( buttonBox, "", QColor( 224, 224, 224 ) );
        connect( diffuse_, SIGNAL( ColorChanged( const QColor& ) ), this, SLOT( OnDiffuseChanged( const QColor& ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: FixedLightWidget destructor
// Created: AGE 2007-01-03
// -----------------------------------------------------------------------------
FixedLightWidget::~FixedLightWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FixedLightWidget::OnDirectionChanged
// Created: AGE 2007-01-03
// -----------------------------------------------------------------------------
void FixedLightWidget::OnDirectionChanged( const geometry::Vector3f& direction )
{
    lighting_.SetLightDirection( direction );
}

// -----------------------------------------------------------------------------
// Name: FixedLightWidget::OnAmbientChanged
// Created: AGE 2007-01-03
// -----------------------------------------------------------------------------
void FixedLightWidget::OnAmbientChanged( const QColor& color )
{
    ambient_->Commit();
    lighting_.SetAmbient( color.red() / 255.f, color.green() / 255.f, color.blue() / 255.f );
}

// -----------------------------------------------------------------------------
// Name: FixedLightWidget::OnDiffuseChanged
// Created: AGE 2007-01-03
// -----------------------------------------------------------------------------
void FixedLightWidget::OnDiffuseChanged( const QColor& color )
{
    diffuse_->Commit();
    lighting_.SetDiffuse( color.red() / 255.f, color.green() / 255.f, color.blue() / 255.f );
}
