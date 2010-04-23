// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "WeatherCreationPanel.h"
#include "moc_WeatherCreationPanel.cpp"
#include "actions/MagicActionMeteo.h"
#include "actions/Numeric.h"
#include "actions/DateTime.h"
#include "actions/Location.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/ParametersLayer.h"
#include "gaming/ActionPublisher.h"
#include "gaming/StaticModel.h"
#include "gaming/ActionTiming.h"
#include "protocol/SimulationSenders.h"
#include "WeatherWidget.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel constructor
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
WeatherCreationPanel::WeatherCreationPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, ActionPublisher& actionPublisher, actions::ActionsModel& actionsModel, const StaticModel& model, const Simulation& simulation, gui::ParametersLayer& layer, const kernel::GlTools_ABC& tools )
    : gui::InfoPanel_ABC( parent, panel, tr( "Weathers" ), "WeatherCreationPanel" )
    , controllers_      ( controllers )
    , layer_            ( layer )
    , actionPublisher_  ( actionPublisher )
    , actionsModel_     ( actionsModel )
    , model_            ( model )
    , simulation_       ( simulation )
    , tools_            ( tools )
    , location_         ( 0 )
    , serializer_       ( model.coordinateConverter_ )
    , isGlobal_         ( false )
{
    
    QGroupBox* localGroup = new QGroupBox( 1, Qt::Horizontal, tr( "Weather" ), this );
    QVBox* localWeatherBox = new QVBox( localGroup );
    weather_ = new WeatherWidget( localWeatherBox, tr( "Weather parameters" ) );

    QGroupBox* parametersGroup = new QGroupBox( 2, Qt::Horizontal, tr( "Time and Position Parameters" ), localWeatherBox );
    new QLabel( tr( "Start time:" ), parametersGroup );
    startTime_ = new QDateTimeEdit( parametersGroup );
    new QLabel( tr( "End time:" ), parametersGroup );
    endTime_ = new QDateTimeEdit( parametersGroup );

    QCheckBox* weatherType = new QCheckBox( tr( "Set as global weather" ), parametersGroup );
    weatherType->setChecked( isGlobal_ );
    
    positionBtn_ = new QPushButton( tr( "Set location" ), parametersGroup );

    QHBox* box = new QHBox( this );
    box->setMaximumHeight( 30 );
    QButton* okBtn     = new QPushButton( tr( "Save" )  , box );
    QButton* cancelBtn = new QPushButton( tr( "Cancel" ), box );

    connect( positionBtn_, SIGNAL( clicked() ), this, SLOT( StartEditingLocation() ) );
    connect( okBtn, SIGNAL( clicked() ), this, SLOT( Commit() ) );
    connect( cancelBtn, SIGNAL( clicked() ), this, SLOT( Reset() ) );
    connect( weatherType, SIGNAL( toggled( bool ) ), this, SLOT( OnToogleWeatherType( bool ) ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel destructor
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
WeatherCreationPanel::~WeatherCreationPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::NotifyUpdated
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
void WeatherCreationPanel::NotifyUpdated( const kernel::ModelLoaded& )
{
    Show();
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::StartEditingLocation
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
void WeatherCreationPanel::StartEditingLocation()
{
    ResetLocation();
    layer_.StartRectangle( *this );
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::CheckValidity
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
bool WeatherCreationPanel::CheckValidity()
{
    return ( isGlobal_ || ( ( startTime_->dateTime() < endTime_->dateTime() ) && location_ ) );
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::Commit
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
void WeatherCreationPanel::Commit()
{
    if( CheckValidity() )
    {      
        MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( model_.types_ ).Get( isGlobal_? "global_meteo" : "local_meteo" );
        MagicActionMeteo* action = new MagicActionMeteo( actionType, controllers_.controller_, true );

        tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
        while( it.HasMoreElements() )
        {
            const OrderParameter& parameter = it.NextElement();

            if( parameter.GetName() == "StartTime" || parameter.GetName() == "EndTime" || parameter.GetName() == "Location" )
            {
                if( isGlobal_ )
                    continue;
                if( parameter.GetName() == "StartTime" )
                    action->AddParameter( *new DateTime( parameter, startTime_->dateTime() ) );
                else if( parameter.GetName() == "EndTime" )
                    action->AddParameter( *new DateTime( parameter, endTime_->dateTime() ) );
                else if( parameter.GetName() == "Location" )
                    action->AddParameter( *new Location( parameter, model_.coordinateConverter_ , *location_ ) );
            }
            else
                action->AddParameter( weather_->CreateParameter( parameter ) );
        }
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
        action->RegisterAndPublish( actionsModel_, actionPublisher_ );
        Reset();
    }
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::OnToogleWeatherType
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void WeatherCreationPanel::OnToogleWeatherType( bool value )
{
    ResetLocation();
    if( value )
    {
        ResetLocation();
        layer_.SelectRaster( *this );
        positionBtn_->setEnabled( false );
        startTime_->setEnabled( false );
        endTime_->setEnabled( false );
        isGlobal_ = true;
    }
    else
    {
        positionBtn_->setEnabled( true );
        startTime_->setEnabled( true );
        endTime_->setEnabled( true );
        isGlobal_ = false;
    }
}
// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::Reset
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
void WeatherCreationPanel::Reset()
{
    ResetLocation();
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::Handle
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void WeatherCreationPanel::Handle( kernel::Location_ABC& location )
{
    if( location.IsValid() )
        location_ = &location;
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::Draw
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void WeatherCreationPanel::Draw( const kernel::Viewport_ABC& /*viewport*/ )
{
    if( location_ && bLocationDisplay_ )
        ShapeHandler_ABC::Draw( *location_, geometry::Rectangle2f(), tools_ );
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::showEvent
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void WeatherCreationPanel::showEvent( QShowEvent* )
{
    bLocationDisplay_ = true;
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::hideEvent
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void WeatherCreationPanel::hideEvent( QHideEvent* )
{
    bLocationDisplay_ = false;
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::ResetLocation
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void WeatherCreationPanel::ResetLocation()
{
    delete location_; location_ = 0;
}