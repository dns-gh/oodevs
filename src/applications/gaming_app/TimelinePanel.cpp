// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TimelinePanel.h"
#include "moc_TimelinePanel.cpp"
#include "ActionsToolbar.h"
#include "TimelineWidget.h"
#include "TabWidget.h"
#include "actions/Action_ABC.h"
#include "actions/ActionsFilter_ABC.h"
#include "actions/ActionTasker.h"
#include "clients_kernel/Profile_ABC.h"
#include "gaming/Tools.h"

using namespace actions;

namespace
{
    struct ProfileActionFilter : private boost::noncopyable
                               , public actions::ActionsFilter_ABC
    {
        explicit ProfileActionFilter( const kernel::Profile_ABC& profile )
            : profile_( profile )
        {
            // NOTHING
        }

        virtual bool Allows( const actions::Action_ABC& action ) const
        {
            if( const ActionTasker* tasker = action.Retrieve< ActionTasker >() )
            {
                if( const kernel::Entity_ABC* entity = tasker->GetTasker() )
                    return profile_.IsVisible( *entity );
            }
            return true;
        }

        const kernel::Profile_ABC& profile_;
    };
    struct GlobalFilter : public ProfileActionFilter
    {
        explicit GlobalFilter( const kernel::Profile_ABC& profile )
            : ProfileActionFilter( profile )
        {
            // NOTHING
        }

        virtual bool Allows( const actions::Action_ABC& action ) const
        {
            return ProfileActionFilter::Allows( action );
        }
    };

    struct CurrentSessionFilter : public ProfileActionFilter
    {
        explicit CurrentSessionFilter( const kernel::Profile_ABC& profile )
            : ProfileActionFilter( profile )
        {
            // NOTHING
        }

        virtual bool Allows( const actions::Action_ABC& action ) const
        {
            if( ProfileActionFilter::Allows( action ) )
            {
                const ActionTasker* tasker = action.Retrieve< ActionTasker >();
                return !tasker || !tasker->IsSimulation();
            }
            return false;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: TimelinePanel constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelinePanel::TimelinePanel( QMainWindow* parent, kernel::Controllers& controllers, ActionsModel& model, ActionsScheduler& scheduler, const Config& config, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile )
    : QDockWidget( "timeline", parent )
{
    setObjectName( "timeLine" );
    setCaption( tools::translate( "TimelinePanel", "Actions timeline" ) );
    Q3VBox* box = new Q3VBox( this );
    toolbar_ = new ActionsToolbar( box, model, config, controllers );
    tabs_ = new TabWidget( box );
    timeline_ = new TimelineWidget( box, controllers, model, scheduler, factory );
    connect( toolbar_, SIGNAL(PlanificationModeChange() ), this, SIGNAL( PlanificationModeChange()) );
    static_cast< TabWidget* >( tabs_ )->setConnect( timeline_, filters_, toolbar_ );
    {
        Q3VBox* box = new Q3VBox( tabs_ );
        filters_.push_back( new GlobalFilter( profile ) );
        tabs_->addTab( box, tools::translate( "TimelinePanel", "Global view" ) );
    }
    {
        Q3VBox* box = new Q3VBox( tabs_ );
        filters_.push_back( new CurrentSessionFilter( profile ) );
        tabs_->addTab( box, tools::translate( "TimelinePanel", "Current session" ) );
    }
    setWidget( box );
}

// -----------------------------------------------------------------------------
// Name: TimelinePanel destructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelinePanel::~TimelinePanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelinePanel::OnViewChanged
// Created: SBO 2010-05-06
// -----------------------------------------------------------------------------
