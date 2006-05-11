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
#include "FireResultListView.h"
#include "ReportFilterOptions.h"
#include "Controllers.h"

// -----------------------------------------------------------------------------
// Name: ReportPanel constructor
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
ReportPanel::ReportPanel( InfoPanels* pParent, Controllers& controllers, ItemFactory_ABC& factory )
    : InfoPanel_ABC     ( pParent, tr( "Rapports" ) )
    , controllers_      ( controllers )
    , selected_         ( 0 )
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
void ReportPanel::NotifySelected( const Agent_ABC* element )
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