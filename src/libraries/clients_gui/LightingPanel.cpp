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
#include "ButtonGroup.h"
#include "ColorButton.h"
#include "DirectionWidget.h"
#include "LightingProxy.h"
#include "RichGroupBox.h"
#include "RichRadioButton.h"
#include "SubObjectName.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LightingPanel constructor
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
LightingPanel::LightingPanel( QWidget* parent, LightingProxy& lighting, kernel::Controllers& controllers )
    : PreferencePanel_ABC( parent, "LightingPanel" )
    , controllers_( controllers )
    , options_( controllers.options_ )
    , lighting_( lighting )
{
    SubObjectName subObject( "LightingPanel" );
    QVBoxLayout* container = new QVBoxLayout( this );
    container->setSizeConstraint( QLayout::SetMinimumSize );

    lighting_.SetAmbient( 0.2f, 0.2f, 0.2f );
    lighting_.SetDiffuse( 0.8f, 0.8f, 0.8f );
    lighting_.SetLightDirection( geometry::Vector3f( 0, 0, 1 ) );

    // $$$$ SBO 2007-01-03: Todo, handle lighting types different from fixed
    lightingType_ = new ButtonGroup( 3, Qt::Horizontal, tr( "Lighting type" ) );
    lightingType_->resize( this->size() );
    lightingType_->insert( new RichRadioButton( "fixed", tr( "Fixed" ), lightingType_ ) );
    lightingType_->insert( new RichRadioButton( "cameraFixed", tr( "Camera fixed" ), lightingType_ ) );
    lightingType_->insert( new RichRadioButton( "simulationTime", tr( "Simulation time" ), lightingType_ ) );
    lightingType_->setButton( 0 );
    container->addWidget( lightingType_ );

    connect( lightingType_, SIGNAL( clicked( int ) ), this, SLOT( OnLightingType( int ) ) );

    QLabel* sourceLabel = new QLabel( tr( "Source position" ) );
    direction_ = new DirectionWidget( 0 );
    QLabel* ambientLabel = new QLabel( tr( "Ambient color" ) );
    ambient_ = new ColorButton( "ambient", 0, "", QColor( 52, 52, 52 ) );
    QLabel* diffuseLabel = new QLabel( tr( "Diffuse color" ) );
    diffuse_ = new ColorButton( "diffuse", 0, "", QColor( 204, 204, 204 ) );

    fixedLightBox_ = new RichGroupBox( "fixedLightBox", tr( "Parameters" ) );
    QGridLayout* fixedLightBoxLayout = new QGridLayout( fixedLightBox_ );
    fixedLightBoxLayout->addWidget( sourceLabel, 0, 0 );
    fixedLightBoxLayout->addWidget( direction_, 0 ,1 );
    fixedLightBoxLayout->addWidget( ambientLabel, 1, 0 );
    fixedLightBoxLayout->addWidget( ambient_, 1, 1 );
    fixedLightBoxLayout->addWidget( diffuseLabel, 2, 0) ;
    fixedLightBoxLayout->addWidget( diffuse_, 2, 1 );
    container->addWidget( fixedLightBox_ );
    container->addStretch( 1 );

    connect( direction_, SIGNAL( DirectionChanged( const geometry::Vector3f& ) ), this, SLOT( DirectionChanged( const geometry::Vector3f& ) ) );
    connect( ambient_, SIGNAL( ColorChanged( const QColor& ) ), this, SLOT( AmbientChanged( const QColor& ) ) );
    connect( diffuse_, SIGNAL( ColorChanged( const QColor& ) ), this, SLOT( DiffuseChanged( const QColor& ) ) );

    setLayout( container );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LightingPanel destructor
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
LightingPanel::~LightingPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LightingPanel::Commit
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void LightingPanel::Commit()
{
    lightingType_->Commit();
    direction_->Commit();
    ambient_  ->Commit();
    diffuse_  ->Commit();

    options_.Change( "Lighting/Type", lightingType_->selectedId() );
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
    lightingType_->Revert();
    OnLightingType(lightingType_->selectedId());
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
    fixedLightBox_->setVisible( type == 0 || type == 1 );
    if( type == 0 )
        lighting_.SwitchToFixed();
    else if( type == 1 )
        lighting_.SwitchToCameraFixed();
    else if( type == 2 )
        lighting_.SwitchToSimulationTime();
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
    if( option[1] == "Type" )
    {
        lightingType_->setButton( value.To< int >());
        lightingType_->Commit();
    }
    else if( option[1] == "Ambient" )
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
