//*****************************************************************************
// Name   : MOS_Node.cpp
// Created: FBD 02-09-19
//*****************************************************************************

#include "MOS_pch.h"
#include "MOS_Node.h"

#include "MOS_Node.h"
#include "MOS_World.h"
#include "MOS_Patch.h"
#include "MOS_ObjectManager.h"

//-----------------------------------------------------------------------------
//  Name  :  MOS_Node constructor
// Created: FBD 02-09-19
//-----------------------------------------------------------------------------
MOS_Node::MOS_Node()
      : vPos_()
{
}

//-----------------------------------------------------------------------------
//  Name  :  MOS_Node destructor
// Created: FBD 02-09-19
//-----------------------------------------------------------------------------
MOS_Node::~MOS_Node()
{
}


//=============================================================================
// GENERATION
//=============================================================================

//-----------------------------------------------------------------------------
//  Name  :  MOS_Node::Read
// Created: FBD 02-09-05
//-----------------------------------------------------------------------------
int MOS_Node::Read( MT_InputArchive_ABC& archive )
{
    vPos_.Read( archive );
    uint nTempIdx;
    archive.ReadField( "nTempIdx_", nTempIdx );
    uint nPassability;
    archive.ReadField( "nPassability", nPassability );
    return 0;
}

//-----------------------------------------------------------------------------
// Name: MOS_Node constructor
// Created: FBD 03-05-23
//-----------------------------------------------------------------------------
MOS_Node::MOS_Node( const MOS_Node& rhs )
:   vPos_( rhs.vPos_ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_Node::=( const 
// Created: FBD 03-05-23
//-----------------------------------------------------------------------------
MOS_Node& MOS_Node::operator = ( const MOS_Node& rhs )
{
    vPos_ = rhs.vPos_;
    return *this;
}

