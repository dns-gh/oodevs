// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-07-07 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_GLDShapeGroup.inl $
// $Author: Ape $
// $Modtime: 8/07/04 16:11 $
// $Revision: 1 $
// $Workfile: MT_GLDShapeGroup.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeGroup::GetShapes
/** @return 
*/
// Created: APE 2004-07-08
// -----------------------------------------------------------------------------
MT_INLINE
const MT_GLDShape_ABC::T_ShapeVector MT_GLDShapeGroup::GetShapes() const
{
    return shapes_;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeGroup::ClearShapes
// Created: APE 2004-07-08
// -----------------------------------------------------------------------------
MT_INLINE
void MT_GLDShapeGroup::ClearShapes()
{
    shapes_.clear();
}
