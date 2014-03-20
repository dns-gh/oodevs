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

#include "actions/ActionsModel.h"
#include "clients_gui/WeatherHelpers.h"
#include "clients_gui/WeatherWidget.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/Tools.h"
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
    , currentModel_( 0 )
{
    // Global Weather
    globalWeatherWidget_ = new gui::WeatherWidget( "globalWidget", globalWidget_, tr( "Weather parameters" ) );
    // Local Weather
    localWeatherWidget_ = new gui::WeatherWidget( "localWidget", localWidget_, tr( "Weather parameters" ) );
    CreateLocalParameters();
    localWeathers_ = new WeatherListView( localWidget_, model.coordinateConverter_, simulation );
    connect( localWeathers_->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( LocalSelectionChanged() ) );

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
    globalWeatherWidget_->Update( *currentModel_->GetGlobalMeteo() );
    static_cast< WeatherListView* >( localWeathers_ )->Update( *currentModel_ );
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::Commit
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
void WeatherPanel::Commit()
{
    if( currentType_ == eWeatherGlobal )
    {
        gui::WeatherParameters params = globalWeatherWidget_->CreateParameters();
        actionsModel_.PublishGlobalWeather( params );
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
                    localWeatherWidget_->Update( *local );
                    gui::LocalWeatherParameters params;
                    params.globalParams_ = localWeatherWidget_->CreateParameters();
                    params.startTime_ = tools::BoostTimeToQTime( local->GetStartTime() );
                    params.endTime_ = tools::BoostTimeToQTime( local->GetEndTime() );
                    params.location_.AddPoint( local->GetBottomRight() );
                    params.location_.AddPoint( local->GetTopLeft() );
                    params.id_ = local->IsCreated() ? 0 : local->GetId();
                    actionsModel_.PublishLocalWeather( params );
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
        while( unsigned int trashedWeather = static_cast< WeatherListView* >( localWeathers_ )->PopTrashedWeather() )
        {
            actionsModel_.PublishLocalDestruction( trashedWeather );
            hasCommited = true;
        }
        if( hasCommited )
            Reset();
    }
}
