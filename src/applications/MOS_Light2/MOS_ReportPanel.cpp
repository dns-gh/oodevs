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

#include "MOS_Light2_Pch.h"
#include "MOS_ReportPanel.h"
#include "MOS_ReportListView.h"

// -----------------------------------------------------------------------------
// Name: MOS_ReportPanel constructor
// Created: AGE 2005-04-21
// -----------------------------------------------------------------------------
MOS_ReportPanel::MOS_ReportPanel( QWidget* pParent )
    : MOS_InfoPanel_ABC( pParent )
    , pReportListView_( 0 )
{
    pReportListView_ = new MOS_ReportListView( this );
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
    OnAgentUpdated( *selectedItem_.pAgent_ );
}

// -----------------------------------------------------------------------------
// Name: MOS_ReportPanel::OnAgentUpdated
// Created: AGE 2005-04-21
// -----------------------------------------------------------------------------
void MOS_ReportPanel::OnAgentUpdated( MOS_Agent& agent )
{
    if( ShouldDisplay( agent ) )
        pReportListView_->SetAgent( selectedItem_.pAgent_ );
}
