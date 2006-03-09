// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-04-21 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ReportPanel.cpp $
// $Author: Age $
// $Modtime: 4/05/05 19:13 $
// $Revision: 2 $
// $Workfile: ReportPanel.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "ReportPanel.h"
#include "ReportListView.h"
//#include "FireResultListView.h"
#include "ReportFilterOptions.h"
#include "Controller.h"
#include "ActionController.h"

// -----------------------------------------------------------------------------
// Name: ReportPanel constructor
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
ReportPanel::ReportPanel( InfoPanel* pParent, Controller& controller, ActionController& actionController )
    : InfoPanel_ABC     ( pParent, tr( "Rapports" ) )
{
    pFilterOptions_      = new ReportFilterOptions( this );
    pReportListView_     = new ReportListView( this, controller, actionController, *pFilterOptions_ );
//    pFireResultListView_ = new FireResultListView( this, controller, actionController );
    connect( pFilterOptions_, SIGNAL( OptionsChanged() ), pReportListView_, SLOT( OnOptionsChanged() ) );

    controller.Register( *this );
    actionController.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ReportPanel destructor
// Created: AGE 2005-04-21
// -----------------------------------------------------------------------------
ReportPanel::~ReportPanel()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReportPanel::NotifySelected
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
void ReportPanel::NotifySelected( const Agent_ABC* element )
{
    if( element )
        Show();
    else
        Hide();
}