// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LightingPanel.h"
#include "moc_LightingPanel.cpp"
#include "ColorButton.h"
#include "DirectionWidget.h"
#include "graphics/FixedLighting.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LightingPanel constructor
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
LightingPanel::LightingPanel( QWidget* parent, FixedLighting& lighting )
    : PreferencePanel_ABC( parent )
    , lighting_( lighting )
{
    lighting_.SetAmbient( 0.2f, 0.2f, 0.2f );
    lighting_.SetDiffuse( 0.8f, 0.8f, 0.8f );
    lighting_.SetLightDirection( geometry::Vector3f( 0, 0, 1 ) );

    lightingType_ = new QButtonGroup( 3, Qt::Horizontal, tr( "Lighting type" ), this );
    lightingType_->insert( new QRadioButton( tr( "Fixed" ), lightingType_ ) );
    lightingType_->insert( new QRadioButton( tr( "Simulation time" ), lightingType_ ) );
    lightingType_->insert( new QRadioButton( tr( "Real time" ), lightingType_ ) );
    lightingType_->setButton( 0 );

    QGroupBox* lightBox = new QGroupBox( 2, Qt::Horizontal, tr( "Parameters" ), this );
    new QLabel( tr( "Source position" ), lightBox );
    DirectionWidget* direction = new DirectionWidget( lightBox );
    new QLabel( tr( "Ambient color" ), lightBox );
    ColorButton* ambient = new ColorButton( lightBox, "", QColor( 50, 50, 50 ) );
    new QLabel( tr( "Diffuse color" ), lightBox );
    ColorButton* diffuse = new ColorButton( lightBox, "", QColor( 200, 200, 200 ) );

    connect( direction, SIGNAL( DirectionChanged( const geometry::Vector3f& ) ), this, SLOT( DirectionChanged( const geometry::Vector3f& ) ) );
    connect( ambient, SIGNAL( ColorChanged( const QColor& ) ), this, SLOT( AmbientChanged( const QColor& ) ) );
    connect( diffuse, SIGNAL( ColorChanged( const QColor& ) ), this, SLOT( DiffuseChanged( const QColor& ) ) );
    // $$$$ SBO 2007-01-03: Todo, handle lighting types different from fixed
}
    
// -----------------------------------------------------------------------------
// Name: LightingPanel destructor
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
LightingPanel::~LightingPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LightingPanel::Commit
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void LightingPanel::Commit()
{
     // $$$$ SBO 2007-01-03: TODO !
}

// -----------------------------------------------------------------------------
// Name: LightingPanel::Reset
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void LightingPanel::Reset()
{
    // $$$$ SBO 2007-01-03: TODO !
}

// -----------------------------------------------------------------------------
// Name: LightingPanel::DirectionChanged
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void LightingPanel::DirectionChanged( const geometry::Vector3f& direction )
{
    lighting_.SetLightDirection( direction );
}

// -----------------------------------------------------------------------------
// Name: LightingPanel::AmbientChanged
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void LightingPanel::AmbientChanged( const QColor& color )
{
    lighting_.SetAmbient( color.red() / 255.f, color.green() / 255.f, color.blue() / 255.f );
}

// -----------------------------------------------------------------------------
// Name: LightingPanel::DiffuseChanged
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void LightingPanel::DiffuseChanged( const QColor& color )
{
    lighting_.SetDiffuse( color.red() / 255.f, color.green() / 255.f, color.blue() / 255.f );
}
