// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "WeatherPanel.h"
#include "moc_WeatherPanel.cpp"

#include "WeatherLayer.h"
#include "WeatherListView.h"
#include "WeatherWidget.h"
#include "actions/ActionTiming.h"
#include "actions/DateTime.h"
#include "actions/Location.h"
#include "actions/MagicAction.h"
#include "actions/Identifier.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/Rectangle.h"
#include "gaming/MeteoModel.h"
#include "gaming/Simulation.h"
#include "gaming/StaticModel.h"
#include "meteo/MeteoLocal.h"

// -----------------------------------------------------------------------------
// Name: WeatherPanel constructor
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
WeatherPanel::WeatherPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, actions::ActionsModel& actionsModel,
                            const StaticModel& model, const kernel::Time_ABC& simulation, WeatherLayer& layer )
    : gui::WeatherPanel( parent, panel, layer )
    , controllers_ ( controllers )
    , actionsModel_( actionsModel )
    , model_       ( model )
    , simulation_  ( simulation )
    , currentModel_( 0 )
{
    // Global Weather
    globalWidget_ = new WeatherWidget( globalLayout_, tr( "Weather parameters" ) );
    // Local Weather
    localWidget_ = new WeatherWidget( localLayout_, tr( "Weather parameters" ) );
    CreateLocalParameters();
    localWeathers_ = new WeatherListView( localLayout_, model.coordinateConverter_, simulation_ );
    connect( localWeathers_, SIGNAL( selectionChanged() ), this, SLOT( LocalSelectionChanged() ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel destructor
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
WeatherPanel::~WeatherPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::Reset
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
void WeatherPanel::Reset()
{
    layer_.Clear();
    selectedLocal_ = 0;
    if( localWeathers_ != 0 )
        while( static_cast< WeatherListView* >( localWeathers_ )->PopTrashedWeather() != 0 )
            ;
    if( currentModel_ )
        NotifyUpdated( *currentModel_ );
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::NotifyUpdated
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
void WeatherPanel::NotifyUpdated( const MeteoModel& model )
{
    currentModel_ = const_cast< MeteoModel* >( &model );
    globalWidget_->Update( *currentModel_->GetGlobalMeteo() );
    static_cast< WeatherListView* >( localWeathers_ )->Update( *currentModel_ );
    Show();
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::Commit
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
void WeatherPanel::Commit()
{
    if( currentType_ == eWeatherGlobal )
    {
        assert( currentModel_->GetGlobalMeteo() );
        kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( model_.types_ ).Get( "global_weather" );
        actions::MagicAction* action = new actions::MagicAction( actionType, controllers_.controller_, tr( "Change Global Weather" ), true );
        tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
        static_cast< WeatherWidget* >( globalWidget_ )->CreateParameters( *action, it );
        action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
        action->RegisterAndPublish( actionsModel_ );
        const_cast< weather::Meteo* >( currentModel_->GetGlobalMeteo() )->SetModified( false );
        Reset();
    }
    else
    {
        // Creation / Update
        CommitLocalWeather();
        bool hasCommited = false;
        const gui::WeatherListView::T_Weathers& weathers = localWeathers_->GetLocalWeathers();
        for( gui::WeatherListView::CIT_Weathers it = weathers.begin(); it != weathers.end(); ++it )
        {
            weather::MeteoLocal* local = static_cast< weather::MeteoLocal* >( ( *it ).get() );
            assert( local );
            if( local->IsModified() )
            {
                if( local->IsValid() )
                {
                    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( model_.types_ ).Get( "local_weather" );
                    actions::MagicAction* action = new actions::MagicAction( actionType, controllers_.controller_, tr( "Change Local Weather" ), true );
                    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();

                    localWidget_->Update( *local );
                    static_cast< WeatherWidget* >( localWidget_ )->CreateParameters( *action, it );
                    action->AddParameter( *new actions::parameters::DateTime( it.NextElement(), local->GetStartTime() ) );
                    action->AddParameter( *new actions::parameters::DateTime( it.NextElement(), local->GetEndTime() ) );
                    kernel::Rectangle rectangle;
                    rectangle.AddPoint( local->GetBottomRight() );
                    rectangle.AddPoint( local->GetTopLeft() );
                    action->AddParameter( *new actions::parameters::Location( it.NextElement(), model_.coordinateConverter_, rectangle ) );
                    action->AddParameter( *new actions::parameters::Identifier( it.NextElement(), ( local->IsCreated() ) ? 0 : local->GetId() ) );

                    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
                    action->RegisterAndPublish( actionsModel_ );
                    hasCommited = true;
                    local->SetModified( false );
                }
                else
                {
                    QMessageBox::warning( this, tr( "SWORD" ), tr( "Invalid local weather %1, please correct time or position settings" ).arg( local->GetName().c_str() ) );
                    continue;
                }
            }
        }
        // Destruction
        while( unsigned long trashedWeather = static_cast< WeatherListView* >( localWeathers_ )->PopTrashedWeather() )
        {
            kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( model_.types_ ).Get( "local_weather_destruction" );
            actions::MagicAction* action = new actions::MagicAction( actionType, controllers_.controller_, tr( "Local Weather Destruction" ), true );
            tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();

            action->AddParameter( *new actions::parameters::Identifier( it.NextElement(), trashedWeather ) );
            action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
            action->RegisterAndPublish( actionsModel_ );
            hasCommited = true;
        }
        if( hasCommited )
            Reset();
    }
}
