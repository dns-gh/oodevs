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
#include "actions/Action_ABC.h"
#include "actions/ActionsFilter_ABC.h"
#include "actions/ActionTasker.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Tools.h"

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
    // Init
    setObjectName( "timeLine" );
    setCaption( tools::translate( "TimelinePanel", "Actions timeline" ) );
    QWidget* box = new QWidget( this );
    QVBoxLayout* layout = new QVBoxLayout( box );
    setWidget( box );
    // Toolbar
    toolbar_ = new ActionsToolbar( this, model, config, controllers );
    connect( toolbar_, SIGNAL( PlanificationModeChange() ), parent, SLOT( OnPlanifStateChange() ) );
    // Radio buttons
    QGroupBox* groupBox = new QGroupBox();
    {
        globalView_ = new QRadioButton( tools::translate( "TimelinePanel", "Global view" ) );
        currentView_ = new QRadioButton( tools::translate( "TimelinePanel", "Current session" ) );
        connect( globalView_, SIGNAL( clicked() ), this, SLOT( OnViewChanged() ) );
        connect( currentView_, SIGNAL( clicked() ), this, SLOT( OnViewChanged() ) );
        globalView_->setChecked( true );
        QHBoxLayout* hbox = new QHBoxLayout( groupBox );
        hbox->setMargin( 0 );
        hbox->addWidget( globalView_ );
        hbox->addWidget( currentView_ );
        hbox->addStretch( 1 );
    }
    // Timeline
    timeline_ = new TimelineWidget( this, controllers, model, scheduler, factory );
    // Layout
    layout->addWidget( toolbar_ );
    layout->addWidget( groupBox );
    layout->addWidget( timeline_, 1 );
    // Filters
    filters_.push_back( new GlobalFilter( profile ) );
    filters_.push_back( new CurrentSessionFilter( profile ) );
    OnViewChanged();
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
// Created: ABR 2011-11-25
// -----------------------------------------------------------------------------
void TimelinePanel::OnViewChanged()
{
    const actions::ActionsFilter_ABC& filter = filters_.at( ( globalView_->isChecked() ? 0 : 1 ) );
    toolbar_->SetFilter( filter );
    timeline_->SetFilter( filter );
}

