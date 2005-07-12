// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-23 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_GLDLayer.inl $
// $Author: Ape $
// $Modtime: 2/07/04 14:51 $
// $Revision: 1 $
// $Workfile: MT_GLDLayer.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MT_GLDLayer::GetName
/** @return 
*/
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
MT_INLINE
const QString& MT_GLDLayer::GetName() const
{
    return strName_;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDLayer::GetOpacity
/** @return 
*/
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
MT_INLINE
float MT_GLDLayer::GetOpacity() const
{
    return rOpacity_;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDLayer::IsVisible
/** @return 
*/
// Created: APE 2004-07-02
// -----------------------------------------------------------------------------
MT_INLINE
bool MT_GLDLayer::IsVisible() const
{
    return bVisible_;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDLayer::SetName
/** @param  strName 
*/
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
MT_INLINE
void MT_GLDLayer::SetName( const QString& strName )
{
    strName_ = strName;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDLayer::SetOpacity
/** @param  rOpacity 
*/
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
MT_INLINE
void MT_GLDLayer::SetOpacity( float rOpacity )
{
    rOpacity_ = rOpacity;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDLayer::SetVisible
/** @param  bVisible 
*/
// Created: APE 2004-07-02
// -----------------------------------------------------------------------------
MT_INLINE
void MT_GLDLayer::SetVisible( bool bVisible )
{
    bVisible_ = bVisible;
}
