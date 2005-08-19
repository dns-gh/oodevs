// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-07-07 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_GLDShapeGroup.cpp $
// $Author: Ape $
// $Modtime: 21/07/04 11:06 $
// $Revision: 1 $
// $Workfile: MT_GLDShapeGroup.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MT_GLDShapeGroup.h"

#ifndef MT_USE_INLINE
#   include "MT_GLDShapeGroup.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeGroup constructor
// Created: APE 2004-07-07
// -----------------------------------------------------------------------------
MT_GLDShapeGroup::MT_GLDShapeGroup( MT_GLDrawer& glDrawer, const MT_GLDShape_ABC::T_ShapeVector& shapes )
    : MT_GLDShape_ABC   ( glDrawer )
    , shapes_           ( shapes )
{
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeGroup destructor
// Created: APE 2004-07-07
// -----------------------------------------------------------------------------
MT_GLDShapeGroup::~MT_GLDShapeGroup()
{
    MT_DELETEOWNED( shapes_ );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeGroup::Draw
// Created: APE 2004-07-07
// -----------------------------------------------------------------------------
void MT_GLDShapeGroup::Draw()
{
    for( MT_GLDShape_ABC::IT_ShapeVector it = shapes_.begin(); it != shapes_.end(); ++it )
        (*it)->Draw();
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeGroup::IsAtPos
/** @param  vPos 
    @return 
*/
// Created: APE 2004-07-07
// -----------------------------------------------------------------------------
bool MT_GLDShapeGroup::IsAtPos( const MT_Vector2D& vPos )
{
    for( MT_GLDShape_ABC::IT_ShapeVector it = shapes_.begin(); it != shapes_.end(); ++it )
        if( (*it)->IsAtPos( vPos ) )
            return true;
    return false;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeGroup::Translate
/** @param  vPos 
*/
// Created: APE 2004-07-07
// -----------------------------------------------------------------------------
void MT_GLDShapeGroup::Translate( const MT_Vector2D& vPos )
{
    for( MT_GLDShape_ABC::IT_ShapeVector it = shapes_.begin(); it != shapes_.end(); ++it )
        (*it)->Translate( vPos );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeGroup::GetPointAt
/** @param  vPos 
    @return 
*/
// Created: APE 2004-07-07
// -----------------------------------------------------------------------------
MT_Vector2D* MT_GLDShapeGroup::GetPointAt( const MT_Vector2D& /*vPos*/ )
{
    return 0;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeGroup::GetProperties
/** @return 
*/
// Created: APE 2004-07-07
// -----------------------------------------------------------------------------
const MT_GLDShapeProperties& MT_GLDShapeGroup::GetProperties() const
{
    assert( ! shapes_.empty() );
    properties_ = shapes_.front()->GetProperties();
    if( shapes_.size() > 1 )
    {
        for( MT_GLDShape_ABC::CIT_ShapeVector it = shapes_.begin(); it != shapes_.end(); ++it )
            properties_ = properties_.IntersectWith( (*it)->GetProperties() );
    }
    return properties_;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeGroup::SetProperties
/** @param  properties 
*/
// Created: APE 2004-07-07
// -----------------------------------------------------------------------------
void MT_GLDShapeGroup::SetProperties( const MT_GLDShapeProperties& properties )
{
    for( MT_GLDShape_ABC::CIT_ShapeVector it = shapes_.begin(); it != shapes_.end(); ++it )
       (*it)->SetProperties( properties );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeGroup::SetState
/** @param  nState 
*/
// Created: APE 2004-07-07
// -----------------------------------------------------------------------------
void MT_GLDShapeGroup::SetState( E_State nState )
{
    nState_ = nState;
    for( MT_GLDShape_ABC::IT_ShapeVector it = shapes_.begin(); it != shapes_.end(); ++it )
        (*it)->SetState( nState );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeGroup::Read
/** @param  archive 
*/
// Created: APE 2004-07-07
// -----------------------------------------------------------------------------
void MT_GLDShapeGroup::Read( MT_XXmlInputArchive& archive )
{
    archive.Section( "ShapeGroup" );
    archive.BeginList( "Shapes" );
    while( archive.NextListElement() )
    {
        std::auto_ptr<MT_GLDShape_ABC> spShape( MT_GLDShape_ABC::ReadNewShape( archive, glDrawer_ ) );
        spShape->Read( archive );
        shapes_.push_back( spShape.release() );
    }
    archive.EndList();
    archive.EndSection();
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeGroup::Write
/** @param  archive 
*/
// Created: APE 2004-07-07
// -----------------------------------------------------------------------------
void MT_GLDShapeGroup::Write( MT_XXmlOutputArchive& archive ) const
{
    archive.Section( "ShapeGroup" );
    archive.BeginList( "Shapes", shapes_.size() );
    for( MT_GLDShape_ABC::CIT_ShapeVector it = shapes_.begin(); it != shapes_.end(); ++it )
    {
        (*it)->Write( archive );
    }
    archive.EndList();
    archive.EndSection();
}
