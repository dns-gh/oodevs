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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamObstacle.inl $
// $Author: Ape $
// $Modtime: 25/05/04 15:39 $
// $Revision: 2 $
// $Workfile: ParamObstacle.inl $
//
// *****************************************************************************

#include "GLTool.h"
#include "ParamLocation.h"


// -----------------------------------------------------------------------------
// Name: ParamObstacle::Draw
// Created: APE 2004-05-25
// -----------------------------------------------------------------------------
inline
void ParamObstacle::Draw()
{
    GLTool::Draw( *this );
}


// -----------------------------------------------------------------------------
// Name: ParamObstacle::GetPointList
/** @return 
*/
// Created: APE 2004-05-25
// -----------------------------------------------------------------------------
inline
const T_PointVector& ParamObstacle::GetPointList()
{
    return pLocation_->GetPointList();
}
