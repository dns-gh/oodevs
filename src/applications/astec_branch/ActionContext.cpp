// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-16 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ActionContext.cpp $
// $Author: Ape $
// $Modtime: 5/05/04 15:42 $
// $Revision: 2 $
// $Workfile: ActionContext.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ActionContext.h"

// -----------------------------------------------------------------------------
// Name: ActionContext constructor
/** @param  pPoint 
*/
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
ActionContext::ActionContext( MT_Vector2D* pPoint )
    : selectedElement_  ()
    , pPoint_           ( pPoint )
{
}


// -----------------------------------------------------------------------------
// Name: ActionContext constructor
/** @param  selectedElement 
    @param  pPoint 
*/
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
ActionContext::ActionContext( SelectedElement& selectedElement, MT_Vector2D* pPoint )
    : selectedElement_  ( selectedElement)
    , pPoint_           ( pPoint )
{
}


// -----------------------------------------------------------------------------
// Name: ActionContext destructor
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
ActionContext::~ActionContext()
{
}
