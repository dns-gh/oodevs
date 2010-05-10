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
#include "gaming/Tools.h"
#include <qtabbar.h>

using namespace actions;

namespace
{
    struct GlobalFilter : public actions::ActionsFilter_ABC
    {
        virtual bool Allows( const actions::Action_ABC& /*action*/ ) const
        {
            return true;
        }
    };

    struct CurrentSessionFilter : public actions::ActionsFilter_ABC
    {
        virtual bool Allows( const actions::Action_ABC& action ) const
        {
            const ActionTasker* tasker = action.Retrieve< ActionTasker >();
            return !tasker || !tasker->IsSimulation();
        }
    };

    class TabWidget : public QTabWidget
    {
    public:
        explicit TabWidget( TimelinePanel* panel ) : QTabWidget( panel )
        {
            connect( tabBar(), SIGNAL( selected( int ) ), panel, SLOT( OnViewChanged() ) );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: TimelinePanel constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelinePanel::TimelinePanel( QMainWindow* parent, kernel::Controllers& controllers, ActionsModel& model, ActionsScheduler& scheduler, const tools::ExerciseConfig& config, gui::ItemFactory_ABC& factory )
    : QDockWindow( parent, "timeline" )
{
    setResizeEnabled( true );
    setNewLine( true );
    setCaption( tools::translate( "TimelinePanel", "Actions timeline" ) );
    setCloseMode( QDockWindow::Always );
    tabs_ = new TabWidget( this );
    QVBox* box = new QVBox( tabs_ );
    toolbar_ = new ActionsToolbar( box, model, config, controllers );
    timeline_ = new TimelineWidget( box, controllers, model, scheduler, factory );
    {
        tabs_->addTab( box, tools::translate( "TimelinePanel", "Global view" ) );
        filters_.push_back( new GlobalFilter() );
    }
    {
        tabs_->addTab( box, tools::translate( "TimelinePanel", "Current session" ) );
        filters_.push_back( new CurrentSessionFilter() );
    }
    setWidget( tabs_ );
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
void TimelinePanel::OnViewChanged()
{
    const actions::ActionsFilter_ABC& filter = filters_.at( tabs_->currentPageIndex() );
    toolbar_->SetFilter( filter );
    timeline_->SetFilter( filter );
}
