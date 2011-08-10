// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TabWidget.h"
#include "moc_TabWidget.cpp"
#include "ActionsToolbar.h"
#include "TimelineWidget.h"

// -----------------------------------------------------------------------------
// Name: TabWidget constructor
// Created: FPT 2011-08-08
// -----------------------------------------------------------------------------
TabWidget::TabWidget( Q3VBox* box ) 
    : QTabWidget( box )
    , filters_( 0 )
    , toolbar_( 0 )
    , timeline_( 0 )
{
  // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TabWidget::OnViewChanged
// Created: FPT 2011-08-08
// -----------------------------------------------------------------------------
void TabWidget::OnViewChanged()
{
   const actions::ActionsFilter_ABC& filter = filters_->at( currentPageIndex() );
   toolbar_->SetFilter( filter );
   timeline_->SetFilter( filter );
}

// -----------------------------------------------------------------------------
// Name: TabWidge::setConnect
// Created: FPT 2011-08-08
// -----------------------------------------------------------------------------
void TabWidget::setConnect( TimelineWidget* timeline, TimelinePanel::T_Filters& filters, ActionsToolbar* toolbar)
{
   toolbar_ = toolbar;
   timeline_ = timeline;
   filters_ = &filters;
   connect( this, SIGNAL( selected( const QString& ) ), SLOT( OnViewChanged() ) );
}