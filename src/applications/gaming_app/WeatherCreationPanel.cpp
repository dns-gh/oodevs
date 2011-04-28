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
#include "actions/ActionTiming.h"
#include "actions/DateTime.h"
#include "actions/Location.h"
#include "actions/MagicAction.h"
#include "actions/Numeric.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/ParametersLayer.h"
#include "gaming/MeteoModel.h"
#include "gaming/StaticModel.h"
#include "protocol/SimulationSenders.h"
#include "WeatherWidget.h"

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel constructor
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
WeatherCreationPanel::WeatherCreationPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const StaticModel& model, const kernel::Time_ABC& simulation, gui::ParametersLayer& layer, const kernel::GlTools_ABC& tools )
    : gui::InfoPanel_ABC( parent, panel, tools::translate( "WeatherCreationPanel", "Weathers" ), "WeatherCreationPanel" )
    , controllers_      ( controllers )
    , layer_            ( layer )
    , actionsModel_     ( actionsModel )
    , model_            ( model )
    , simulation_       ( simulation )
    , tools_            ( tools )
    , location_         ( 0 )
    , serializer_       ( model.coordinateConverter_ )
    , isGlobal_         ( true )
{
    QGroupBox* localGroup = new QGroupBox( 1, Qt::Horizontal, tools::translate( "WeatherCreationPanel", "Weather" ), this );
    QVBox* localWeatherBox = new QVBox( localGroup );
    weather_ = new WeatherWidget( localWeatherBox, tr( "Weather parameters" ) );

    QGroupBox* parametersGroup = new QGroupBox( 2, Qt::Horizontal, tools::translate( "WeatherCreationPanel", "Time and Position Parameters" ), localWeatherBox );
    startTimeLabel_ = new QLabel( tools::translate( "WeatherCreationPanel", "Start time:" ), parametersGroup );
    startTime_ = new QDateTimeEdit( parametersGroup );
    endTimeLabel_ = new QLabel( tools::translate( "WeatherCreationPanel", "End time:" ), parametersGroup );
    endTime_ = new QDateTimeEdit( parametersGroup );

    QCheckBox* weatherType = new QCheckBox( tools::translate( "WeatherCreationPanel", "Set as global weather" ), parametersGroup );
    weatherType->setChecked( isGlobal_ );

    positionBtn_ = new QPushButton( tools::translate( "WeatherCreationPanel", "Set location" ), parametersGroup );

    QHBox* box = new QHBox( this );
    box->setMaximumHeight( 30 );
    QButton* okBtn     = new QPushButton( tools::translate( "CreationPanel", "Apply" )  , box );
    QButton* cancelBtn = new QPushButton( tools::translate( "CreationPanel", "Cancel" ), box );

    connect( positionBtn_, SIGNAL( clicked() ), this, SLOT( StartEditingLocation() ) );
    connect( okBtn, SIGNAL( clicked() ), this, SLOT( Commit() ) );
    connect( cancelBtn, SIGNAL( clicked() ), this, SLOT( Reset() ) );
    connect( weatherType, SIGNAL( toggled( bool ) ), this, SLOT( OnToogleWeatherType( bool ) ) );
    controllers_.Register( *this );
    OnToogleWeatherType( true );
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
// Name: WeatherCreationPanel::NotifyUpdated
// Created: SLG 2010-05-20
// -----------------------------------------------------------------------------
void WeatherCreationPanel::NotifyUpdated( const Simulation& simulation )
{
    if( !startTime_->dateTime().isValid() )
        startTime_->setDateTime( simulation.GetInitialDateTime() );
    if( !endTime_->dateTime().isValid() )
        endTime_->setDateTime( simulation.GetInitialDateTime() );
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::NotifyUpdated
// Created: ABR 2011-04-28
// -----------------------------------------------------------------------------
void WeatherCreationPanel::NotifyUpdated( const MeteoModel& meteoModel )
{
    const weather::PHY_Meteo* meteo = meteoModel.GetGlobalMeteo();
    if( meteo )
        weather_->Update( *meteo );
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
    if( isGlobal_ )
        return true;
    else if( ( startTime_->dateTime() > endTime_->dateTime() ) || !startTime_->dateTime().isValid()  || !endTime_->dateTime().isValid() )
    {
        startTimeLabel_->setPaletteForegroundColor( Qt::red );
        endTimeLabel_->setPaletteForegroundColor( Qt::red );
        QTimer::singleShot( 1000, this, SLOT( OnTimeWarnStop() ) );
        return false;
    }
    else if( !location_ )
    {
        positionBtn_->setPaletteForegroundColor( Qt::red );
        QTimer::singleShot( 1000, this, SLOT( OnPosWarnStop() ) );
        return false;
    }
    else
        return true;
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::OnTimeWarnStop
// Created: SLG 2010-05-19
// -----------------------------------------------------------------------------
void WeatherCreationPanel::OnTimeWarnStop()
{
    startTimeLabel_->setPaletteForegroundColor( Qt::black );
    endTimeLabel_->setPaletteForegroundColor( Qt::black );
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::OnPosWarnStop
// Created: SLG 2010-05-19
// -----------------------------------------------------------------------------
void WeatherCreationPanel::OnPosWarnStop()
{
    positionBtn_->setPaletteForegroundColor( Qt::black );
}

// -----------------------------------------------------------------------------
// Name: WeatherCreationPanel::Commit
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
void WeatherCreationPanel::Commit()
{
    if( CheckValidity() )
    {
        kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( model_.types_ ).Get( isGlobal_? "global_weather" : "local_weather" );
        MagicAction* action = new MagicAction( actionType, controllers_.controller_, isGlobal_? tr( "Change Global Weather" ) : tr( "Change Local Weather" ), true );
        tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
        weather_->CreateParameters( *action, it );
        if( !isGlobal_ )
        {
            action->AddParameter( *new DateTime( it.NextElement(), startTime_->dateTime() ) );
            action->AddParameter( *new DateTime( it.NextElement(), endTime_->dateTime() ) );
            action->AddParameter( *new Location( it.NextElement(), model_.coordinateConverter_ , *location_ ) );
        }
        action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
        action->RegisterAndPublish( actionsModel_ );
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