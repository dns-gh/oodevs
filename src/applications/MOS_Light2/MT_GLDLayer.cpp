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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_GLDLayer.cpp $
// $Author: Ape $
// $Modtime: 8/07/04 16:11 $
// $Revision: 1 $
// $Workfile: MT_GLDLayer.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MT_GLDLayer.h"

#include "MT_GLDShape_ABC.h"
#include "MT_GLDShape.h"
#include "MT_GLDShapeGroup.h"

#include <memory.h>

#ifndef MT_USE_INLINE
#   include "MT_GLDLayer.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MT_GLDLayer constructor
/** @param  strName 
*/
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
MT_GLDLayer::MT_GLDLayer( const QString& strName, MT_GLDrawer& glDrawer )
    : glDrawer_ ( glDrawer )
    , strName_  ( strName )
    , rOpacity_ ( 1.0f )
    , bVisible_ ( true )
{
}


// -----------------------------------------------------------------------------
// Name: MT_GLDLayer destructor
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
MT_GLDLayer::~MT_GLDLayer()
{
    MT_DELETEOWNED( shapes_ );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDLayer::Draw
// Created: APE 2004-06-24
// -----------------------------------------------------------------------------
void MT_GLDLayer::Draw()
{
    if( ! bVisible_ )
        return;

    for( MT_GLDShape_ABC::IT_ShapeVector it = shapes_.begin(); it != shapes_.end(); ++it )
        (*it)->Draw();
}


// -----------------------------------------------------------------------------
// Name: MT_GLDLayer::AddShape
/** @param  shape 
*/
// Created: APE 2004-06-24
// -----------------------------------------------------------------------------
void MT_GLDLayer::AddShape( MT_GLDShape_ABC& shape )
{
    shapes_.push_back( &shape );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDLayer::AddShapes
/** @param  shapes 
*/
// Created: APE 2004-06-30
// -----------------------------------------------------------------------------
void MT_GLDLayer::AddShapes( const MT_GLDShape_ABC::T_ShapeVector& shapes )
{
    for( MT_GLDShape_ABC::CIT_ShapeVector it = shapes.begin(); it != shapes.end(); ++it )
        shapes_.push_back( *it );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDLayer::RemoveShapes
/** @param  shapes 
*/
// Created: APE 2004-06-30
// -----------------------------------------------------------------------------
void MT_GLDLayer::RemoveShapes( const MT_GLDShape_ABC::T_ShapeVector& shapes )
{
    for( MT_GLDShape_ABC::CIT_ShapeVector it = shapes.begin(); it != shapes.end(); ++it )
    {
        MT_GLDShape_ABC::IT_ShapeVector itFound = std::find( shapes_.begin(), shapes_.end(), *it );
        assert( itFound != shapes_.end() );
        shapes_.erase( itFound );
    }
}


// -----------------------------------------------------------------------------
// Name: MT_GLDLayer::DeleteShapes
/** @param  shapes 
*/
// Created: APE 2004-06-30
// -----------------------------------------------------------------------------
void MT_GLDLayer::DeleteShapes( const MT_GLDShape_ABC::T_ShapeVector& shapes )
{
    for( MT_GLDShape_ABC::CIT_ShapeVector it = shapes.begin(); it != shapes.end(); ++it )
    {
        MT_GLDShape_ABC::IT_ShapeVector itFound = std::find( shapes_.begin(), shapes_.end(), *it );
        assert( itFound != shapes_.end() );
        delete *itFound;
        shapes_.erase( itFound );
    }
}


// -----------------------------------------------------------------------------
// Name: MT_GLDLayer::GetShapeAtPos
/** @param  vPos 
    @return 
*/
// Created: APE 2004-06-29
// -----------------------------------------------------------------------------
MT_GLDShape_ABC* MT_GLDLayer::GetShapeAtPos( const MT_Vector2D& vPos )
{
    for( MT_GLDShape_ABC::IT_ShapeVector it = shapes_.begin(); it != shapes_.end(); ++it )
        if( (*it)->IsAtPos( vPos ) )
            return *it;

    return 0;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDLayer::Read
/** @param  archive 
    @return 
*/
// Created: APE 2004-07-06
// -----------------------------------------------------------------------------
void MT_GLDLayer::Read( MT_XXmlInputArchive& archive )
{
    std::string strName;
    archive.ReadAttribute( "Name", strName );
    strName_ = strName.c_str();
    archive.BeginList( "Shapes" );
    while( archive.NextListElement() )
    {
        std::auto_ptr<MT_GLDShape_ABC> spShape( MT_GLDShape_ABC::ReadNewShape( archive, glDrawer_ ) );
        spShape->Read( archive );
        shapes_.push_back( spShape.release() );
    }
    archive.EndList();
}


// -----------------------------------------------------------------------------
// Name: MT_GLDLayer::Write
/** @param  archive 
*/
// Created: APE 2004-07-06
// -----------------------------------------------------------------------------
void MT_GLDLayer::Write( MT_XXmlOutputArchive& archive ) const
{
    archive.WriteAttribute( "Name", strName_.ascii() );
    archive.BeginList( "Shapes", shapes_.size() );
    for( MT_GLDShape_ABC::CIT_ShapeVector it = shapes_.begin(); it != shapes_.end(); ++it )
        (*it)->Write( archive );
    archive.EndList();
}


// -----------------------------------------------------------------------------
// Name: MT_GLDLayer::Group
/** @param  shapes 
    @return 
*/
// Created: APE 2004-07-08
// -----------------------------------------------------------------------------
MT_GLDShape_ABC& MT_GLDLayer::Group( MT_GLDShape_ABC::T_ShapeVector& shapes )
{
    this->RemoveShapes( shapes );
    MT_GLDShapeGroup* pNewGroup = new MT_GLDShapeGroup( glDrawer_, shapes );
    shapes_.push_back( pNewGroup );
    return *pNewGroup;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDLayer::Disgroup
/** @param  shape 
    @return 
*/
// Created: APE 2004-07-08
// -----------------------------------------------------------------------------
MT_GLDShape_ABC::T_ShapeVector MT_GLDLayer::Disgroup( MT_GLDShape_ABC& shape )
{
    const MT_GLDShape_ABC::T_ShapeVector shapes = shape.GetShapes();
    this->AddShapes( shapes );
    shape.ClearShapes();

    MT_GLDShape_ABC::IT_ShapeVector itFound = std::find( shapes_.begin(), shapes_.end(), &shape );
    assert( itFound != shapes_.end() );
    delete *itFound;
    shapes_.erase( itFound );

    return shapes;
}
