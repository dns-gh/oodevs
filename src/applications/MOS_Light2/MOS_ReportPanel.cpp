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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ReportPanel.cpp $
// $Author: Age $
// $Modtime: 4/05/05 19:13 $
// $Revision: 2 $
// $Workfile: MOS_ReportPanel.cpp $
//
// *****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_ReportPanel.h"
#include "MOS_ReportListView.h"
#include "MOS_FireResultListView.h"
#include "MOS_ReportFilterOptions.h"
#include "MOS_Agent.h"
#include "MOS_Agent_ABC.h"
#include "MOS_Population.h"

// -----------------------------------------------------------------------------
// Name: MOS_ReportPanel constructor
// Created: AGE 2005-04-21
// -----------------------------------------------------------------------------
MOS_ReportPanel::MOS_ReportPanel( QWidget* pParent )
    : MOS_InfoPanel_ABC    ( pParent )
    , pFilterOptions_      ( 0 )
    , pReportListView_     ( 0 )
    , pFireResultListView_ ( 0 )
{
    pFilterOptions_      = new MOS_ReportFilterOptions( this );
    pReportListView_     = new MOS_ReportListView( this, *pFilterOptions_ );
    pFireResultListView_ = new MOS_FireResultListView( this );
    connect( pFilterOptions_, SIGNAL( OptionsChanged() ), pReportListView_, SLOT( OnOptionsChanged() ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_ReportPanel destructor
// Created: AGE 2005-04-21
// -----------------------------------------------------------------------------
MOS_ReportPanel::~MOS_ReportPanel()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_ReportPanel::OnUpdate
// Created: AGE 2005-04-21
// -----------------------------------------------------------------------------
void MOS_ReportPanel::OnUpdate()
{
    if( selectedItem_.pAgent_ != 0 )
        OnAgentUpdated( *selectedItem_.pAgent_ );
    if( selectedItem_.pPopulation_ != 0 )
        OnAgentUpdated( *selectedItem_.pPopulation_ );   
    else
        OnClearSelection();
}

// -----------------------------------------------------------------------------
// Name: MOS_ReportPanel::OnAgentUpdated
// Created: AGE 2005-04-21
// -----------------------------------------------------------------------------
void MOS_ReportPanel::OnAgentUpdated( MOS_Agent& agent )
{
    if( ShouldDisplay( agent ) )
    {
        pReportListView_->SetAgent( selectedItem_.pAgent_ );
        pFireResultListView_->SetAgent( selectedItem_.pAgent_ );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_ReportPanel::OnAgentUpdated
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
void MOS_ReportPanel::OnAgentUpdated( MOS_Population& population )
{
    if( ShouldDisplay( population ) )
    {
        pReportListView_->SetAgent( selectedItem_.pPopulation_ );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_ReportPanel::OnClearSelection
// Created: SBO 2005-09-22
// -----------------------------------------------------------------------------
void MOS_ReportPanel::OnClearSelection()
{
    pReportListView_->SetAgent( 0 );
    pFireResultListView_->SetAgent( 0 );
}
