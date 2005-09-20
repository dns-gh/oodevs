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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamLocation.inl $
// $Author: Ape $
// $Modtime: 26/08/04 10:02 $
// $Revision: 4 $
// $Workfile: MOS_ParamLocation.inl $
//
// *****************************************************************************

#include "MOS_GLTool.h"


// -----------------------------------------------------------------------------
// Name: MOS_ParamLocation::Draw
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
inline
void MOS_ParamLocation::Draw()
{
    MOS_GLTool::Draw( *this );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamLocation::Clear
// Created: APE 2004-06-04
// -----------------------------------------------------------------------------
inline
void MOS_ParamLocation::Clear()
{
    pShapeLabel_->setText( tr( "---" ) );
    pointList_.clear();
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamLocation::GetPointList
/** @return 
*/
// Created: APE 2004-05-25
// -----------------------------------------------------------------------------
inline
const T_PointVector& MOS_ParamLocation::GetPointList()
{
    return pointList_;
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamLocation::GetType
/** @return 
*/
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
inline
ASN1T_EnumTypeLocalisation MOS_ParamLocation::GetType()
{
    return nType_;
}
