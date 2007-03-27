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
#include "LightingProxy.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LightingPanel constructor
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
LightingPanel::LightingPanel( QWidget* parent, LightingProxy& lighting, kernel::Controllers& controllers )
    : PreferencePanel_ABC( parent )
    , controllers_( controllers )
    , options_( controllers.options_ )
    , lighting_( lighting )
{
    lighting_.SetAmbient( 0.2f, 0.2f, 0.2f );
    lighting_.SetDiffuse( 0.8f, 0.8f, 0.8f );
    lighting_.SetLightDirection( geometry::Vector3f( 0, 0, 1 ) );

    // $$$$ SBO 2007-01-03: Todo, handle lighting types different from fixed
    QButtonGroup* lightingType = new QButtonGroup( 3, Qt::Horizontal, tr( "Lighting type" ), this );
    lightingType->insert( new QRadioButton( tr( "Fixed" ), lightingType ) );
    lightingType->insert( new QRadioButton( tr( "Simulation time" ), lightingType ) );
    lightingType->insert( new QRadioButton( tr( "Real time" ), lightingType ) );
    lightingType->setButton( 0 );

    connect( lightingType, SIGNAL( clicked( int ) ), this, SLOT( OnLightingType( int ) ) );

    fixedLightBox_ = new QGroupBox( 2, Qt::Horizontal, tr( "Parameters" ), this );
    new QLabel( tr( "Source position" ), fixedLightBox_ );
    direction_ = new DirectionWidget( fixedLightBox_ );
    new QLabel( tr( "Ambient color" ), fixedLightBox_ );
    ambient_ = new ColorButton( fixedLightBox_, "", QColor( 52, 52, 52 ) );
    new QLabel( tr( "Diffuse color" ), fixedLightBox_ );
    diffuse_ = new ColorButton( fixedLightBox_, "", QColor( 204, 204, 204 ) );

    connect( direction_, SIGNAL( DirectionChanged( const geometry::Vector3f& ) ), this, SLOT( DirectionChanged( const geometry::Vector3f& ) ) );
    connect( ambient_, SIGNAL( ColorChanged( const QColor& ) ), this, SLOT( AmbientChanged( const QColor& ) ) );
    connect( diffuse_, SIGNAL( ColorChanged( const QColor& ) ), this, SLOT( DiffuseChanged( const QColor& ) ) );

    controllers_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: LightingPanel destructor
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
LightingPanel::~LightingPanel()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: LightingPanel::Commit
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void LightingPanel::Commit()
{
    direction_->Commit();
    ambient_  ->Commit();
    diffuse_  ->Commit();

    options_.Change( "Lighting/Ambient", ambient_->GetColor().name() );
    options_.Change( "Lighting/Diffuse", diffuse_->GetColor().name() );
    options_.Change( "Lighting/Direction", direction_->GetValue() );
}

// -----------------------------------------------------------------------------
// Name: LightingPanel::Reset
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void LightingPanel::Reset()
{
    direction_->Revert();
    ambient_  ->Revert();
    diffuse_  ->Revert();
}

// -----------------------------------------------------------------------------
// Name: LightingPanel::OnLightingType
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void LightingPanel::OnLightingType( int type )
{
    fixedLightBox_->setShown( type == 0 );
    if( type == 0 )
        lighting_.SwitchToFixed();
    else if( type == 1 )
        lighting_.SwitchToClockTime();
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

// -----------------------------------------------------------------------------
// Name: LightingPanel::OptionChanged
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
void LightingPanel::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    const QStringList option = QStringList::split( "/", name.c_str() );
    if( !( option[0] == "Lighting" ) )
        return;
    if( option[1] == "Ambient" )
    {
        ambient_->SetColor( QColor( value.To< QString >() ) );
        ambient_->Commit();
    }
    else if( option[1] == "Diffuse" )
    {
        diffuse_->SetColor( QColor( value.To< QString >() ) );
        diffuse_->Commit();
    }
    else if( option[1] == "Direction" )
    {
        direction_->SetValue( value.To< QString >() );
        direction_->Commit();
    }
}
