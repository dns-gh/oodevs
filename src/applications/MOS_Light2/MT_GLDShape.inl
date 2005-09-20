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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_GLDShape.inl $
// $Author: Ape $
// $Modtime: 5/07/04 15:54 $
// $Revision: 1 $
// $Workfile: MT_GLDShape.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MT_GLDShape::GetProperties
/** @return 
*/
// Created: APE 2004-07-02
// -----------------------------------------------------------------------------
inline
const MT_GLDShapeProperties& MT_GLDShape::GetProperties() const
{
    return properties_;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShape::SetProperties
/** @param  properties 
*/
// Created: APE 2004-07-02
// -----------------------------------------------------------------------------
inline
void MT_GLDShape::SetProperties( const MT_GLDShapeProperties& properties )
{
    properties_ = properties;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShape::Translate
/** @param  vPos 
*/
// Created: APE 2004-07-01
// -----------------------------------------------------------------------------
inline
void MT_GLDShape::Translate( const MT_Vector2D& vPos )
{
    for( IT_PointVector it = points_.begin(); it != points_.end(); ++it )
        (*it) += vPos;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShape::SetColor
/** @param  color 
*/
// Created: APE 2004-07-02
// -----------------------------------------------------------------------------
inline
void MT_GLDShape::SetColor( QRgb color ) const
{
    glColor4ub( qRed(color), qGreen(color), qBlue(color), qAlpha(color) );
}
