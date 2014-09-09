// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_DotationTypeParameter.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "protocol/Protocol.h"
#include "MIL.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_DotationTypeParameter )

// -----------------------------------------------------------------------------
// Name: MIL_DotationTypeParameter constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
MIL_DotationTypeParameter::MIL_DotationTypeParameter()
    : pCategory_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationTypeParameter constructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_DotationTypeParameter::MIL_DotationTypeParameter( const sword::Id & asn )
    : pCategory_( PHY_DotationType::FindDotationCategory( asn.id() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationTypeParameter constructor
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
MIL_DotationTypeParameter::MIL_DotationTypeParameter( const PHY_DotationCategory* pCategory )
    : pCategory_( pCategory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationTypeParameter destructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_DotationTypeParameter::~MIL_DotationTypeParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationTypeParameter::IsOfType
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_DotationTypeParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eResourceType;
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationTypeParameter::ToDotationType
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_DotationTypeParameter::ToDotationType( const PHY_DotationCategory*& value ) const
{
    value = pCategory_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationTypeParameter::ToElement
// Created: MGD 2010-11-19
// -----------------------------------------------------------------------------
bool MIL_DotationTypeParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    if( !pCategory_ )
        return false;
    elem.mutable_resourcetype()->set_id( pCategory_->GetMosID() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationTypeParameter::load
// Created: LGY 2011-06-15
// -----------------------------------------------------------------------------
void MIL_DotationTypeParameter::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_BaseParameter >( *this );
    file >> pCategory_;
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationTypeParameter::save
// Created: LGY 2011-06-15
// -----------------------------------------------------------------------------
void MIL_DotationTypeParameter::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_BaseParameter >( *this );
    file << pCategory_;
}
