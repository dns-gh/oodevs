// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-06 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamLocation.inl $
// $Author: Ape $
// $Modtime: 26/08/04 10:02 $
// $Revision: 4 $
// $Workfile: ParamLocation.inl $
//
// *****************************************************************************

#include "GLTool.h"


// -----------------------------------------------------------------------------
// Name: ParamLocation::Draw
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
inline
void ParamLocation::Draw()
{
    GLTool::Draw( *this );
}


// -----------------------------------------------------------------------------
// Name: ParamLocation::Clear
// Created: APE 2004-06-04
// -----------------------------------------------------------------------------
inline
void ParamLocation::Clear()
{
    pShapeLabel_->setText( tr( "---" ) );
    pointList_.clear();
}


// -----------------------------------------------------------------------------
// Name: ParamLocation::GetPointList
/** @return 
*/
// Created: APE 2004-05-25
// -----------------------------------------------------------------------------
inline
const T_PointVector& ParamLocation::GetPointList()
{
    return pointList_;
}


// -----------------------------------------------------------------------------
// Name: ParamLocation::GetType
/** @return 
*/
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
inline
ASN1T_EnumTypeLocalisation ParamLocation::GetType()
{
    return nType_;
}
