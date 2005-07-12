//*****************************************************************************
// Name   : MOS_Tri.cpp
// Created: FBD 02-07-01
//*****************************************************************************

#include "MOS_pch.h"
#include "MOS_Tri.h"

#include "MOS_Node.h"
#include "MOS_World.h"
#include "MOS_App.h"
#include "MOS_ObjectManager.h"

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
// Name: MOS_Tri constructor
// Created: FBD 03-05-23
//-----------------------------------------------------------------------------
MOS_Tri::MOS_Tri( const MOS_Tri& rhs )
:   pNode1_( rhs.pNode1_ )
,   pNode2_( rhs.pNode2_ )
,   pNode3_( rhs.pNode3_ )
,   nFrame_( rhs.nFrame_ )
,   rSize_( rhs.rSize_ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_Tri::=( const 
// Created: FBD 03-05-23
//-----------------------------------------------------------------------------
MOS_Tri& MOS_Tri::operator = ( const MOS_Tri& rhs )
{
    pNode1_ = rhs.pNode1_;
    pNode2_ = rhs.pNode2_;
    pNode3_ = rhs.pNode3_;
    nFrame_ = rhs.nFrame_;
    rSize_ = rhs.rSize_;
    return *this;
}



//-----------------------------------------------------------------------------
// Name: MOS_Tri::Read
// Created: FBD 02-11-15
//-----------------------------------------------------------------------------
int MOS_Tri::Read( MT_InputArchive_ABC& archive )
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

    rSize_ = pNode1_->GetPos().SquareDistance( pNode2_->GetPos() );
    rSize_ += pNode1_->GetPos().SquareDistance( pNode3_->GetPos() );
    rSize_ += pNode2_->GetPos().SquareDistance( pNode3_->GetPos() );
    rSize_ /= 3.;

    return 0;
}


