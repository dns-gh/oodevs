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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_GLDrawer.inl $
// $Author: Ape $
// $Modtime: 23/07/04 16:37 $
// $Revision: 1 $
// $Workfile: MT_GLDrawer.inl $
//
// *****************************************************************************

#include "MT_GLDShapeDialog.h"

// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::SetDefaults
/** @param  rFontSize 
    @param  lineColor 
    @param  fillColor 
*/
// Created: APE 2004-06-30
// -----------------------------------------------------------------------------
inline
void MT_GLDrawer::SetDefaults( float rMinFontSize, float rMaxFontSize, const MT_GLDShapeProperties& properties )
{
    assert( rMinFontSize > 0 && rMaxFontSize );
    defaultShapeProperties_ = properties;
    rMinFontSize_ = rMinFontSize;
    rMaxFontSize_ = rMaxFontSize;
    defaultShapeProperties_.rFontSize_ = (rMaxFontSize + rMinFontSize) / 2.0f;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::ScreenToGL
/** @param  vScreenPos 
    @return 
*/
// Created: APE 2004-06-24
// -----------------------------------------------------------------------------
inline
MT_Vector2D MT_GLDrawer::ScreenToGL( const MT_Vector2D& vScreenPos )
{
    return pScreenToGLFunc_( vScreenPos, glWidget_ );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::GetGLWidget
/** @return 
*/
// Created: APE 2004-06-24
// -----------------------------------------------------------------------------
inline
QGLWidget& MT_GLDrawer::GetGLWidget()
{
    return glWidget_;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::GetFont
/** @return 
*/
// Created: APE 2004-06-30
// -----------------------------------------------------------------------------
inline
MT_GLFont& MT_GLDrawer::GetFont()
{
    return *pGLFont_;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::GetMinFontSize
/** @return 
*/
// Created: APE 2004-07-01
// -----------------------------------------------------------------------------
inline
float MT_GLDrawer::GetMinFontSize()
{
    return rMinFontSize_;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::GetMaxFontSize
/** @return 
*/
// Created: APE 2004-07-01
// -----------------------------------------------------------------------------
inline
float MT_GLDrawer::GetMaxFontSize()
{
    return rMaxFontSize_;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::GetDefautShapeProperties
/** @return 
*/
// Created: APE 2004-07-02
// -----------------------------------------------------------------------------
inline
const MT_GLDShapeProperties& MT_GLDrawer::GetDefautShapeProperties() const
{
    return defaultShapeProperties_;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::SetSelection
/** @param  shape 
*/
// Created: APE 2004-07-02
// -----------------------------------------------------------------------------
inline
void MT_GLDrawer::SetSelection( MT_GLDShape_ABC& shape )
{
    for( MT_GLDShape_ABC::IT_ShapeVector it = selection_.begin(); it != selection_.end(); ++it )
        (*it)->SetState( MT_GLDShape_ABC::eNormal );
    selection_.clear();
    shape.SetState( MT_GLDShape_ABC::eSelected );
    selection_.push_back( &shape );
    pShapeDialog_->SetShapes( selection_ );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::SetSelection
/** @param  shapes 
*/
// Created: APE 2004-07-08
// -----------------------------------------------------------------------------
inline
void MT_GLDrawer::SetSelection( MT_GLDShape_ABC::T_ShapeVector& shapes )
{
    MT_GLDShape_ABC::IT_ShapeVector it;
    for( it = selection_.begin(); it != selection_.end(); ++it )
        (*it)->SetState( MT_GLDShape_ABC::eNormal );
    selection_.clear();
    selection_ = shapes;
    for( it = selection_.begin(); it != selection_.end(); ++it )
        (*it)->SetState( MT_GLDShape_ABC::eSelected );
    pShapeDialog_->SetShapes( selection_ );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::AddToSelection
/** @param  shape 
*/
// Created: APE 2004-07-02
// -----------------------------------------------------------------------------
inline
void MT_GLDrawer::AddToSelection( MT_GLDShape_ABC& shape )
{
    shape.SetState( MT_GLDShape_ABC::eSelected );
    selection_.push_back( &shape );
    pShapeDialog_->SetShapes( selection_ );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::RemoveFromSelection
/** @param  shape 
*/
// Created: APE 2004-07-02
// -----------------------------------------------------------------------------
inline
void MT_GLDrawer::RemoveFromSelection( MT_GLDShape_ABC& shape )
{
    MT_GLDShape_ABC::IT_ShapeVector it = std::find( selection_.begin(), selection_.end(), &shape );
    assert( it != selection_.end() );
    selection_.erase( it );
    shape.SetState( MT_GLDShape_ABC::eNormal );
    pShapeDialog_->SetShapes( selection_ );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::ClearSelection
// Created: APE 2004-07-02
// -----------------------------------------------------------------------------
inline
void MT_GLDrawer::ClearSelection()
{
    for( MT_GLDShape_ABC::IT_ShapeVector it = selection_.begin(); it != selection_.end(); ++it )
        (*it)->SetState( MT_GLDShape_ABC::eNormal );
    selection_.clear();
    pShapeDialog_->SetShapes( selection_ );
}
