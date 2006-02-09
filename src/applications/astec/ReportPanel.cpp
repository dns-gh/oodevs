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
#include "Agent.h"
#include "Agent_ABC.h"
#include "Population.h"

// -----------------------------------------------------------------------------
// Name: ReportPanel constructor
// Created: AGE 2005-04-21
// -----------------------------------------------------------------------------
ReportPanel::ReportPanel( QWidget* pParent )
    : InfoPanel_ABC    ( pParent )
    , pFilterOptions_      ( 0 )
    , pReportListView_     ( 0 )
    , pFireResultListView_ ( 0 )
{
    pFilterOptions_      = new ReportFilterOptions( this );
    pReportListView_     = new ReportListView( this, *pFilterOptions_ );
    pFireResultListView_ = new FireResultListView( this );
    connect( pFilterOptions_, SIGNAL( OptionsChanged() ), pReportListView_, SLOT( OnOptionsChanged() ) );
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
// Name: ReportPanel::OnUpdate
// Created: AGE 2005-04-21
// -----------------------------------------------------------------------------
void ReportPanel::OnUpdate()
{
    if( selectedItem_.pAgent_ != 0 )
        OnAgentUpdated( *selectedItem_.pAgent_ );
    else if( selectedItem_.pPopulation_ != 0 )
        OnAgentUpdated( *selectedItem_.pPopulation_ );   
    else
        OnClearSelection();
}

// -----------------------------------------------------------------------------
// Name: ReportPanel::OnAgentUpdated
// Created: AGE 2005-04-21
// -----------------------------------------------------------------------------
void ReportPanel::OnAgentUpdated( Agent& agent )
{
    if( ShouldDisplay( agent ) )
    {
        pReportListView_    ->SetAgent ( selectedItem_.pAgent_ );
        pFireResultListView_->SetOrigin( selectedItem_.pAgent_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ReportPanel::OnAgentUpdated
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
void ReportPanel::OnAgentUpdated( Population& population )
{
    if( ShouldDisplay( population ) )
    {
        pReportListView_    ->SetAgent ( selectedItem_.pPopulation_ );
        pFireResultListView_->SetOrigin( selectedItem_.pPopulation_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ReportPanel::OnClearSelection
// Created: SBO 2005-09-22
// -----------------------------------------------------------------------------
void ReportPanel::OnClearSelection()
{
    pReportListView_    ->SetAgent ( 0 );
    pFireResultListView_->SetOrigin( 0 );
}
