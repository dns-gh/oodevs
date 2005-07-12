//*****************************************************************************
// Name   : MOS_Tri.cpp
// Created: FBD 02-07-01
//*****************************************************************************

#include "MOS_pch.h"
#include "MOS_Tri.h"

#include "MOS_App.h"
#include "MOS_ObjectManager.h"

#ifndef  MOS_USE_INLINE
#   include "MOS_Tri.inl"
#endif


//-----------------------------------------------------------------------------
//  Name  :  MOS_Tri constructor
// Created: FBD 02-07-01
//-----------------------------------------------------------------------------
MOS_Tri::MOS_Tri()
:   pNode1_         ( 0 )
,   pNode2_         ( 0 )
,   pNode3_         ( 0 )
,   nFrame_         ( 0 )
{
}

//-----------------------------------------------------------------------------
//  Name  :  MOS_Tri::~MOS_Tri
// Created: FBD 02-07-01
//-----------------------------------------------------------------------------
MOS_Tri::~MOS_Tri()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_Tri::Read
// Created: FBD 02-11-15
//-----------------------------------------------------------------------------
void MOS_Tri::Read( MT_InputArchive_ABC& archive )
{
    uint nTempIdx;
    archive.ReadField( "nTempIdx_", nTempIdx );
    
    MOS_ObjectManager& objectMgr = MOS_App::GetApp().GetObjectManager();

    uint nNode1 = 0;
    archive.ReadField( "nNode1", nNode1 );
    pNode1_ = objectMgr.GetNodeFromIdx( nNode1 );
    assert( pNode1_ );

    uint nNode2 = 0;
    archive.ReadField( "nNode2", nNode2 );
    pNode2_ = objectMgr.GetNodeFromIdx( nNode2 );
    assert( pNode2_ );

    uint nNode3 = 0;
    archive.ReadField( "nNode3", nNode3 );
    pNode3_ = objectMgr.GetNodeFromIdx( nNode3 );
    assert( pNode3_ );

    uint nPassability;
    archive.ReadField( "nPassability", nPassability );
}


