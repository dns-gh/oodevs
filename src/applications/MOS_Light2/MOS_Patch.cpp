//*****************************************************************************
//
// $Created: FBD 02-11-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Patch.cpp $
// $Author: Ape $
// $Modtime: 15/09/04 17:47 $
// $Revision: 3 $
// $Workfile: MOS_Patch.cpp $
//
//*****************************************************************************

#include "MOS_Pch.h"
#include "MOS_Patch.h"

#ifndef MOS_USE_INLINE
#	include "MOS_Patch.inl"
#endif


//-----------------------------------------------------------------------------
// Name: MOS_Patch constructor
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
MOS_Patch::MOS_Patch()
    : rect_()
    , nGl_( -1 )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_Patch destructor
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
MOS_Patch::~MOS_Patch()
{
}


//-----------------------------------------------------------------------------
//  Name  :  MOS_Patch::Read
// Created: FBD 02-09-05
//-----------------------------------------------------------------------------
void MOS_Patch::Read( MT_InputArchive_ABC& archive )
{
    rect_.Read( archive );
    uint nUnused;
    archive.ReadField( "nIdxPatchX_", nUnused );
    archive.ReadField( "nIdxPatchY_", nUnused );
}
