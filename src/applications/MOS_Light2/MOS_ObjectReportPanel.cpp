// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-09-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ObjectReportPanel.cpp $
// $Author: Age $
// $Modtime: 5/04/05 17:12 $
// $Revision: 7 $
// $Workfile: MOS_ObjectReportPanel.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_ObjectReportPanel.h"
#include "moc_MOS_ObjectReportPanel.cpp"

#include "MOS_Object_ABC.h"
#include "MOS_App.h"
#include "MOS_FireResultListView.h"


// -----------------------------------------------------------------------------
// Name: MOS_ObjectReportPanel constructor
/** @param  pParent 
*/
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
MOS_ObjectReportPanel::MOS_ObjectReportPanel( QWidget* pParent )
    : MOS_InfoPanel_ABC ( pParent )
{
    pFireResultListView_ = new MOS_FireResultListView( this );
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectReportPanel destructor
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
MOS_ObjectReportPanel::~MOS_ObjectReportPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectReportPanel::OnObjectUpdated
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
void MOS_ObjectReportPanel::OnObjectUpdated( MOS_Object_ABC& object )
{
    if( ! ShouldDisplay( object ) )
        return;

    pFireResultListView_->SetObject( &object );
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectReportPanel::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void MOS_ObjectReportPanel::OnUpdate()
{
    if( selectedItem_.pObject_ != 0 )
        OnObjectUpdated( *selectedItem_.pObject_ );
    else
        OnClearSelection();
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectReportPanel::OnClearSelection
// Created: SBO 2005-09-22
// -----------------------------------------------------------------------------
void MOS_ObjectReportPanel::OnClearSelection()
{
    pFireResultListView_->SetAgent( 0 );
}
