// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
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
MOS_INLINE
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
MOS_INLINE
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
MOS_INLINE
T_PointVector& MOS_ShapeEditorMapEventFilter::GetPointList()
{
    return pointList_;
}
