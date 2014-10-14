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
    enum E_3DLightingType
    {
        eFixed,
        eCameraFixed,
        eSimulationTime
    };
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
                              kernel::OptionsController& options,
                              LightingProxy& lighting )
    : PreferencePanel_ABC( parent, "LightingPanel" )
    , lighting_( lighting )
{
    SubObjectName subObject( "LightingPanel" );

    lighting_.SetAmbient( 0.2f, 0.2f, 0.2f );
    lighting_.SetDiffuse( 0.8f, 0.8f, 0.8f );
    lighting_.SetLightDirection( geometry::Vector3f( 0, 0, 1 ) );

    auto lightingType = new OptionButtonGroup( options, "type_box", "Lighting/Type" );
    lightingType->setTitle( tr( "Lighting type" ) );
    lightingType->AddButton( eFixed, new RichRadioButton( "fixed", tr( "Fixed" ), lightingType ) ); // only this one works well...
    lightingType->AddButton( eCameraFixed, new RichRadioButton( "cameraFixed", tr( "Camera fixed" ), lightingType ) );
    lightingType->AddButton( eSimulationTime, new RichRadioButton( "simulationTime", tr( "Simulation time" ), lightingType ) );
    lightingType->SetChecked( eFixed );

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
// Name: LightingPanel::OnTypeChanged
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void LightingPanel::OnTypeChanged( int type )
{
    fixedBox_->setVisible( type == eFixed || type == eCameraFixed );
    if( type == eFixed )
        lighting_.SwitchToFixed();
    else if( type == eCameraFixed )
        lighting_.SwitchToCameraFixed();
    else if( type == eSimulationTime )
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
