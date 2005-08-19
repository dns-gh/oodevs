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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ActionContext.cpp $
// $Author: Ape $
// $Modtime: 5/05/04 15:42 $
// $Revision: 2 $
// $Workfile: MOS_ActionContext.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_ActionContext.h"

#ifndef MOS_USE_INLINE
#   include "MOS_ActionContext.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MOS_ActionContext constructor
/** @param  pPoint 
*/
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
MOS_ActionContext::MOS_ActionContext( MT_Vector2D* pPoint )
    : selectedElement_  ()
    , pPoint_           ( pPoint )
{
}


// -----------------------------------------------------------------------------
// Name: MOS_ActionContext constructor
/** @param  selectedElement 
    @param  pPoint 
*/
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
MOS_ActionContext::MOS_ActionContext( MOS_SelectedElement& selectedElement, MT_Vector2D* pPoint )
    : selectedElement_  ( selectedElement)
    , pPoint_           ( pPoint )
{
}


// -----------------------------------------------------------------------------
// Name: MOS_ActionContext destructor
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
MOS_ActionContext::~MOS_ActionContext()
{
}
