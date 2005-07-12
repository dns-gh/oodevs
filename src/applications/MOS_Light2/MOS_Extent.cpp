//*****************************************************************************
// Name   : MOS_Extent.cpp
// Created: FBD 02-04-19
//*****************************************************************************

#include "MOS_Pch.h"
#include "MOS_Extent.h"

#ifndef  MOS_USE_INLINE
#   include "MOS_Extent.inl"
#endif  // _DEBUG


//-----------------------------------------------------------------------------
//  Name  :  MOS_Extent constructor
// Created: FBD 02-04-19
//-----------------------------------------------------------------------------
MOS_Extent::MOS_Extent()
    : rMeterPerPixel_( 0.0 )
{
}


//-----------------------------------------------------------------------------
//  Name  :  MOS_Extent destructor
// Created: FBD 02-04-19
//-----------------------------------------------------------------------------
MOS_Extent::~MOS_Extent()
{
}


//-----------------------------------------------------------------------------
//  Name  :  MOS_Extent::Read
// Created: FBD 02-09-05
//-----------------------------------------------------------------------------
void MOS_Extent::Read( MT_InputArchive_ABC& archive )
{
    rectDMS_.Read( archive );
    rect_   .Read( archive );

    std::string strDummy;
    archive.ReadField( "strDMSTop_",      strDummy );
    archive.ReadField( "strDMSBottom_",   strDummy );
    archive.ReadField( "strDMSLeft_",     strDummy );
    archive.ReadField( "strDMSRight_",    strDummy );
    archive.ReadField( "strWidthPixel_",  strDummy );
    archive.ReadField( "strHeightPixel_", strDummy );
    archive.ReadField( "rMeterPerPixel_", rMeterPerPixel_ );
}

