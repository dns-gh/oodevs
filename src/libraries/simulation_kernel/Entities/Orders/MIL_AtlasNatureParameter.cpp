// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AtlasNatureParameter.h"

#include "MIL_ParameterType_NatureAtlas.h"
#include "Network/NET_ASN_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_AtlasNatureParameter constructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_AtlasNatureParameter::MIL_AtlasNatureParameter( const ASN1T_AtlasNature& asn )
{
    NET_ASN_Tools::CopyNatureAtlas( asn, nature_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_AtlasNatureParameter constructor
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
MIL_AtlasNatureParameter::MIL_AtlasNatureParameter( int nature )
: nature_( nature )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AtlasNatureParameter destructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_AtlasNatureParameter::~MIL_AtlasNatureParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AtlasNatureParameter::IsOfType
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AtlasNatureParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_NatureAtlas*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_AtlasNatureParameter::ToNatureAtlas
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AtlasNatureParameter::ToNatureAtlas( ASN1T_AtlasNature& asn ) const
{
    return NET_ASN_Tools::CopyNatureAtlas( nature_, asn );
}

// -----------------------------------------------------------------------------
// Name: MIL_AtlasNatureParameter::ToNatureAtlas
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AtlasNatureParameter::ToNatureAtlas( int& atlas ) const
{
    atlas = nature_;
    return true;
}
