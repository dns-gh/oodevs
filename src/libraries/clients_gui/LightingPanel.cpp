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
#include "DirectionWidget.h"
#include "GLOptions.h"
#include "GlProxy.h"
#include "LightingHelpers.h"
#include "LightingProxy.h"
#include "OptionWidgets.h"
#include "RichGroupBox.h"
#include "RichRadioButton.h"
#include "SignalAdapter.h"
#include "SubObjectName.h"
#include "Tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"

using namespace gui;

namespace
{
    class OptionDirection : public OptionWidget< DirectionWidget >
    {
    public:
        OptionDirection( kernel::OptionsController& options,
                         const QString& objectName,
                         const std::string& optionName,
                         QWidget* parent = 0 )
            : OptionWidget< DirectionWidget >( options, objectName, optionName, parent )
        {
            gui::connect( this, SIGNAL( DirectionChanged( const geometry::Vector3f& ) ), [=,&options]{
                options.Change( optionName, GetValue() );
            } );
        }
    private:
        virtual void OnOptionChanged( const kernel::OptionVariant& value )
        {
            SetValue( value.To< QString >() );
        }
    };
    class OptionButtonGroup : public OptionWidget< ButtonGroup >
    {
    public:
        OptionButtonGroup( kernel::OptionsController& options,
                           const QString& objectName,
                           const std::string& optionName,
                           QWidget* parent = 0 )
            : OptionWidget< ButtonGroup >( options, objectName, optionName, parent )
        {
            gui::connect( this, SIGNAL( ButtonClicked( int ) ), [=,&options]{
                options.Change( optionName, CheckedId() );
            } );
        }
    private:
        virtual void OnOptionChanged( const kernel::OptionVariant& value )
        {
            SetChecked( value.To< int >() );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: LightingPanel constructor
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
LightingPanel::LightingPanel( QWidget* parent,
                              kernel::OptionsController& options )
    : PreferencePanel_ABC( parent, "LightingPanel" )
{
    SubObjectName subObject( "LightingPanel" );

    auto lightingType = new OptionButtonGroup( options, "type_box", "Lighting/Type" );
    lightingType->setTitle( tr( "Lighting type" ) );
    lightingType->AddButton( lighting::eFixed, new RichRadioButton( "fixed", tr( "Fixed" ), lightingType ) ); // only this one works well...
    lightingType->AddButton( lighting::eCameraFixed, new RichRadioButton( "cameraFixed", tr( "Camera fixed" ), lightingType ) );
    lightingType->AddButton( lighting::eSimulationTime, new RichRadioButton( "simulationTime", tr( "Simulation time" ), lightingType ) );
    lightingType->SetChecked( lighting::eFixed );

    auto direction = new OptionDirection( options, "direction", "Lighting/Direction" );
    auto ambient = new OptionColorButton( options, "ambient", "Lighting/Ambient" );
    auto diffuse = new OptionColorButton( options, "diffuse", "Lighting/Diffuse" );

    fixedBox_ = new RichGroupBox( "fixedLightBox", tr( "Parameters" ) );
    QVBoxLayout* fixedLayout = new QVBoxLayout( fixedBox_ );
    fixedLayout->addWidget( tools::AddLabeledWidget( tr( "Source position" ), direction ) );
    fixedLayout->addWidget( tools::AddLabeledWidget( tr( "Ambient color" ), ambient ) );
    fixedLayout->addWidget( tools::AddLabeledWidget( tr( "Diffuse color" ), diffuse ) );

    QVBoxLayout* container = new QVBoxLayout( this );
    container->setSizeConstraint( QLayout::SetMinimumSize );
    container->addWidget( lightingType );
    container->addWidget( fixedBox_ );
    container->addStretch( 1 );

    connect( lightingType, SIGNAL( ButtonClicked( int ) ), this, SLOT( OnTypeChanged( int ) ) );
    connect( direction, SIGNAL( DirectionChanged( const geometry::Vector3f& ) ),
             this, SLOT( DirectionChanged( const geometry::Vector3f& ) ) );
    connect( ambient, SIGNAL( ColorChanged( const QColor& ) ), this, SLOT( AmbientChanged( const QColor& ) ) );
    connect( diffuse, SIGNAL( ColorChanged( const QColor& ) ), this, SLOT( DiffuseChanged( const QColor& ) ) );

    setLayout( container );
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
// Name: LightingPanel::Load
// Created: ABR 2014-10-20
// -----------------------------------------------------------------------------
void LightingPanel::Load( const GlProxy& view )
{
    const auto& options = view.GetOptions();
    lighting_ = std::dynamic_pointer_cast< LightingProxy >( options.GetLighting() );
}

// -----------------------------------------------------------------------------
// Name: LightingPanel::OnTypeChanged
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void LightingPanel::OnTypeChanged( int type )
{
    fixedBox_->setVisible( type == lighting::eFixed || type == lighting::eCameraFixed );
    if( !lighting_ )
        return;
    if( type == lighting::eFixed )
        lighting_->SwitchToFixed();
    else if( type == lighting::eCameraFixed )
        lighting_->SwitchToCameraFixed();
    else if( type == lighting::eSimulationTime )
        lighting_->SwitchToSimulationTime();
}

// -----------------------------------------------------------------------------
// Name: LightingPanel::DirectionChanged
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void LightingPanel::DirectionChanged( const geometry::Vector3f& direction )
{
    if( lighting_ )
        lighting_->SetLightDirection( direction );
}

// -----------------------------------------------------------------------------
// Name: LightingPanel::AmbientChanged
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void LightingPanel::AmbientChanged( const QColor& color )
{
    if( lighting_ )
        lighting_->SetAmbient( color );
}

// -----------------------------------------------------------------------------
// Name: LightingPanel::DiffuseChanged
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void LightingPanel::DiffuseChanged( const QColor& color )
{
    if( lighting_ )
        lighting_->SetDiffuse( color );
}
