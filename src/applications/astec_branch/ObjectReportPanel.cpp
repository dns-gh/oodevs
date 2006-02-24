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

#include "astec_pch.h"
#include "ObjectReportPanel.h"

#include "Object_ABC.h"
//#include "FireResultListView.h"


// -----------------------------------------------------------------------------
// Name: ObjectReportPanel constructor
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
ObjectReportPanel::ObjectReportPanel( InfoPanel* pParent, Controller& controller, ActionController& actionController )
    : InfoPanel_ABC( pParent, "Rapports" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectReportPanel destructor
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
ObjectReportPanel::~ObjectReportPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectReportPanel::NotifyUpdated
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void ObjectReportPanel::NotifyUpdated( const Object_ABC& )
{

}

// -----------------------------------------------------------------------------
// Name: ObjectReportPanel::NotifyDeleted
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void ObjectReportPanel::NotifyDeleted( const Object_ABC& object )
{

}

// -----------------------------------------------------------------------------
// Name: ObjectReportPanel::NotifySelected
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void ObjectReportPanel::NotifySelected( const Object_ABC* object )
{

}
