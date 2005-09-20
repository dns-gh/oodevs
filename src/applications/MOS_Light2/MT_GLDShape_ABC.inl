// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-24 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_GLDShape_ABC.inl $
// $Author: Ape $
// $Modtime: 8/07/04 16:09 $
// $Revision: 1 $
// $Workfile: MT_GLDShape_ABC.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MT_GLDShape_ABC::SetState
/** @param  nState 
*/
// Created: APE 2004-07-02
// -----------------------------------------------------------------------------
inline
void MT_GLDShape_ABC::SetState( MT_GLDShape_ABC::E_State nState )
{
    nState_ = nState;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShape_ABC::GetShapes
/** @return 
*/
// Created: APE 2004-07-08
// -----------------------------------------------------------------------------
inline
const MT_GLDShape_ABC::T_ShapeVector MT_GLDShape_ABC::GetShapes() const
{
    return T_ShapeVector();
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShape_ABC::ClearShapes
// Created: APE 2004-07-08
// -----------------------------------------------------------------------------
inline
void MT_GLDShape_ABC::ClearShapes()
{
}
