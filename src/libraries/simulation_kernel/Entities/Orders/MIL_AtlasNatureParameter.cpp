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
#include "Network/NET_ASN_Tools.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_AtlasNatureParameter constructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_AtlasNatureParameter::MIL_AtlasNatureParameter( const Common::MsgAtlasNature& asn )
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
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_AtlasNatureParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eNatureAtlas;
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

// -----------------------------------------------------------------------------
// Name: MIL_AtlasNatureParameter::ToElement
// Created: MGD 2010-11-19
// -----------------------------------------------------------------------------
bool MIL_AtlasNatureParameter::ToElement( Common::MsgMissionParameter_Value& elem ) const
{
    return NET_ASN_Tools::CopyNatureAtlas( nature_, *elem.mutable_atlasnature() );
}
