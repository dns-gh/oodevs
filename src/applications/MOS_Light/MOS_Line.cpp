//*****************************************************************************
//
// $Created: FBD 03-04-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Line.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 5 $
// $Workfile: MOS_Line.cpp $
//
//*****************************************************************************

#include "MOS_Pch.h"
#include "MOS_Line.h"

#ifndef MOS_USE_INLINE
#	include "MOS_Line.inl"
#endif

#include "MOS_ObjectManager.h"
#include "MOS_App.h"
//-----------------------------------------------------------------------------
// Name: MOS_Line constructor
// Created: FBD 03-04-08
//-----------------------------------------------------------------------------
MOS_Line::MOS_Line()
:   pNode1_ ( 0 )
,   pNode2_ ( 0 )
//,   nIdx_   ( 0 )
,   nFrame_ ( 0 )
{
}



//-----------------------------------------------------------------------------
// Name: MOS_Line constructor
// Created: FBD 03-04-08
//-----------------------------------------------------------------------------
MOS_Line::MOS_Line( MOS_Node& node1, MOS_Node& node2 )
:   pNode1_ ( &node1 )
,   pNode2_ ( &node2 )
//,   nIdx_   ( 0 )
,   nFrame_ ( 0 )
{
}



//-----------------------------------------------------------------------------
// Name: MOS_Line destructor
// Created: FBD 03-04-08
//-----------------------------------------------------------------------------
MOS_Line::~MOS_Line()
{
    
}

//-----------------------------------------------------------------------------
// Name: MOS_Line::Read
// Created: FBD 02-11-15
//-----------------------------------------------------------------------------
int MOS_Line::Read( MT_InputArchive_ABC& archive )
{
    MOS_ObjectManager& objectMgr = MOS_App::GetApp().GetObjectManager();

    uint nIdx;
    archive.ReadField( "nIdx_", nIdx );
    
    uint nNode1 = 0;
    archive.ReadField( "nNode1", nNode1 );
    pNode1_ = objectMgr.GetNodeFromIdx( nNode1 );
    uint nNode2 = 0;
    archive.ReadField( "nNode2", nNode2 );
    pNode2_ = objectMgr.GetNodeFromIdx( nNode2 );

    uint nPassability;
    archive.ReadField( "nPassability", nPassability );
    
    return 0;
}


//-----------------------------------------------------------------------------
// Name: MOS_Line constructor
// Created: FBD 03-05-23
//-----------------------------------------------------------------------------
MOS_Line::MOS_Line( const MOS_Line& rhs )
:   pNode1_( rhs.pNode1_ )
,   pNode2_( rhs.pNode2_ )
,   nFrame_( rhs.nFrame_ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_Line::=( const 
// Created: FBD 03-05-23
//-----------------------------------------------------------------------------
MOS_Line& MOS_Line::operator = ( const MOS_Line& rhs )
{
    pNode1_ = rhs.pNode1_;
    pNode2_ = rhs.pNode2_;
    nFrame_ = rhs.nFrame_;
    return *this;
}

