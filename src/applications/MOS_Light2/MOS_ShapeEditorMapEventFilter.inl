// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-14 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ShapeEditorMapEventFilter.inl $
// $Author: Ape $
// $Modtime: 5/10/04 15:58 $
// $Revision: 2 $
// $Workfile: MOS_ShapeEditorMapEventFilter.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MOS_ShapeEditorMapEventFilter::PrepareNewLine
/** @param  nType 
*/
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
inline
void MOS_ShapeEditorMapEventFilter::PrepareNewLine( E_LineType nType )
{
    nLineType_ = nType;
    pointList_.clear();
}


// -----------------------------------------------------------------------------
// Name: MOS_ShapeEditorMapEventFilter::GetObject
/** @return 
*/
// Created: APE 2004-10-05
// -----------------------------------------------------------------------------
inline
QObject* MOS_ShapeEditorMapEventFilter::GetObject()
{
    return this;
}


// -----------------------------------------------------------------------------
// Name: MOS_ShapeEditorMapEventFilter::GetPointList
/** @return 
*/
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
inline
T_PointVector& MOS_ShapeEditorMapEventFilter::GetPointList()
{
    return pointList_;
}
