// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ReportPanel.h"
#include "ReportListView.h"
#include "FireResultListView.h"
#include "ReportFilterOptions.h"
#include "clients_kernel/Controllers.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ReportPanel constructor
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
ReportPanel::ReportPanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory )
    : InfoPanel_ABC     ( parent, panel, tr( "Reports" ) )
    , controllers_      ( controllers )
    , selected_         ( controllers )
{
    pFilterOptions_      = new ReportFilterOptions( this );
    pReportListView_     = new ReportListView( this, controllers_, *pFilterOptions_, factory );
    pFireResultListView_ = new FireResultListView( this, factory );
    connect( pFilterOptions_, SIGNAL( OptionsChanged() ), pReportListView_, SLOT( OnOptionsChanged() ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ReportPanel destructor
// Created: AGE 2005-04-21
// -----------------------------------------------------------------------------
ReportPanel::~ReportPanel()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: ReportPanel::NotifySelected
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
void ReportPanel::NotifySelected( const Entity_ABC* element )
{
    if( selected_ != element || ! element )
    {
        selected_ = element;
        if( selected_ )
            Show();
        else
            Hide();
    }
}