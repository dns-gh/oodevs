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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectReportPanel.cpp $
// $Author: Age $
// $Modtime: 5/04/05 17:12 $
// $Revision: 7 $
// $Workfile: ObjectReportPanel.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ObjectReportPanel.h"
#include "moc_ObjectReportPanel.cpp"

#include "Object_ABC.h"
#include "App.h"
#include "FireResultListView.h"


// -----------------------------------------------------------------------------
// Name: ObjectReportPanel constructor
/** @param  pParent 
*/
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
ObjectReportPanel::ObjectReportPanel( QWidget* pParent )
    : InfoPanel_ABC ( pParent )
{
    pFireResultListView_ = new FireResultListView( this );
}


// -----------------------------------------------------------------------------
// Name: ObjectReportPanel destructor
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
ObjectReportPanel::~ObjectReportPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectReportPanel::OnObjectUpdated
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
void ObjectReportPanel::OnObjectUpdated( Object_ABC& object )
{
    if( ! ShouldDisplay( object ) )
        return;

    pFireResultListView_->SetObject( &object );
}

// -----------------------------------------------------------------------------
// Name: ObjectReportPanel::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void ObjectReportPanel::OnUpdate()
{
    if( selectedItem_.pObject_ != 0 )
        OnObjectUpdated( *selectedItem_.pObject_ );
    else
        OnClearSelection();
}

// -----------------------------------------------------------------------------
// Name: ObjectReportPanel::OnClearSelection
// Created: SBO 2005-09-22
// -----------------------------------------------------------------------------
void ObjectReportPanel::OnClearSelection()
{
    pFireResultListView_->SetObject( 0 );
}
