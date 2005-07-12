//*****************************************************************************
// Name   : MOS_Extent.cpp
// Created: FBD 02-04-19
//*****************************************************************************

#include "MOS_Pch.h"
#include "MOS_Extent.h"

//-----------------------------------------------------------------------------
//  Name  :  MOS_Extent constructor
// Created: FBD 02-04-19
//-----------------------------------------------------------------------------
MOS_Extent::MOS_Extent()
: rMeterPerPixel_( 0. )
{
}

//-----------------------------------------------------------------------------
//  Name  :  MOS_Extent destructor
// Created: FBD 02-04-19
//-----------------------------------------------------------------------------
MOS_Extent::~MOS_Extent()
{
}

 
//=============================================================================
// IN/OUT
//=============================================================================

//-----------------------------------------------------------------------------
//  Name  :  MOS_Extent::Read
// Created: FBD 02-09-05
//-----------------------------------------------------------------------------
int MOS_Extent::Read( MT_InputArchive_ABC& archive )
{
    rectDMS_.Read( archive );
    rect_   .Read( archive );
    archive.ReadField( "strDMSTop_",      strDMSTop_ );
    archive.ReadField( "strDMSBottom_",   strDMSBottom_ );
    archive.ReadField( "strDMSLeft_",     strDMSLeft_ );
    archive.ReadField( "strDMSRight_",    strDMSRight_ );
    archive.ReadField( "strWidthPixel_",  strWidthPixel_ );
    archive.ReadField( "strHeightPixel_", strHeightPixel_ );
    archive.ReadField( "rMeterPerPixel_", rMeterPerPixel_ );

    return 0;
}

