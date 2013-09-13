// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ProgressIndicator_ABC.h"
#include "ADN_Workspace.h"

// -----------------------------------------------------------------------------
// Name: ADN_ProgressIndicator_ABC constructor
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
ADN_ProgressIndicator_ABC::ADN_ProgressIndicator_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_ProgressIndicator_ABC destructor
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
ADN_ProgressIndicator_ABC::~ADN_ProgressIndicator_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_ProgressIndicator_ABC::SetVisible
// Created: ABR 2012-07-13
// -----------------------------------------------------------------------------
void ADN_ProgressIndicator_ABC::SetVisible( bool visible )
{
    ADN_Workspace::GetWorkspace().GetMainWindow()->statusBar()->setVisible( visible );
}
