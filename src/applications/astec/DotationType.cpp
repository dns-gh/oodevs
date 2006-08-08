//*****************************************************************************
// Created: JVT 02-08-30
//*****************************************************************************

#include "astec_pch.h"

#include "DotationType.h"

// -----------------------------------------------------------------------------
// Name: DotationType constructor
// Created: NLD 2004-09-10
// Modified: JVT 2004-09-27
// -----------------------------------------------------------------------------
DotationType::DotationType( const std::string& strCategory, const std::string& strName, InputArchive& archive )
    : strCategory_              ( strCategory )
    , strName_                  ( strName )
    , bCanBeUsedForIndirectFire_( false )
{
    archive.ReadField( "MosID", nID_ );
    if( archive.Section( "TirIndirect", InputArchive::eNothing ) )
    {
        bCanBeUsedForIndirectFire_ = true;
        archive.EndSection();
    }
}

// -----------------------------------------------------------------------------
// Name: DotationType destructor
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
DotationType::~DotationType()
{
}
