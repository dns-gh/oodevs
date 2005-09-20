// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamObstacle.inl $
// $Author: Ape $
// $Modtime: 25/05/04 15:39 $
// $Revision: 2 $
// $Workfile: MOS_ParamObstacle.inl $
//
// *****************************************************************************

#include "MOS_GLTool.h"
#include "MOS_ParamLocation.h"


// -----------------------------------------------------------------------------
// Name: MOS_ParamObstacle::Draw
// Created: APE 2004-05-25
// -----------------------------------------------------------------------------
inline
void MOS_ParamObstacle::Draw()
{
    MOS_GLTool::Draw( *this );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamObstacle::GetPointList
/** @return 
*/
// Created: APE 2004-05-25
// -----------------------------------------------------------------------------
inline
const T_PointVector& MOS_ParamObstacle::GetPointList()
{
    return pLocation_->GetPointList();
}
