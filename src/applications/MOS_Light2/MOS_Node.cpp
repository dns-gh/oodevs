//*****************************************************************************
// Name   : MOS_Node.cpp
// Created: FBD 02-09-19
//*****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_Node.h"

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


//-----------------------------------------------------------------------------
//  Name  :  MOS_Node::Read
// Created: FBD 02-09-05
//-----------------------------------------------------------------------------
void MOS_Node::Read( MT_InputArchive_ABC& archive )
{
    archive.ReadField( "X", vPos_.rX_ );
    archive.ReadField( "Y", vPos_.rY_ );

    uint nTempIdx;
    archive.ReadField( "nTempIdx_", nTempIdx );
    uint nPassability;
    archive.ReadField( "nPassability", nPassability );
}
