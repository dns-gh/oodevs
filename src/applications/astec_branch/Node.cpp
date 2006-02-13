//*****************************************************************************
// Name   : Node.cpp
// Created: FBD 02-09-19
//*****************************************************************************

#include "astec_pch.h"
#include "Node.h"

//-----------------------------------------------------------------------------
//  Name  :  Node constructor
// Created: FBD 02-09-19
//-----------------------------------------------------------------------------
Node::Node()
    : vPos_()
{
}


//-----------------------------------------------------------------------------
//  Name  :  Node destructor
// Created: FBD 02-09-19
//-----------------------------------------------------------------------------
Node::~Node()
{
}


//-----------------------------------------------------------------------------
//  Name  :  Node::Read
// Created: FBD 02-09-05
//-----------------------------------------------------------------------------
void Node::Read( MT_InputArchive_ABC& archive )
{
    archive.ReadField( "X", vPos_.rX_ );
    archive.ReadField( "Y", vPos_.rY_ );

    uint nTempIdx;
    archive.ReadField( "nTempIdx_", nTempIdx );
    uint nPassability;
    archive.ReadField( "nPassability", nPassability );
}
