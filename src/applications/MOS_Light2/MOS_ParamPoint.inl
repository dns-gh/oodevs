// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamPoint.inl $
// $Author: Ape $
// $Modtime: 27/08/04 17:14 $
// $Revision: 3 $
// $Workfile: MOS_ParamPoint.inl $
//
// *****************************************************************************


#include "MOS_GLTool.h"


// -----------------------------------------------------------------------------
// Name: MOS_ParamPoint::Draw
// Created: APE 2004-06-03
// -----------------------------------------------------------------------------
inline
void MOS_ParamPoint::Draw()
{
    MOS_GLTool::Draw( *this );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamPoint::GetPoint
/** @return 
*/
// Created: APE 2004-08-27
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MOS_ParamPoint::GetPoint()
{
    return point_;
}
