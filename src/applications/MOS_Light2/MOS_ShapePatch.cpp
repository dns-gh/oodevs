//*****************************************************************************
// Name   : MOS_ShapePatch.cpp
// Created: FBD 02-07-01
//*****************************************************************************

#include "MOS_pch.h"
#include "MOS_ShapePatch.h"

#include "MOS_App.h"
#include "MOS_ObjectManager.h"
#include "MOS_Patch.h"
#include "MOS_Object.h"

#ifndef  MOS_USE_INLINE
#   include "MOS_ShapePatch.inl"
#endif


//-----------------------------------------------------------------------------
//  Name  :  MOS_ShapePatch constructor
// Created: FBD 02-11-18
//-----------------------------------------------------------------------------
MOS_ShapePatch::MOS_ShapePatch( MOS_Object& object, MOS_Patch& patch )
    : object_              ( object )
    , patch_               ( patch )
    , nodePtrVector_       ()
    , linePtrVector_       ()
    , triPtrVector_        ()
    , nGL_                 ( -1 )
{
}


//-----------------------------------------------------------------------------
//  Name  :  MOS_ShapePatch destructor
// Created: FBD 02-07-02
//-----------------------------------------------------------------------------
MOS_ShapePatch::~MOS_ShapePatch()
{
}


//-----------------------------------------------------------------------------
//  Name  :  MOS_ShapePatch::Read
// Created: FBD 02-09-05
//-----------------------------------------------------------------------------
void MOS_ShapePatch::Read( MT_InputArchive_ABC& archive )
{
    MOS_ObjectManager& objectManager = MOS_App::GetApp().GetObjectManager();

    // Bouding box
    MT_Float rValue;
    archive.ReadField( "rXMin_", rValue );
    archive.ReadField( "rXMax_", rValue );
    archive.ReadField( "rYMin_", rValue );
    archive.ReadField( "rYMax_", rValue );

    // Nodes
    uint nNodes = 0;
    archive.ReadField( "nNodes", nNodes );
    for( uint i = 0 ; i < nNodes ; ++i )
    {
        uint nNode = 0;
        archive.ReadField( "nNode", nNode );

        // Only read the nodes if we are not going to read lines or triangles
        // afterwards.
        if ( GetObjectShape().IsPoint() )
        {
            MOS_Node* pNode = objectManager.GetNodeFromIdx( nNode );
            assert( pNode );
            nodePtrVector_.push_back( pNode );
        }
    }

    // Lines
    uint nLines = 0;
    archive.ReadField( "nLines", nLines );
    for( i = 0 ; i < nLines ; ++i )
    {
        uint nLine = 0;
        archive.ReadField( "nLine", nLine );
        MOS_Line* pLine = objectManager.GetLineFromIdx( nLine );
        assert( pLine );
        linePtrVector_.push_back( pLine );
    }

    // Triangles
    uint nTris = 0;
    archive.ReadField( "nTris", nTris );
    for( i = 0 ; i < nTris ; ++i )
    {
        uint nTri = 0;
        archive.ReadField( "nTri", nTri );
        MOS_Tri* pTri = objectManager.GetTriFromIdx( nTri );
        assert( pTri );
        triPtrVector_.push_back( pTri );

    }
}
